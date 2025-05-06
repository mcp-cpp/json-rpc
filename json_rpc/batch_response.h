// Copyright (c) The Feeds Team. All rights reserved.

#pragma once

#include <map>

#include "response.h"

namespace json_rpc {

struct IdentifierComparer {
  bool operator()(const Identifier& lhs, const Identifier& rhs) const {
    if (lhs.Type() == Identifier::IdType::kNumber && rhs.Type() == Identifier::IdType::kNumber) {
      return lhs.IntId() < rhs.IntId();
    }
    if (lhs.Type() == Identifier::IdType::kString && rhs.Type() == Identifier::IdType::kString) {
      return lhs.StringId() < rhs.StringId();
    }
    return lhs.Type() < rhs.Type();
  }
};

// The Server should respond with an Array containing the corresponding Response objects, after all
// of the batch Request objects have been processed. A Response object SHOULD exist for each Request
// object, except that there SHOULD NOT be any Response objects for notifications. The Server MAY
// process a batch rpc call as a set of concurrent tasks, processing them in any order and with any
// width of parallelism.

// The Response objects being returned from a batch call MAY be returned in any order within the
// Array. The Client SHOULD match contexts between the set of Request objects and the resulting set
// of Response objects based on the id member within each Object.

// If the batch rpc call itself fails to be recognized as an valid JSON or as an Array with at least
// one value, the response from the Server MUST be a single Response object. If there are no
// Response objects contained within the Response array as it is to be sent to the client, the
// server MUST NOT return an empty Array and should return nothing at all.
class BatchResponse {
 public:
  BatchResponse() = default;

  using ResponseMap = std::map<Identifier, Response, IdentifierComparer>;

  bool AddResponse(const Response& response);

  // Returns a JSON object with the responses.
  [[nodiscard]] Json ToJson() const;

  [[nodiscard]] const ResponseMap& Responses() const { return responses_; }

 private:
  ResponseMap responses_;
};

}  // namespace json_rpc
