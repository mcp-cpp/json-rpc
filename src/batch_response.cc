
#include "batch_response.h"

namespace json_rpc {

bool BatchResponse::AddResponse(const Response& response) {
  return responses_.emplace(response.Id(), response).second;
}

Json BatchResponse::ToJson() const {
  Json array = Json::array();
  for (const auto& response : responses_) {
    array.emplace_back(response.second.ToJson());
  }
  return array;
}

}  // namespace json_rpc