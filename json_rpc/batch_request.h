
#pragma once

#include <vector>

#include "request.h"

namespace json_rpc {

/// To send several Request objects at the same time, the Client MAY send an Array filled with
/// Request objects.
class BatchRequest {
 public:
  /// @brief Default constructor.
  BatchRequest() = default;

  /// @brief Parses a JSON string into a batch request.
  /// @param json_str The JSON string to parse.
  /// @return A Status object indicating success or failure.
  Status ParseJson(const std::string& json_str);

  /// @brief Parses a JSON object into a batch request.
  /// @param json The JSON object to parse.
  /// @return A Status object indicating success or failure.
  Status ParseJson(const Json& json);

  /// @brief Gets the list of requests and their parsing statuses.
  /// @return A constant reference to the vector of request-status pairs.
  [[nodiscard]] const std::vector<std::pair<Request, Status>>& Requests() const {
    return requests_;
  }

 private:
  std::vector<std::pair<Request, Status>> requests_;
};

}  // namespace json_rpc
