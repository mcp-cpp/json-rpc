

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
    return {kInvalidRequest, "Invalid Request"};
  } catch (...) {
    return {kInvalidRequest, "Invalid Request"};
  }
  return ParseJson(json);
}

Status BatchRequest::ParseJson(const Json& json) {
  if (json.is_array()) {
    if (json.empty()) {
      return {kInvalidRequest, "Invalid Request"};
    }
    for (const auto& item : json) {
      Request request;
      const auto status = request.ParseJson(item);
      requests_.emplace_back(request, status);
    }
  } else if (json.is_object()) {
    Request request;
    auto status = request.ParseJson(json);
    requests_.emplace_back(request, status);
  } else {
    return {kInvalidRequest, "Invalid Request"};
  }
  return {kSuccess, ""};
}

}  // namespace json_rpc
