
#include "response.h"

namespace json_rpc {

Json Response::ToJson() const {
  Json json;
  json[kJsonRpcVersionName] = jsonrpc_version_;
  if (error_.Code() != ErrorCode::kSuccess) {
    json[kErrorName] = error_.ToJson();
  } else {
    json[kResultName] = result_;
  }
  // If there was an error in detecting the id in the Request object (e.g. Parse
  // error/Invalid Request), it MUST be Null.
  json[kIdName] = id_.ToJson();
  return json;
}

}  // namespace json_rpc