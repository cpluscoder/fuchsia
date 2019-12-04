// WARNING: This file is machine generated by fidlgen.

#include <fuchsia/net/llcpp/fidl.h>
#include <memory>

namespace llcpp {

namespace fuchsia {
namespace net {

namespace {

[[maybe_unused]]
constexpr uint64_t kConnectivity_OnNetworkReachable_Ordinal = 0x658708c800000000lu;
[[maybe_unused]]
constexpr uint64_t kConnectivity_OnNetworkReachable_GenOrdinal = 0x6f099dcaa3ff5b7lu;
extern "C" const fidl_type_t fuchsia_net_ConnectivityOnNetworkReachableRequestTable;
extern "C" const fidl_type_t fuchsia_net_ConnectivityOnNetworkReachableEventTable;
extern "C" const fidl_type_t v1_fuchsia_net_ConnectivityOnNetworkReachableEventTable;

}  // namespace
zx_status_t Connectivity::SyncClient::HandleEvents(Connectivity::EventHandlers handlers) {
  return Connectivity::Call::HandleEvents(::zx::unowned_channel(channel_), std::move(handlers));
}

zx_status_t Connectivity::Call::HandleEvents(::zx::unowned_channel client_end, Connectivity::EventHandlers handlers) {
  zx_status_t status = client_end->wait_one(ZX_CHANNEL_READABLE | ZX_CHANNEL_PEER_CLOSED,
                                            ::zx::time::infinite(),
                                            nullptr);
  if (status != ZX_OK) {
    return status;
  }
  constexpr uint32_t kReadAllocSize = ([]() constexpr {
    uint32_t x = 0;
    if (::fidl::internal::ClampedMessageSize<OnNetworkReachableResponse, ::fidl::MessageDirection::kReceiving>() >= x) {
      x = ::fidl::internal::ClampedMessageSize<OnNetworkReachableResponse, ::fidl::MessageDirection::kReceiving>();
    }
    return x;
  })();
  constexpr uint32_t kHandleAllocSize = ([]() constexpr {
    uint32_t x = 0;
    if (OnNetworkReachableResponse::MaxNumHandles >= x) {
      x = OnNetworkReachableResponse::MaxNumHandles;
    }
    if (x > ZX_CHANNEL_MAX_MSG_HANDLES) {
      x = ZX_CHANNEL_MAX_MSG_HANDLES;
    }
    return x;
  })();
  ::fidl::internal::ByteStorage<kReadAllocSize> read_storage;
  uint8_t* read_bytes = read_storage.buffer().data();
  zx_handle_t read_handles[kHandleAllocSize];
  uint32_t actual_bytes;
  uint32_t actual_handles;
  status = client_end->read(ZX_CHANNEL_READ_MAY_DISCARD,
                            read_bytes, read_handles,
                            kReadAllocSize, kHandleAllocSize,
                            &actual_bytes, &actual_handles);
  if (status == ZX_ERR_BUFFER_TOO_SMALL) {
    // Message size is unexpectedly larger than calculated.
    // This can only be due to a newer version of the protocol defining a new event,
    // whose size exceeds the maximum of known events in the current protocol.
    return handlers.unknown();
  }
  if (status != ZX_OK) {
    return status;
  }
  if (actual_bytes < sizeof(fidl_message_header_t)) {
    zx_handle_close_many(read_handles, actual_handles);
    return ZX_ERR_INVALID_ARGS;
  }
  auto msg = fidl_msg_t {
      .bytes = read_bytes,
      .handles = read_handles,
      .num_bytes = actual_bytes,
      .num_handles = actual_handles
  };
  fidl_message_header_t* hdr = reinterpret_cast<fidl_message_header_t*>(msg.bytes);
  status = fidl_validate_txn_header(hdr);
  if (status != ZX_OK) {
    return status;
  }
  switch (hdr->ordinal) {
    case kConnectivity_OnNetworkReachable_Ordinal:
    case kConnectivity_OnNetworkReachable_GenOrdinal:
    {
      auto result = ::fidl::DecodeAs<OnNetworkReachableResponse>(&msg);
      if (result.status != ZX_OK) {
        return result.status;
      }
      auto message = result.message.message();
      return handlers.on_network_reachable(std::move(message->reachable));
    }
    default:
      zx_handle_close_many(read_handles, actual_handles);
      return handlers.unknown();
  }
}

bool Connectivity::TryDispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn) {
  if (msg->num_bytes < sizeof(fidl_message_header_t)) {
    zx_handle_close_many(msg->handles, msg->num_handles);
    txn->Close(ZX_ERR_INVALID_ARGS);
    return true;
  }
  fidl_message_header_t* hdr = reinterpret_cast<fidl_message_header_t*>(msg->bytes);
  zx_status_t status = fidl_validate_txn_header(hdr);
  if (status != ZX_OK) {
    txn->Close(status);
    return true;
  }
  switch (hdr->ordinal) {
    default: {
      return false;
    }
  }
}

bool Connectivity::Dispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn) {
  bool found = TryDispatch(impl, msg, txn);
  if (!found) {
    zx_handle_close_many(msg->handles, msg->num_handles);
    txn->Close(ZX_ERR_NOT_SUPPORTED);
  }
  return found;
}


zx_status_t Connectivity::SendOnNetworkReachableEvent(::zx::unowned_channel _chan, bool reachable) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<OnNetworkReachableResponse, ::fidl::MessageDirection::kSending>();
  FIDL_ALIGNDECL uint8_t _write_bytes[_kWriteAllocSize] = {};
  auto& _response = *reinterpret_cast<OnNetworkReachableResponse*>(_write_bytes);
  Connectivity::SetTransactionHeaderFor::OnNetworkReachableResponse(
      ::fidl::DecodedMessage<OnNetworkReachableResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              OnNetworkReachableResponse::PrimarySize,
              OnNetworkReachableResponse::PrimarySize)));
  _response.reachable = std::move(reachable);
  ::fidl::BytePart _response_bytes(_write_bytes, _kWriteAllocSize, sizeof(OnNetworkReachableResponse));
  return ::fidl::Write(::zx::unowned_channel(_chan), ::fidl::DecodedMessage<OnNetworkReachableResponse>(std::move(_response_bytes)));
}

