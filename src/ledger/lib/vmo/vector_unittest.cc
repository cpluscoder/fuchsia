// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/ledger/lib/vmo/vector.h"

#include "gtest/gtest.h"

namespace ledger {
namespace {

TEST(VmoVector, ShortVector) {
  std::vector<char> v(123, 'f');
  SizedVmo sb;
  EXPECT_TRUE(VmoFromVector(v, &sb));
  std::vector<char> v_out;
  EXPECT_TRUE(VectorFromVmo(std::move(sb), &v_out));
  EXPECT_EQ(v, v_out);

  ::fuchsia::mem::Buffer buf;
  EXPECT_TRUE(VmoFromVector(v, &buf));
  EXPECT_TRUE(VectorFromVmo(buf, &v_out));
  EXPECT_EQ(v, v_out);
}

TEST(VmoVector, EmptyVector) {
  std::vector<char> v;
  SizedVmo sb;
  EXPECT_TRUE(VmoFromVector(v, &sb));
  std::vector<char> v_out;
  EXPECT_TRUE(VectorFromVmo(std::move(sb), &v_out));
  EXPECT_EQ(v, v_out);

  ::fuchsia::mem::Buffer buf;
  EXPECT_TRUE(VmoFromVector(v, &buf));
  EXPECT_TRUE(VectorFromVmo(buf, &v_out));
  EXPECT_EQ(v, v_out);
}

TEST(VmoVector, ShortUnsignedVector) {
  std::vector<uint8_t> v(123, 42);
  SizedVmo sb;
  EXPECT_TRUE(VmoFromVector(v, &sb));
  std::vector<uint8_t> v_out;
  EXPECT_TRUE(VectorFromVmo(std::move(sb), &v_out));
  EXPECT_EQ(v, v_out);

  ::fuchsia::mem::Buffer buf;
  EXPECT_TRUE(VmoFromVector(v, &buf));
  EXPECT_TRUE(VectorFromVmo(buf, &v_out));
  EXPECT_EQ(v, v_out);
}

TEST(VmoVector, EmptyUnsignedVector) {
  std::vector<uint8_t> v;
  SizedVmo sb;
  EXPECT_TRUE(VmoFromVector(v, &sb));
  std::vector<uint8_t> v_out;
  EXPECT_TRUE(VectorFromVmo(std::move(sb), &v_out));
  EXPECT_EQ(v, v_out);

  ::fuchsia::mem::Buffer buf;
  EXPECT_TRUE(VmoFromVector(v, &buf));
  EXPECT_TRUE(VectorFromVmo(buf, &v_out));
  EXPECT_EQ(v, v_out);
}

}  // namespace
}  // namespace ledger