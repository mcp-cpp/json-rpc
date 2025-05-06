// Copyright (c) The Feeds Team. All rights reserved.

#pragma once

#include <map>
#include <string>
#include <vector>

#include "json.h"

namespace json_rpc {

class Parameter {
 public:
  enum class ParamType : int { kNull, kArray, kMap };

  Parameter() = default;

  explicit Parameter(const Json& json);

  Json ToJson() const;
  void ParseJson(const Json& json);

  ParamType Type() const {
    return type_;
  }

  const std::vector<Json>& Array() const {
    return array_;
  }

  const std::map<std::string, Json>& Map() const {
    return map_;
  }

  // Before using the Get(key) function, please use has to determine if it exists, or directly use Get(key, default_value)
  Json Get(const std::string& key) const;

  // Before using the Get(idx) function, please use has to determine if it exists, or directly use Get(idx, default_value)
  Json Get(size_t idx) const;

  bool Has(const std::string& key) const;

  bool Has(size_t idx) const;

  template <typename T>
  T Get(const std::string& key) const {
    return Get(key).get<T>();
  }

  template <typename T>
  T Get(size_t idx) const {
    return Get(idx).get<T>();
  }

  template <typename T>
  T Get(const std::string& key, const T& default_value) const {
    if (!Has(key)) {
      return default_value;
    }
    return Get<T>(key);
  }

  template <typename T>
  T Get(size_t idx, const T& default_value) const {
    if (!Has(idx)) {
      return default_value;
    }
    return Get<T>(idx);
  }

 private:
  ParamType type_ = ParamType::kNull;
  std::vector<Json> array_;
  std::map<std::string, Json> map_;
};

}  // namespace json_rpc