zx_status_t Connectivity::SendOnNetworkReachableEvent(::zx::unowned_channel _chan, ::fidl::BytePart _buffer, bool reachable) {
  if (_buffer.capacity() < OnNetworkReachableResponse::PrimarySize) {
    return ZX_ERR_BUFFER_TOO_SMALL;
  }
  auto& _response = *reinterpret_cast<OnNetworkReachableResponse*>(_buffer.data());
  Connectivity::SetTransactionHeaderFor::OnNetworkReachableResponse(
      ::fidl::DecodedMessage<OnNetworkReachableResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              OnNetworkReachableResponse::PrimarySize,
              OnNetworkReachableResponse::PrimarySize)));
  _response.reachable = std::move(reachable);
  _buffer.set_actual(sizeof(OnNetworkReachableResponse));
  return ::fidl::Write(::zx::unowned_channel(_chan), ::fidl::DecodedMessage<OnNetworkReachableResponse>(std::move(_buffer)));
}

zx_status_t Connectivity::SendOnNetworkReachableEvent(::zx::unowned_channel _chan, ::fidl::DecodedMessage<OnNetworkReachableResponse> params) {
  Connectivity::SetTransactionHeaderFor::OnNetworkReachableResponse(params);
  return ::fidl::Write(::zx::unowned_channel(_chan), std::move(params));
}



void Connectivity::SetTransactionHeaderFor::OnNetworkReachableResponse(const ::fidl::DecodedMessage<Connectivity::OnNetworkReachableResponse>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kConnectivity_OnNetworkReachable_GenOrdinal);
}

::llcpp::fuchsia::net::NameLookup_LookupHostname_Result::NameLookup_LookupHostname_Result() {
  ordinal_ = Ordinal::Invalid;
}

::llcpp::fuchsia::net::NameLookup_LookupHostname_Result::~NameLookup_LookupHostname_Result() {
  Destroy();
}

void ::llcpp::fuchsia::net::NameLookup_LookupHostname_Result::Destroy() {
  switch (ordinal_) {
  case Ordinal::kResponse:
    response_.~NameLookup_LookupHostname_Response();
    break;
  case Ordinal::kErr:
    err_.~LookupError();
    break;
  default:
    break;
  }
  ordinal_ = Ordinal::Invalid;
}

void ::llcpp::fuchsia::net::NameLookup_LookupHostname_Result::MoveImpl_(NameLookup_LookupHostname_Result&& other) {
  switch (other.ordinal_) {
  case Ordinal::kResponse:
    mutable_response() = std::move(other.mutable_response());
    break;
  case Ordinal::kErr:
    mutable_err() = std::move(other.mutable_err());
    break;
  default:
    break;
  }
  other.Destroy();
}

