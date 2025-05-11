

#include "batch_request.h"

namespace json_rpc {

bool BatchRequest::ParseJson(const std::string& json_str) {
  Json json;
  try {
    json = Json::parse(json_str);
  } catch (const nlohmann::detail::parse_error& e) {
    return false;
  } catch (const std::exception& e) {
    return false;
  } catch (...) {
    return false;
  }
  return ParseJson(json);
}

bool BatchRequest::ParseJson(const Json& json) {
  if (json.is_array()) {
    for (const auto& item : json) {
      if (Request request; request.ParseJson(item)) {
        requests_.emplace_back(std::move(request));
      } else {
        return false;
      }
    }
  } else if (json.is_object()) {
    if (Request request; request.ParseJson(json)) {
      requests_.emplace_back(std::move(request));
    } else {
      return false;
    }
  } else {
    return false;
  }
  return true;
}

}  // namespace json_rpc
