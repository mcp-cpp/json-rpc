
#include "batch_response.h"

namespace json_rpc {

void BatchResponse::AddResponse(const Response& response) {
  responses_.emplace_back(response);
}

Json BatchResponse::ToJson() const {
  Json array = Json::array();
  for (const auto& response : responses_) {
    array.emplace_back(response.ToJson());
  }
  return array;
}

}  // namespace json_rpc