void ::llcpp::fuchsia::net::NameLookup_LookupHostname_Result::SizeAndOffsetAssertionHelper() {
  static_assert(offsetof(::llcpp::fuchsia::net::NameLookup_LookupHostname_Result, response_) == 8);
  static_assert(offsetof(::llcpp::fuchsia::net::NameLookup_LookupHostname_Result, err_) == 8);
  static_assert(sizeof(::llcpp::fuchsia::net::NameLookup_LookupHostname_Result) == ::llcpp::fuchsia::net::NameLookup_LookupHostname_Result::PrimarySize);
}


::llcpp::fuchsia::net::NameLookup_LookupHostname_Response& ::llcpp::fuchsia::net::NameLookup_LookupHostname_Result::mutable_response() {
  if (ordinal_ != Ordinal::kResponse) {
    Destroy();
    new (&response_) ::llcpp::fuchsia::net::NameLookup_LookupHostname_Response;
    ordinal_ = Ordinal::kResponse;
  }
  return response_;
}

::llcpp::fuchsia::net::LookupError& ::llcpp::fuchsia::net::NameLookup_LookupHostname_Result::mutable_err() {
  if (ordinal_ != Ordinal::kErr) {
    Destroy();
    new (&err_) ::llcpp::fuchsia::net::LookupError;
    ordinal_ = Ordinal::kErr;
  }
  return err_;
}


::llcpp::fuchsia::net::NameLookup_LookupIp_Result::NameLookup_LookupIp_Result() {
  ordinal_ = Ordinal::Invalid;
}

::llcpp::fuchsia::net::NameLookup_LookupIp_Result::~NameLookup_LookupIp_Result() {
  Destroy();
}

void ::llcpp::fuchsia::net::NameLookup_LookupIp_Result::Destroy() {
  switch (ordinal_) {
  case Ordinal::kResponse:
    response_.~NameLookup_LookupIp_Response();
    break;
  case Ordinal::kErr:
    err_.~LookupError();
    break;
  default:
    break;
  }
  ordinal_ = Ordinal::Invalid;
}

void ::llcpp::fuchsia::net::NameLookup_LookupIp_Result::MoveImpl_(NameLookup_LookupIp_Result&& other) {
  switch (other.ordinal_) {
  case Ordinal::kResponse:
    mutable_response() = std::move(other.mutable_response());
    break;
  case Ordinal::kErr:
    mutable_err() = std::move(other.mutable_err());
    break;
  default:
    break;
  }
  other.Destroy();
}

void ::llcpp::fuchsia::net::NameLookup_LookupIp_Result::SizeAndOffsetAssertionHelper() {
  static_assert(offsetof(::llcpp::fuchsia::net::NameLookup_LookupIp_Result, response_) == 8);
  static_assert(offsetof(::llcpp::fuchsia::net::NameLookup_LookupIp_Result, err_) == 8);
  static_assert(sizeof(::llcpp::fuchsia::net::NameLookup_LookupIp_Result) == ::llcpp::fuchsia::net::NameLookup_LookupIp_Result::PrimarySize);
}


::llcpp::fuchsia::net::NameLookup_LookupIp_Response& ::llcpp::fuchsia::net::NameLookup_LookupIp_Result::mutable_response() {
  if (ordinal_ != Ordinal::kResponse) {
    Destroy();
    new (&response_) ::llcpp::fuchsia::net::NameLookup_LookupIp_Response;
    ordinal_ = Ordinal::kResponse;
  }
  return response_;
}

::llcpp::fuchsia::net::LookupError& ::llcpp::fuchsia::net::NameLookup_LookupIp_Result::mutable_err() {
  if (ordinal_ != Ordinal::kErr) {
    Destroy();
    new (&err_) ::llcpp::fuchsia::net::LookupError;
    ordinal_ = Ordinal::kErr;
  }
  return err_;
}


::llcpp::fuchsia::net::IpAddress::IpAddress() {
  ordinal_ = Ordinal::Invalid;
}

::llcpp::fuchsia::net::IpAddress::~IpAddress() {
  Destroy();
}

void ::llcpp::fuchsia::net::IpAddress::Destroy() {
  switch (ordinal_) {
  case Ordinal::kIpv4:
    ipv4_.~Ipv4Address();
    break;
  case Ordinal::kIpv6:
    ipv6_.~Ipv6Address();
    break;
  default:
    break;
  }
  ordinal_ = Ordinal::Invalid;
}

