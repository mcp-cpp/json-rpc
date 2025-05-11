
#include "request.h"

#include "error.h"
#include "json_rpc_version.h"

namespace json_rpc {

Request::Request(std::string jsonrpc_version, std::string method, Parameter params, Identifier id)
    : jsonrpc_version_(std::move(jsonrpc_version)),
      method_(std::move(method)),
      params_(std::move(params)),
      id_(std::move(id)) {
}

Status Request::ParseJson(const std::string& json_str) {
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

Status Request::ParseJson(const Json& json) {
  try {
    from_json(json, *this);
  } catch (const nlohmann::detail::parse_error& e) {
    return {kParseError, "Parse error"};
  } catch (const std::exception& e) {
    return {kInvalidRequest, "Invalid Request"};
  } catch (...) {
    return {kInvalidRequest, "Invalid Request"};
  }
  return {kSuccess, ""};
}

// to_json()
void to_json(Json& j, const Request& req) {
  j = Json{{kJsonRpcVersionName, req.JsonrpcVersion()},
           {kMethodName, req.Method()},
           {kParamsName, req.Params().ToJson()}};

  if (!req.IsNotification()) {
    j[kIdName] = req.Id().ToJson();
  }
}

// from_json()
void from_json(const Json& j, Request& req) {
  // MUST be exactly "2.0".
  const auto jsonrpc_version = j.at(kJsonRpcVersionName).get<std::string>();
  if (jsonrpc_version != kJsonRpcVersion) {
    throw std::invalid_argument("invalid json_rpc version");
  }

  // MUST be a string.
  const auto method = j.at(kMethodName).get<std::string>();

  // MAY be omitted
  Parameter params;
  if (j.contains(kParamsName)) {
    auto p = j.at(kParamsName);
    // If present, parameters for the rpc call MUST be provided as a Structured
    // value. Either by-position through an Array or by-name through an Object
    if (!p.is_array() && !p.is_object()) {
      throw std::invalid_argument("invalid params");
    }
    params.ParseJson(p);
  }

  Identifier id;
  if (j.contains(kIdName)) {
    id.ParseJson(j.at(kIdName));
  }

  req = Request(jsonrpc_version, method, params, id);
}

}  // namespace json_rpc