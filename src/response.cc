// Copyright (c) The Feeds Team. All rights reserved.

#include "response.h"

namespace json_rpc {

Json Response::ToJson() const {
  Json json;
  json["jsonrpc"] = JsonrpcVersion();
  if (error_.Code() != ErrorCode::kSuccess) {
    json["error"] = error_.ToJson();
  } else {
    json["result"] = result_;
  }
  // If there was an error in detecting the id in the Request object (e.g. Parse
  // error/Invalid Request), it MUST be Null.
  json["id"] = id_.ToJson();
  return json;
}

}  // namespace json_rpc