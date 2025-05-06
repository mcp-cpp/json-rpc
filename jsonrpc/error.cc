// Copyright (c) The Feeds Team. All rights reserved.

#include "error.h"

namespace json_rpc {

Error::Error(int code, std::string message) : code_(code), message_(std::move(message)) {}

Error::Error(int code, std::string message, const Json& data)
    : code_(code), message_(std::move(message)), data_(data) {}

Json Error::ToJson() const {
  Json json;
  json["code"] = code_;
  json["message"] = message_;
  if (!data_.is_null()) {
    json["data"] = data_;
  }
  return json;
}

}  // namespace json_rpc
