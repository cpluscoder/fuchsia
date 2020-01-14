// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <fuchsia/io2/llcpp/fidl.h>
#include <lib/async-loop/cpp/loop.h>
#include <lib/async-loop/default.h>
#include <lib/fidl-async/cpp/bind.h>
#include <lib/zxio/inception.h>
#include <lib/zxio/ops.h>
#include <lib/zxio/zxio.h>

#include <atomic>
#include <memory>

#include <zxtest/zxtest.h>

namespace {

namespace fio2 = ::llcpp::fuchsia::io2;

class TestServerBase : public fio2::Node::Interface {
 public:
  TestServerBase() = default;
  virtual ~TestServerBase() = default;

  void Reopen(fio2::ConnectionOptions options, ::zx::channel object_request,
              ReopenCompleter::Sync completer) override {
    completer.Close(ZX_ERR_NOT_SUPPORTED);
  }

  // Exercised by |zxio_close|.
  void Close(CloseCompleter::Sync completer) override {
    num_close_.fetch_add(1);
    completer.Close(ZX_OK);
  }

  void Describe(fio2::ConnectionInfoQuery query, DescribeCompleter::Sync completer) override {
    completer.Close(ZX_ERR_NOT_SUPPORTED);
  }

  void GetToken(GetTokenCompleter::Sync completer) override {
    completer.Close(ZX_ERR_NOT_SUPPORTED);
  }

  void GetAttributes(fio2::NodeAttributesQuery query,
                     GetAttributesCompleter::Sync completer) override {
    completer.Close(ZX_ERR_NOT_SUPPORTED);
  }

  void UpdateAttributes(fio2::NodeAttributes attributes,
                        UpdateAttributesCompleter::Sync completer) override {
    completer.Close(ZX_ERR_NOT_SUPPORTED);
  }

  void Sync(SyncCompleter::Sync completer) override { completer.Close(ZX_ERR_NOT_SUPPORTED); }

  uint32_t num_close() const { return num_close_.load(); }

 private:
  std::atomic<uint32_t> num_close_ = 0;
};

class RemoteV2 : public zxtest::Test {
 public:
  void SetUp() final {
    ASSERT_OK(zx::channel::create(0, &control_client_end_, &control_server_end_));
    ASSERT_OK(zxio_remote_v2_init(&remote_, control_client_end_.release(), ZX_HANDLE_INVALID));
  }

  template <typename ServerImpl>
  ServerImpl* StartServer() {
    server_ = std::make_unique<ServerImpl>();
    loop_ = std::make_unique<async::Loop>(&kAsyncLoopConfigNoAttachToCurrentThread);
    zx_status_t status;
    EXPECT_OK(status = loop_->StartThread("fake-filesystem"));
    if (status != ZX_OK) {
      return nullptr;
    }
    EXPECT_OK(fidl::Bind(loop_->dispatcher(), std::move(control_server_end_), server_.get()));
    if (status != ZX_OK) {
      return nullptr;
    }
    return static_cast<ServerImpl*>(server_.get());
  }

  void TearDown() final {
    ASSERT_EQ(0, server_->num_close());
    ASSERT_OK(zxio_close(&remote_.io));
    ASSERT_EQ(1, server_->num_close());
  }

 protected:
  zxio_storage_t remote_;
  zx::channel control_client_end_;
  zx::channel control_server_end_;
  std::unique_ptr<TestServerBase> server_;
  std::unique_ptr<async::Loop> loop_;
};

TEST_F(RemoteV2, GetAttributes) {
  constexpr uint64_t kContentSize = 42;
  class TestServer : public TestServerBase {
   public:
    void GetAttributes(fio2::NodeAttributesQuery query,
                       GetAttributesCompleter::Sync completer) override {
      EXPECT_EQ(query, fio2::NodeAttributesQuery::mask);
      uint64_t content_size = kContentSize;
      fio2::NodeProtocolSet protocols = fio2::NodeProtocolSet::FILE;
      auto builder = fio2::NodeAttributes::Build()
          .set_content_size(&content_size)
          .set_protocols(&protocols);
      completer.ReplySuccess(builder.view());
    }
  };
  ASSERT_NO_FAILURES(StartServer<TestServer>());

  zxio_node_attr_t attr = {};
  ASSERT_OK(zxio_attr_get(&remote_.io, &attr));

  EXPECT_TRUE(attr.has.protocols);
  EXPECT_EQ(ZXIO_NODE_PROTOCOL_FILE, attr.protocols);
  EXPECT_TRUE(attr.has.content_size);
  EXPECT_EQ(kContentSize, attr.content_size);
  EXPECT_FALSE(attr.has.storage_size);
  EXPECT_FALSE(attr.has.abilities);
  EXPECT_FALSE(attr.has.creation_time);
  EXPECT_FALSE(attr.has.modification_time);
  EXPECT_FALSE(attr.has.id);
  EXPECT_FALSE(attr.has.link_count);
}

TEST_F(RemoteV2, GetAttributesError) {
  class TestServer : public TestServerBase {
   public:
    void GetAttributes(fio2::NodeAttributesQuery query,
                       GetAttributesCompleter::Sync completer) override {
      completer.ReplyError(ZX_ERR_INVALID_ARGS);
    }
  };
  ASSERT_NO_FAILURES(StartServer<TestServer>());

  zxio_node_attr_t attr = {};
  EXPECT_EQ(ZX_ERR_INVALID_ARGS, zxio_attr_get(&remote_.io, &attr));
}

TEST_F(RemoteV2, SetAttributes) {
  constexpr uint64_t kCreationTime = 123;
  class TestServer : public TestServerBase {
   public:
    void UpdateAttributes(fio2::NodeAttributes attributes,
                          UpdateAttributesCompleter::Sync completer) override {
      EXPECT_TRUE(attributes.has_creation_time());
      EXPECT_FALSE(attributes.has_protocols());
      EXPECT_FALSE(attributes.has_abilities());
      EXPECT_FALSE(attributes.has_modification_time());
      EXPECT_FALSE(attributes.has_content_size());
      EXPECT_FALSE(attributes.has_storage_size());
      EXPECT_FALSE(attributes.has_link_count());

      uint64_t creation_time = kCreationTime;
      EXPECT_EQ(creation_time, attributes.creation_time());
      completer.ReplySuccess();
      called_.store(true);
    }

    std::atomic<bool> called_ = false;
  };

  TestServer* server;
  ASSERT_NO_FAILURES(server = StartServer<TestServer>());

  zxio_node_attr_t attr = {};
  ZXIO_NODE_ATTR_SET(attr, creation_time, kCreationTime);
  ASSERT_OK(zxio_attr_set(&remote_.io, &attr));
  EXPECT_TRUE(server->called_.load());
}

TEST_F(RemoteV2, SetAttributesError) {
  class TestServer : public TestServerBase {
   public:
    void UpdateAttributes(fio2::NodeAttributes attributes,
                          UpdateAttributesCompleter::Sync completer) override {
      completer.ReplyError(ZX_ERR_INVALID_ARGS);
    }
  };
  ASSERT_NO_FAILURES(StartServer<TestServer>());

  zxio_node_attr_t attr = {};
  EXPECT_EQ(ZX_ERR_INVALID_ARGS, zxio_attr_set(&remote_.io, &attr));
}

}  // namespace
