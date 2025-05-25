
#pragma once

#include <vector>

#include "response.h"

namespace json_rpc {

/// The Server should respond with an Array containing the corresponding Response objects, after all
/// of the batch Request objects have been processed. A Response object SHOULD exist for each
/// Request object, except that there SHOULD NOT be any Response objects for notifications. The
/// Server MAY process a batch rpc call as a set of concurrent tasks, processing them in any order
/// and with any width of parallelism.
///
/// The Response objects being returned from a batch call MAY be returned in any order within the
/// Array. The Client SHOULD match contexts between the set of Request objects and the resulting set
/// of Response objects based on the id member within each Object.
///
/// If the batch rpc call itself fails to be recognized as an valid JSON or as an Array with at
/// least one value, the response from the Server MUST be a single Response object. If there are no
/// Response objects contained within the Response array as it is to be sent to the client, the
/// server MUST NOT return an empty Array and should return nothing at all.
class BatchResponse {
 public:
  /// @brief Default constructor.
  BatchResponse() = default;

  /// @brief Adds a response to the batch.
  /// @param response The response to add to the batch.
  void AddResponse(const Response& response);

  /// @brief Converts the batch response to a JSON object.
  /// @return A JSON representation of the batch response.
  [[nodiscard]] Json ToJson() const;

  /// @brief Gets the list of responses in the batch.
  /// @return A constant reference to the vector of responses.
  [[nodiscard]] const std::vector<Response>& Responses() const {
    return responses_;
  }

 private:
  std::vector<Response> responses_;
};

}  // namespace json_rpc
