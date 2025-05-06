// Copyright (c) The Feeds Team. All rights reserved.

#include "gtest/gtest.h"

#include "src/json_rpc.h"

namespace json_rpc {

Response Subtract(const Request &request) {
  Response response(request.Id());
  if (request.Params().Array().size() != 2) {
    response.SetError({kInvalidParams, "Invalid params"});
    return response;
  }
  const int a = request.Params().Array()[0].get<int>();
  const int b = request.Params().Array()[1].get<int>();
  response.SetResult(a - b);
  return response;
}

Response Service(const Request &request) {
  if (request.JsonrpcVersion() != kJsonRpcVersion) {
    Response response(request.Id());
    response.SetError({kInvalidRequest, "Invalid request"});
    return response;
  }
  EXPECT_EQ(request.Method(), "subtract");
  if (request.Method() == "subtract") {
    return Subtract(request);
  }
  Response response(request.Id());
  response.SetError({kMethodNotFound, "Method not found"});
  return response;
}

// rpc call with positional parameters:
// --> {"json_rpc": "2.0", "method": "subtract", "params": [42, 23], "id": 1}
// <-- {"json_rpc": "2.0", "result": 19, "id": 1}

// --> {"json_rpc": "2.0", "method": "subtract", "params": [23, 42], "id": 2}
// <-- {"json_rpc": "2.0", "result": -19, "id": 2}
TEST(UnaryJsonRpc, PositionalParameters) {
  Request request;
  Response response;

  std::string req_json_str = R"({
        "json_rpc": "2.0",
        "method": "subtract",
        "params": [42, 23],
        "id": 1
    })";

  EXPECT_TRUE(request.ParseJson(req_json_str));

  std::string rsp_json_str = R"({
        "json_rpc": "2.0",
        "result": 19,
        "id": 1
    })";

  EXPECT_EQ(Service(request).ToJson(), Json::parse(rsp_json_str));

  std::string req_json_str_2 = R"({
        "json_rpc": "2.0",
        "method": "subtract",
        "params": [23, 42],
        "id": 2
    })";

  EXPECT_TRUE(request.ParseJson(req_json_str_2));

  std::string rsp_json_str_2 = R"({
        "json_rpc": "2.0",
        "result": -19,
        "id": 2
    })";

  EXPECT_EQ(Service(request).ToJson(), Json::parse(rsp_json_str_2));
}

// rpc call with named parameters:
// --> {"jsonrpc": "2.0", "method": "subtract", "params": {"subtrahend": 23, "minuend": 42}, "id": 3}
// <-- {"jsonrpc": "2.0", "result": 19, "id": 3}

// --> {"jsonrpc": "2.0", "method": "subtract", "params": {"minuend": 42, "subtrahend": 23}, "id": 4}
// <-- {"jsonrpc": "2.0", "result": 19, "id": 4}
TEST(UnaryJsonRpc, NamedParameters) {
  Request request;
  Response response;
}

} // namespace json_rpc