void ::llcpp::fuchsia::net::IpAddress::MoveImpl_(IpAddress&& other) {
  switch (other.ordinal_) {
  case Ordinal::kIpv4:
    mutable_ipv4() = std::move(other.mutable_ipv4());
    break;
  case Ordinal::kIpv6:
    mutable_ipv6() = std::move(other.mutable_ipv6());
    break;
  default:
    break;
  }
  other.Destroy();
}

void ::llcpp::fuchsia::net::IpAddress::SizeAndOffsetAssertionHelper() {
  static_assert(offsetof(::llcpp::fuchsia::net::IpAddress, ipv4_) == 4);
  static_assert(offsetof(::llcpp::fuchsia::net::IpAddress, ipv6_) == 4);
  static_assert(sizeof(::llcpp::fuchsia::net::IpAddress) == ::llcpp::fuchsia::net::IpAddress::PrimarySize);
}


::llcpp::fuchsia::net::Ipv4Address& ::llcpp::fuchsia::net::IpAddress::mutable_ipv4() {
  if (ordinal_ != Ordinal::kIpv4) {
    Destroy();
    new (&ipv4_) ::llcpp::fuchsia::net::Ipv4Address;
    ordinal_ = Ordinal::kIpv4;
  }
  return ipv4_;
}

::llcpp::fuchsia::net::Ipv6Address& ::llcpp::fuchsia::net::IpAddress::mutable_ipv6() {
  if (ordinal_ != Ordinal::kIpv6) {
    Destroy();
    new (&ipv6_) ::llcpp::fuchsia::net::Ipv6Address;
    ordinal_ = Ordinal::kIpv6;
  }
  return ipv6_;
}


namespace {

[[maybe_unused]]
constexpr uint64_t kNameLookup_LookupIp_Ordinal = 0x30c22b4c00000000lu;
[[maybe_unused]]
constexpr uint64_t kNameLookup_LookupIp_GenOrdinal = 0x58576c7210cd0f32lu;
extern "C" const fidl_type_t fuchsia_net_NameLookupLookupIpRequestTable;
extern "C" const fidl_type_t fuchsia_net_NameLookupLookupIpResponseTable;
extern "C" const fidl_type_t v1_fuchsia_net_NameLookupLookupIpResponseTable;
[[maybe_unused]]
constexpr uint64_t kNameLookup_LookupHostname_Ordinal = 0x17582c9400000000lu;
[[maybe_unused]]
constexpr uint64_t kNameLookup_LookupHostname_GenOrdinal = 0x5dfea9b2c92f510alu;
extern "C" const fidl_type_t fuchsia_net_NameLookupLookupHostnameRequestTable;
extern "C" const fidl_type_t fuchsia_net_NameLookupLookupHostnameResponseTable;
extern "C" const fidl_type_t v1_fuchsia_net_NameLookupLookupHostnameResponseTable;

}  // namespace
template <>
NameLookup::ResultOf::LookupIp_Impl<NameLookup::LookupIpResponse>::LookupIp_Impl(::zx::unowned_channel _client_end, ::fidl::StringView hostname, ::llcpp::fuchsia::net::LookupIpOptions options) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<LookupIpRequest, ::fidl::MessageDirection::kSending>();
  ::fidl::internal::AlignedBuffer<_kWriteAllocSize> _write_bytes_inlined;
  auto& _write_bytes_array = _write_bytes_inlined;
  LookupIpRequest _request = {};
  _request.hostname = std::move(hostname);
  _request.options = std::move(options);
  auto _linearize_result = ::fidl::Linearize(&_request, _write_bytes_array.view());
  if (_linearize_result.status != ZX_OK) {
    Super::SetFailure(std::move(_linearize_result));
    return;
  }
  ::fidl::DecodedMessage<LookupIpRequest> _decoded_request = std::move(_linearize_result.message);
  Super::SetResult(
      NameLookup::InPlace::LookupIp(std::move(_client_end), std::move(_decoded_request), Super::response_buffer()));
}

NameLookup::ResultOf::LookupIp NameLookup::SyncClient::LookupIp(::fidl::StringView hostname, ::llcpp::fuchsia::net::LookupIpOptions options) {
    return ResultOf::LookupIp(::zx::unowned_channel(this->channel_), std::move(hostname), std::move(options));
}

