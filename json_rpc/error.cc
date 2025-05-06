// Copyright (c) The Feeds Team. All rights reserved.

#include "error.h"

#include <utility>

namespace json_rpc {

Error::Error(const int code, std::string message) : code_(code), message_(std::move(message)) {}

Error::Error(const int code, std::string message, Json data)
    : code_(code), message_(std::move(message)), data_(std::move(data)) {}

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
