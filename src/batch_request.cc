

#include "batch_request.h"

#include "error.h"

namespace json_rpc {

Status BatchRequest::ParseJson(const std::string& json_str) {
  Json json;
  try {
    json = Json::parse(json_str);
  } catch (const nlohmann::detail::parse_error& e) {
    return {kParseError, "Parse error"};
  } catch (const std::exception& e) {
    return {kInvalidRequest, "Invalid request"};
  } catch (...) {
    return {kInvalidRequest, "Invalid request"};
  }
  return ParseJson(json);
}

Status BatchRequest::ParseJson(const Json& json) {
  if (json.is_array()) {
    for (const auto& item : json) {
      Request request;
      if (const auto status = request.ParseJson(item); status.Ok()) {
        return status;
      }
      requests_.emplace_back(request);
    }
  } else if (json.is_object()) {
    Request request;
    auto status = request.ParseJson(json);
    requests_.emplace_back(request);
  } else {
    return {kInvalidRequest, "Invalid request"};
  }
  return {kSuccess, ""};
}

}  // namespace json_rpc
