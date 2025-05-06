// Copyright (c) The Feeds Team. All rights reserved.

#include "identifier.h"

#include <utility>

namespace json_rpc {

Identifier::Identifier(int64_t id) : type_(IdType::kNumber), int_id_(id) {}
Identifier::Identifier(const char* id) : type_(IdType::kString), string_id_(id) {}
Identifier::Identifier(std::string id) : type_(IdType::kString), string_id_(std::move(id)) {}

Json Identifier::ToJson() const {
  if (type_ == IdType::kString) {
    return string_id_;
  }
  if (type_ == IdType::kNumber) {
    return int_id_;
  }
  return nullptr;
}

bool Identifier::ParseJson(const Json& json) {
  if (json.is_null()) {
    type_ = IdType::kNull;
    return true;
  }
  if (json.is_number_integer()) {
    type_ = IdType::kNumber;
    int_id_ = json.get<int64_t>();
    return true;
  }
  if (json.is_string()) {
    type_ = IdType::kString;
    string_id_ = json.get<std::string>();
    return true;
  }
  return false;
}

}  // namespace json_rpc