
#pragma once

#include <map>
#include <string>
#include <vector>

#include "json.h"

namespace json_rpc {

/// A Structured value that holds the parameter values to be used during the invocation of the
/// method. This member MAY be omitted.
///
/// If present, parameters for the rpc call MUST be provided as a Structured value. Either
/// by-position through an Array or by-name through an Object.
///
/// by - position : params MUST be an Array,
/// containing the values in the Server expected order.
///
/// by - name : params MUST be an Object,
/// with member names that match the Server expected parameter
/// names.The absence of expected names MAY result in an error being generated.The names MUST
/// match exactly, including case, to the method's expected parameters.
class Parameter {
 public:
  enum class ParamType : int { kNull, kArray, kMap };

  Parameter() = default;

  explicit Parameter(const Json& json);

  [[nodiscard]] Json ToJson() const;
  void ParseJson(const Json& json);

  [[nodiscard]] ParamType Type() const {
    return type_;
  }

  [[nodiscard]] const std::vector<Json>& Array() const {
    return array_;
  }

  [[nodiscard]] const std::map<std::string, Json>& Map() const {
    return map_;
  }

  // Before using the Get(key) function, please use has to determine if it exists, or directly use
  // Get(key, default_value)
  [[nodiscard]] Json Get(const std::string& key) const;

  // Before using the Get(idx) function, please use has to determine if it exists, or directly use
  // Get(idx, default_value)
  [[nodiscard]] Json Get(size_t idx) const;

  [[nodiscard]] bool Has(const std::string& key) const;

  [[nodiscard]] bool Has(size_t idx) const;

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
