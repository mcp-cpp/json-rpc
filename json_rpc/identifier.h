
#pragma once

#include <cstdint>
#include <string>

#include "json.h"

namespace json_rpc {

// An identifier established by the Client that MUST contain a String, Number,
// or NULL value if included. If it is not included it is assumed to be a
// notification. The value SHOULD normally not be Null [1] and Numbers SHOULD
// NOT contain fractional parts [2]
class Identifier {
 public:
  enum class IdType : int { kNull, kNumber, kString };

  Identifier() = default;

  explicit Identifier(int64_t id);
  explicit Identifier(const char* id);
  explicit Identifier(std::string id);

  // ToJson() returns a json object that represents the identifier.
  [[nodiscard]] Json ToJson() const;

  // ParseJson() returns true if the json is a valid identifier.
  bool ParseJson(const Json& json);

  [[nodiscard]] const IdType& Type() const {
    return type_;
  }

  [[nodiscard]] int64_t IntId() const {
    return int_id_;
  }

  [[nodiscard]] const std::string& StringId() const {
    return string_id_;
  }

 protected:
  IdType type_ = IdType::kNull;
  int64_t int_id_ = 0;
  std::string string_id_;
};

}  // namespace json_rpc
