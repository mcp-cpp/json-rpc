
#pragma once

#include <cstdint>
#include <string>

#include "json.h"

namespace json_rpc {

/// An identifier established by the Client that MUST contain a String, Number,
/// or NULL value if included. If it is not included it is assumed to be a
/// notification. The value SHOULD normally not be Null [1] and Numbers SHOULD
/// NOT contain fractional parts [2]
class Identifier {
 public:
  enum class IdType : int { kNull, kNumber, kString };

  /// @brief Default constructor.
  Identifier() = default;

  /// @brief Constructor with an integer identifier.
  /// @param id The integer identifier.
  explicit Identifier(int64_t id);

  /// @brief Constructor with a C-style string identifier.
  /// @param id The C-style string identifier.
  explicit Identifier(const char* id);

  /// @brief Constructor with a string identifier.
  /// @param id The string identifier.
  explicit Identifier(std::string id);

  /// @brief Converts the identifier to a JSON object.
  /// @return A JSON representation of the identifier.
  [[nodiscard]] Json ToJson() const;

  /// @brief Parses a JSON object into the identifier.
  /// @param json The JSON object to parse.
  /// @return true if the JSON is a valid identifier, otherwise false.
  bool ParseJson(const Json& json);

  /// @brief Gets the type of the identifier.
  /// @return The identifier type (kNull, kNumber, or kString).
  [[nodiscard]] const IdType& Type() const {
    return type_;
  }

  /// @brief Gets the integer value of the identifier (if type is kNumber).
  /// @return The integer identifier.
  [[nodiscard]] int64_t IntId() const {
    return int_id_;
  }

  /// @brief Gets the string value of the identifier (if type is kString).
  /// @return The string identifier.
  [[nodiscard]] const std::string& StringId() const {
    return string_id_;
  }

 protected:
  IdType type_ = IdType::kNull;
  int64_t int_id_ = 0;
  std::string string_id_;
};

}  // namespace json_rpc
