
#include "parameter.h"

namespace json_rpc {

Parameter::Parameter(const Json& json) {
  ParseJson(json);
}

Json Parameter::ToJson() const {
  if (type_ == ParamType::kArray) {
    return array_;
  }
  if (type_ == ParamType::kMap) {
    return map_;
  }
  return nullptr;
}

void Parameter::ParseJson(const Json& json) {
  if (json.is_array()) {
    type_ = ParamType::kArray;
    array_ = json.get<std::vector<Json>>();
  } else if (json.is_object()) {
    type_ = ParamType::kMap;
    map_ = json.get<std::map<std::string, Json>>();
  } else {
    type_ = ParamType::kNull;
  }
}

Json Parameter::Get(const std::string& key) const {
  return map_.at(key);
}

Json Parameter::Get(size_t idx) const {
  return array_.at(idx);
}

bool Parameter::Has(const std::string& key) const {
  if (type_ != ParamType::kMap) {
    return false;
  }
  return map_.find(key) != map_.end();
}

bool Parameter::Has(size_t idx) const {
  if (type_ != ParamType::kArray) {
    return false;
  }
  return idx < array_.size();
}

}  // namespace json_rpc