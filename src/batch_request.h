// Copyright (c) The Feeds Team. All rights reserved.

#pragma once

#include <vector>

#include "request.h"

namespace json_rpc {

// To send several Request objects at the same time, the Client MAY send an Array filled with
// Request objects.
class BatchRequest {
 public:
  BatchRequest() = default;

  bool ParseJson(const std::string& json_str);

  bool ParseJson(const Json& json);

  const std::vector<Request>& Requests() const { return requests_; }

 private:
  std::vector<Request> requests_;
};

}  // namespace json_rpc