NameLookup::ResultOf::LookupIp NameLookup::Call::LookupIp(::zx::unowned_channel _client_end, ::fidl::StringView hostname, ::llcpp::fuchsia::net::LookupIpOptions options) {
  return ResultOf::LookupIp(std::move(_client_end), std::move(hostname), std::move(options));
}

template <>
NameLookup::UnownedResultOf::LookupIp_Impl<NameLookup::LookupIpResponse>::LookupIp_Impl(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::fidl::StringView hostname, ::llcpp::fuchsia::net::LookupIpOptions options, ::fidl::BytePart _response_buffer) {
  if (_request_buffer.capacity() < LookupIpRequest::PrimarySize) {
    Super::SetFailure(::fidl::DecodeResult<LookupIpResponse>(ZX_ERR_BUFFER_TOO_SMALL, ::fidl::internal::kErrorRequestBufferTooSmall));
    return;
  }
  LookupIpRequest _request = {};
  _request.hostname = std::move(hostname);
  _request.options = std::move(options);
  auto _linearize_result = ::fidl::Linearize(&_request, std::move(_request_buffer));
  if (_linearize_result.status != ZX_OK) {
    Super::SetFailure(std::move(_linearize_result));
    return;
  }
  ::fidl::DecodedMessage<LookupIpRequest> _decoded_request = std::move(_linearize_result.message);
  Super::SetResult(
      NameLookup::InPlace::LookupIp(std::move(_client_end), std::move(_decoded_request), std::move(_response_buffer)));
}

NameLookup::UnownedResultOf::LookupIp NameLookup::SyncClient::LookupIp(::fidl::BytePart _request_buffer, ::fidl::StringView hostname, ::llcpp::fuchsia::net::LookupIpOptions options, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::LookupIp(::zx::unowned_channel(this->channel_), std::move(_request_buffer), std::move(hostname), std::move(options), std::move(_response_buffer));
}

NameLookup::UnownedResultOf::LookupIp NameLookup::Call::LookupIp(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::fidl::StringView hostname, ::llcpp::fuchsia::net::LookupIpOptions options, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::LookupIp(std::move(_client_end), std::move(_request_buffer), std::move(hostname), std::move(options), std::move(_response_buffer));
}

::fidl::DecodeResult<NameLookup::LookupIpResponse> NameLookup::InPlace::LookupIp(::zx::unowned_channel _client_end, ::fidl::DecodedMessage<LookupIpRequest> params, ::fidl::BytePart response_buffer) {
  NameLookup::SetTransactionHeaderFor::LookupIpRequest(params);
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::DecodeResult<NameLookup::LookupIpResponse>::FromFailure(
        std::move(_encode_request_result));
  }
  auto _call_result = ::fidl::Call<LookupIpRequest, LookupIpResponse>(
    std::move(_client_end), std::move(_encode_request_result.message), std::move(response_buffer));
  if (_call_result.status != ZX_OK) {
    return ::fidl::DecodeResult<NameLookup::LookupIpResponse>::FromFailure(
        std::move(_call_result));
  }
  return ::fidl::Decode(std::move(_call_result.message));
}

template <>
NameLookup::ResultOf::LookupHostname_Impl<NameLookup::LookupHostnameResponse>::LookupHostname_Impl(::zx::unowned_channel _client_end, ::llcpp::fuchsia::net::IpAddress addr) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<LookupHostnameRequest, ::fidl::MessageDirection::kSending>();
  ::fidl::internal::AlignedBuffer<_kWriteAllocSize> _write_bytes_inlined;
  auto& _write_bytes_array = _write_bytes_inlined;
  uint8_t* _write_bytes = _write_bytes_array.view().data();
  memset(_write_bytes, 0, LookupHostnameRequest::PrimarySize);
  auto& _request = *reinterpret_cast<LookupHostnameRequest*>(_write_bytes);
  _request.addr = std::move(addr);
  ::fidl::BytePart _request_bytes(_write_bytes, _kWriteAllocSize, sizeof(LookupHostnameRequest));
  ::fidl::DecodedMessage<LookupHostnameRequest> _decoded_request(std::move(_request_bytes));
  Super::SetResult(
      NameLookup::InPlace::LookupHostname(std::move(_client_end), std::move(_decoded_request), Super::response_buffer()));
}

NameLookup::ResultOf::LookupHostname NameLookup::SyncClient::LookupHostname(::llcpp::fuchsia::net::IpAddress addr) {
    return ResultOf::LookupHostname(::zx::unowned_channel(this->channel_), std::move(addr));
}

