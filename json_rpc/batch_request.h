
#pragma once

#include <vector>

#include "request.h"

namespace json_rpc {

// To send several Request objects at the same time, the Client MAY send an Array filled with
// Request objects.
class BatchRequest {
 public:
  BatchRequest() = default;

  Status ParseJson(const std::string& json_str);

  Status ParseJson(const Json& json);

  [[nodiscard]] const std::vector<std::pair<Request, Status>>& Requests() const {
    return requests_;
  }

 private:
  std::vector<std::pair<Request, Status>> requests_;
};

}  // namespace json_rpc
