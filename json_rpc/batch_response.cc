// Copyright (c) The Feeds Team. All rights reserved.

#include "batch_response.h"

namespace json_rpc {

bool BatchResponse::AddResponse(const Response& response) {
  return responses_.emplace(response.Id(), response).second;
}

Json BatchResponse::ToJson() const {
  Json json;
  for (const auto& response : responses_) {
    json.push_back(response.second.ToJson());
  }
  return json;
}

}  // namespace json_rpc