NameLookup::ResultOf::LookupHostname NameLookup::Call::LookupHostname(::zx::unowned_channel _client_end, ::llcpp::fuchsia::net::IpAddress addr) {
  return ResultOf::LookupHostname(std::move(_client_end), std::move(addr));
}

template <>
NameLookup::UnownedResultOf::LookupHostname_Impl<NameLookup::LookupHostnameResponse>::LookupHostname_Impl(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::llcpp::fuchsia::net::IpAddress addr, ::fidl::BytePart _response_buffer) {
  if (_request_buffer.capacity() < LookupHostnameRequest::PrimarySize) {
    Super::SetFailure(::fidl::DecodeResult<LookupHostnameResponse>(ZX_ERR_BUFFER_TOO_SMALL, ::fidl::internal::kErrorRequestBufferTooSmall));
    return;
  }
  memset(_request_buffer.data(), 0, LookupHostnameRequest::PrimarySize);
  auto& _request = *reinterpret_cast<LookupHostnameRequest*>(_request_buffer.data());
  _request.addr = std::move(addr);
  _request_buffer.set_actual(sizeof(LookupHostnameRequest));
  ::fidl::DecodedMessage<LookupHostnameRequest> _decoded_request(std::move(_request_buffer));
  Super::SetResult(
      NameLookup::InPlace::LookupHostname(std::move(_client_end), std::move(_decoded_request), std::move(_response_buffer)));
}

NameLookup::UnownedResultOf::LookupHostname NameLookup::SyncClient::LookupHostname(::fidl::BytePart _request_buffer, ::llcpp::fuchsia::net::IpAddress addr, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::LookupHostname(::zx::unowned_channel(this->channel_), std::move(_request_buffer), std::move(addr), std::move(_response_buffer));
}

NameLookup::UnownedResultOf::LookupHostname NameLookup::Call::LookupHostname(::zx::unowned_channel _client_end, ::fidl::BytePart _request_buffer, ::llcpp::fuchsia::net::IpAddress addr, ::fidl::BytePart _response_buffer) {
  return UnownedResultOf::LookupHostname(std::move(_client_end), std::move(_request_buffer), std::move(addr), std::move(_response_buffer));
}

::fidl::DecodeResult<NameLookup::LookupHostnameResponse> NameLookup::InPlace::LookupHostname(::zx::unowned_channel _client_end, ::fidl::DecodedMessage<LookupHostnameRequest> params, ::fidl::BytePart response_buffer) {
  NameLookup::SetTransactionHeaderFor::LookupHostnameRequest(params);
  auto _encode_request_result = ::fidl::Encode(std::move(params));
  if (_encode_request_result.status != ZX_OK) {
    return ::fidl::DecodeResult<NameLookup::LookupHostnameResponse>::FromFailure(
        std::move(_encode_request_result));
  }
  auto _call_result = ::fidl::Call<LookupHostnameRequest, LookupHostnameResponse>(
    std::move(_client_end), std::move(_encode_request_result.message), std::move(response_buffer));
  if (_call_result.status != ZX_OK) {
    return ::fidl::DecodeResult<NameLookup::LookupHostnameResponse>::FromFailure(
        std::move(_call_result));
  }
  return ::fidl::Decode(std::move(_call_result.message));
}


