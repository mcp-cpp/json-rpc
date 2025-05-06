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

class BatchResponse {
 public:
  BatchResponse() = default;

  using ResponseMap = std::map<Identifier, Response, IdentifierComparer>;

  bool AddResponse(const Response& response);

  // Returns a JSON object with the responses.
  Json ToJson() const;

  const ResponseMap& Responses() const {
    return responses_;
  }

 private:
  ResponseMap responses_;
};

}  // namespace json_rpc
