
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

  /// @brief Default constructor.
  Parameter() = default;

  /// @brief Constructor with a JSON object.
  /// @param json The JSON object to initialize the parameter.
  explicit Parameter(const Json& json);

  /// @brief Converts the parameter to a JSON object.
  /// @return A JSON representation of the parameter.
  [[nodiscard]] Json ToJson() const;

  /// @brief Parses a JSON object into the parameter.
  /// @param json The JSON object to parse.
  void ParseJson(const Json& json);

  /// @brief Gets the type of the parameter.
  /// @return The parameter type (kNull, kArray, or kMap).
  [[nodiscard]] ParamType Type() const {
    return type_;
  }

  /// @brief Gets the array value of the parameter (if type is kArray).
  /// @return A constant reference to the array of JSON values.
  [[nodiscard]] const std::vector<Json>& Array() const {
    return array_;
  }

  /// @brief Gets the map value of the parameter (if type is kMap).
  /// @return A constant reference to the map of JSON values.
  [[nodiscard]] const std::map<std::string, Json>& Map() const {
    return map_;
  }

  /// @brief Gets a value from the parameter map by key.
  /// @param key The key to look up in the map.
  /// @return The JSON value associated with the key.
  /// @note Before using this function, ensure the key exists using `Has(key)` or use `Get(key, default_value)`.
  [[nodiscard]] Json Get(const std::string& key) const;

  /// @brief Gets a value from the parameter array by index.
  /// @param idx The index to look up in the array.
  /// @return The JSON value at the specified index.
  /// @note Before using this function, ensure the index exists using `Has(idx)` or use `Get(idx, default_value)`.
  [[nodiscard]] Json Get(size_t idx) const;

  /// @brief Checks if a key exists in the parameter map.
  /// @param key The key to check.
  /// @return true if the key exists, otherwise false.
  [[nodiscard]] bool Has(const std::string& key) const;

  /// @brief Checks if an index exists in the parameter array.
  /// @param idx The index to check.
  /// @return true if the index exists, otherwise false.
  [[nodiscard]] bool Has(size_t idx) const;

  /// @brief Gets a typed value from the parameter map by key.
  /// @tparam T The type to convert the JSON value to.
  /// @param key The key to look up in the map.
  /// @return The typed value associated with the key.
  template <typename T>
  T Get(const std::string& key) const {
    return Get(key).get<T>();
  }

  /// @brief Gets a typed value from the parameter array by index.
  /// @tparam T The type to convert the JSON value to.
  /// @param idx The index to look up in the array.
  /// @return The typed value at the specified index.
  template <typename T>
  T Get(size_t idx) const {
    return Get(idx).get<T>();
  }

  /// @brief Gets a typed value from the parameter map by key, with a default value if the key does not exist.
  /// @tparam T The type to convert the JSON value to.
  /// @param key The key to look up in the map.
  /// @param default_value The default value to return if the key does not exist.
  /// @return The typed value associated with the key, or the default value if the key does not exist.
  template <typename T>
  T Get(const std::string& key, const T& default_value) const {
    if (!Has(key)) {
      return default_value;
    }
    return Get<T>(key);
  }

  /// @brief Gets a typed value from the parameter array by index, with a default value if the index does not exist.
  /// @tparam T The type to convert the JSON value to.
  /// @param idx The index to look up in the array.
  /// @param default_value The default value to return if the index does not exist.
  /// @return The typed value at the specified index, or the default value if the index does not exist.
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