bool NameLookup::TryDispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn) {
  if (msg->num_bytes < sizeof(fidl_message_header_t)) {
    zx_handle_close_many(msg->handles, msg->num_handles);
    txn->Close(ZX_ERR_INVALID_ARGS);
    return true;
  }
  fidl_message_header_t* hdr = reinterpret_cast<fidl_message_header_t*>(msg->bytes);
  zx_status_t status = fidl_validate_txn_header(hdr);
  if (status != ZX_OK) {
    txn->Close(status);
    return true;
  }
  switch (hdr->ordinal) {
    case kNameLookup_LookupIp_Ordinal:
    case kNameLookup_LookupIp_GenOrdinal:
    {
      auto result = ::fidl::DecodeAs<LookupIpRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      auto message = result.message.message();
      impl->LookupIp(std::move(message->hostname), std::move(message->options),
          Interface::LookupIpCompleter::Sync(txn));
      return true;
    }
    case kNameLookup_LookupHostname_Ordinal:
    case kNameLookup_LookupHostname_GenOrdinal:
    {
      constexpr uint32_t kTransformerDestSize = ::fidl::internal::ClampedMessageSize<LookupHostnameRequest, ::fidl::MessageDirection::kReceiving>();
      ::fidl::internal::ByteStorage<kTransformerDestSize> transformer_dest_storage(::fidl::internal::DelayAllocation);
      if (fidl_should_decode_union_from_xunion(hdr)) {
        transformer_dest_storage.Allocate();
        uint8_t* transformer_dest = transformer_dest_storage.buffer().data();
        zx_status_t transform_status = fidl_transform(FIDL_TRANSFORMATION_V1_TO_OLD,
                                                      LookupHostnameRequest::AltType,
                                                      reinterpret_cast<uint8_t*>(msg->bytes),
                                                      msg->num_bytes,
                                                      transformer_dest,
                                                      kTransformerDestSize,
                                                      &msg->num_bytes,
                                                      nullptr);
        if (transform_status != ZX_OK) {
          txn->Close(ZX_ERR_INVALID_ARGS);
          zx_handle_close_many(msg->handles, msg->num_handles);
          return true;
        }
        msg->bytes = transformer_dest;
      }
      auto result = ::fidl::DecodeAs<LookupHostnameRequest>(msg);
      if (result.status != ZX_OK) {
        txn->Close(ZX_ERR_INVALID_ARGS);
        return true;
      }
      auto message = result.message.message();
      impl->LookupHostname(std::move(message->addr),
          Interface::LookupHostnameCompleter::Sync(txn));
      return true;
    }
    default: {
      return false;
    }
  }
}

bool NameLookup::Dispatch(Interface* impl, fidl_msg_t* msg, ::fidl::Transaction* txn) {
  bool found = TryDispatch(impl, msg, txn);
  if (!found) {
    zx_handle_close_many(msg->handles, msg->num_handles);
    txn->Close(ZX_ERR_NOT_SUPPORTED);
  }
  return found;
}


void NameLookup::Interface::LookupIpCompleterBase::Reply(::llcpp::fuchsia::net::NameLookup_LookupIp_Result result) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<LookupIpResponse, ::fidl::MessageDirection::kSending>();
  std::unique_ptr<uint8_t[]> _write_bytes_unique_ptr(new uint8_t[_kWriteAllocSize]);
  uint8_t* _write_bytes = _write_bytes_unique_ptr.get();
  LookupIpResponse _response = {};
  NameLookup::SetTransactionHeaderFor::LookupIpResponse(
      ::fidl::DecodedMessage<LookupIpResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              LookupIpResponse::PrimarySize,
              LookupIpResponse::PrimarySize)));
  _response.result = std::move(result);
  auto _linearize_result = ::fidl::Linearize(&_response, ::fidl::BytePart(_write_bytes,
                                                                          _kWriteAllocSize));
  if (_linearize_result.status != ZX_OK) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  CompleterBase::SendReply(std::move(_linearize_result.message));
}
void NameLookup::Interface::LookupIpCompleterBase::ReplySuccess(::llcpp::fuchsia::net::IpAddressInfo addr) {
  NameLookup_LookupIp_Response response;
  response.addr = std::move(addr);

  Reply(NameLookup_LookupIp_Result::WithResponse(&response));
}
void NameLookup::Interface::LookupIpCompleterBase::ReplyError(LookupError error) {
  Reply(NameLookup_LookupIp_Result::WithErr(&error));
}

void NameLookup::Interface::LookupIpCompleterBase::Reply(::fidl::BytePart _buffer, ::llcpp::fuchsia::net::NameLookup_LookupIp_Result result) {
  if (_buffer.capacity() < LookupIpResponse::PrimarySize) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  LookupIpResponse _response = {};
  NameLookup::SetTransactionHeaderFor::LookupIpResponse(
      ::fidl::DecodedMessage<LookupIpResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              LookupIpResponse::PrimarySize,
              LookupIpResponse::PrimarySize)));
  _response.result = std::move(result);
  auto _linearize_result = ::fidl::Linearize(&_response, std::move(_buffer));
  if (_linearize_result.status != ZX_OK) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  CompleterBase::SendReply(std::move(_linearize_result.message));
}
void NameLookup::Interface::LookupIpCompleterBase::ReplySuccess(::fidl::BytePart _buffer, ::llcpp::fuchsia::net::IpAddressInfo addr) {
  NameLookup_LookupIp_Response response;
  response.addr = std::move(addr);

  Reply(std::move(_buffer), NameLookup_LookupIp_Result::WithResponse(&response));
}

void NameLookup::Interface::LookupIpCompleterBase::Reply(::fidl::DecodedMessage<LookupIpResponse> params) {
  NameLookup::SetTransactionHeaderFor::LookupIpResponse(params);
  CompleterBase::SendReply(std::move(params));
}


void NameLookup::Interface::LookupHostnameCompleterBase::Reply(::llcpp::fuchsia::net::NameLookup_LookupHostname_Result result) {
  constexpr uint32_t _kWriteAllocSize = ::fidl::internal::ClampedMessageSize<LookupHostnameResponse, ::fidl::MessageDirection::kSending>();
  FIDL_ALIGNDECL uint8_t _write_bytes[_kWriteAllocSize];
  LookupHostnameResponse _response = {};
  NameLookup::SetTransactionHeaderFor::LookupHostnameResponse(
      ::fidl::DecodedMessage<LookupHostnameResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              LookupHostnameResponse::PrimarySize,
              LookupHostnameResponse::PrimarySize)));
  _response.result = std::move(result);
  auto _linearize_result = ::fidl::Linearize(&_response, ::fidl::BytePart(_write_bytes,
                                                                          _kWriteAllocSize));
  if (_linearize_result.status != ZX_OK) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  CompleterBase::SendReply(std::move(_linearize_result.message));
}
void NameLookup::Interface::LookupHostnameCompleterBase::ReplySuccess(::fidl::StringView hostname) {
  NameLookup_LookupHostname_Response response;
  response.hostname = std::move(hostname);

  Reply(NameLookup_LookupHostname_Result::WithResponse(&response));
}
void NameLookup::Interface::LookupHostnameCompleterBase::ReplyError(LookupError error) {
  Reply(NameLookup_LookupHostname_Result::WithErr(&error));
}

void NameLookup::Interface::LookupHostnameCompleterBase::Reply(::fidl::BytePart _buffer, ::llcpp::fuchsia::net::NameLookup_LookupHostname_Result result) {
  if (_buffer.capacity() < LookupHostnameResponse::PrimarySize) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  LookupHostnameResponse _response = {};
  NameLookup::SetTransactionHeaderFor::LookupHostnameResponse(
      ::fidl::DecodedMessage<LookupHostnameResponse>(
          ::fidl::BytePart(reinterpret_cast<uint8_t*>(&_response),
              LookupHostnameResponse::PrimarySize,
              LookupHostnameResponse::PrimarySize)));
  _response.result = std::move(result);
  auto _linearize_result = ::fidl::Linearize(&_response, std::move(_buffer));
  if (_linearize_result.status != ZX_OK) {
    CompleterBase::Close(ZX_ERR_INTERNAL);
    return;
  }
  CompleterBase::SendReply(std::move(_linearize_result.message));
}
void NameLookup::Interface::LookupHostnameCompleterBase::ReplySuccess(::fidl::BytePart _buffer, ::fidl::StringView hostname) {
  NameLookup_LookupHostname_Response response;
  response.hostname = std::move(hostname);

  Reply(std::move(_buffer), NameLookup_LookupHostname_Result::WithResponse(&response));
}

void NameLookup::Interface::LookupHostnameCompleterBase::Reply(::fidl::DecodedMessage<LookupHostnameResponse> params) {
  NameLookup::SetTransactionHeaderFor::LookupHostnameResponse(params);
  CompleterBase::SendReply(std::move(params));
}



void NameLookup::SetTransactionHeaderFor::LookupIpRequest(const ::fidl::DecodedMessage<NameLookup::LookupIpRequest>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kNameLookup_LookupIp_GenOrdinal);
}
void NameLookup::SetTransactionHeaderFor::LookupIpResponse(const ::fidl::DecodedMessage<NameLookup::LookupIpResponse>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kNameLookup_LookupIp_GenOrdinal);
}

void NameLookup::SetTransactionHeaderFor::LookupHostnameRequest(const ::fidl::DecodedMessage<NameLookup::LookupHostnameRequest>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kNameLookup_LookupHostname_GenOrdinal);
}
void NameLookup::SetTransactionHeaderFor::LookupHostnameResponse(const ::fidl::DecodedMessage<NameLookup::LookupHostnameResponse>& _msg) {
  fidl_init_txn_header(&_msg.message()->_hdr, 0, kNameLookup_LookupHostname_GenOrdinal);
}

}  // namespace net
}  // namespace fuchsia
}  // namespace llcpp
