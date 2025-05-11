// Copyright (c) The Feeds Team. All rights reserved.

#include "gtest/gtest.h"

#include "src/json_rpc.h"

namespace json_rpc {

Response SubtractWithArrayParam(const Request& request) {
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

Response SubtractWithMapParam(const Request& request) {
  Response response(request.Id());
  if (request.Params().Map().size() != 2) {
    response.SetError({kInvalidParams, "Invalid params"});
    return response;
  }
  const int a = request.Params().Map().at("minuend").get<int>();
  const int b = request.Params().Map().at("subtrahend").get<int>();
  response.SetResult(a - b);
  return response;
}

Response Service(const Request& request) {
  if (request.JsonrpcVersion() != kJsonRpcVersion) {
    Response response(request.Id());
    response.SetError({kInvalidRequest, "Invalid request"});
    return response;
  }
  EXPECT_EQ(request.Method(), "subtract");
  if (request.Method() == "subtract") {
    if (request.Params().Array().empty()) {
      return SubtractWithMapParam(request);
    }
    return SubtractWithArrayParam(request);
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
  std::string req_json_str = R"({
        "json_rpc": "2.0",
        "method": "subtract",
        "params": [42, 23],
        "id": 1
    })";

  Request request1;
  EXPECT_TRUE(request1.ParseJson(req_json_str));

  std::string rsp_json_str = R"({
        "json_rpc": "2.0",
        "result": 19,
        "id": 1
    })";

  EXPECT_EQ(Service(request1).ToJson(), Json::parse(rsp_json_str));

  std::string req_json_str_2 = R"({
        "json_rpc": "2.0",
        "method": "subtract",
        "params": [23, 42],
        "id": 2
    })";

  Request request2;
  EXPECT_TRUE(request2.ParseJson(req_json_str_2));

  std::string rsp_json_str_2 = R"({
        "json_rpc": "2.0",
        "result": -19,
        "id": 2
    })";

  EXPECT_EQ(Service(request2).ToJson(), Json::parse(rsp_json_str_2));
}

// rpc call with named parameters:
// --> {"jsonrpc": "2.0", "method": "subtract", "params": {"subtrahend": 23, "minuend": 42}, "id": 3}
// <-- {"jsonrpc": "2.0", "result": 19, "id": 3}

// --> {"jsonrpc": "2.0", "method": "subtract", "params": {"minuend": 42, "subtrahend": 23}, "id": 4}
// <-- {"jsonrpc": "2.0", "result": 19, "id": 4}
TEST(UnaryJsonRpc, NamedParameters) {
  std::string req_json_str = R"({
        "json_rpc": "2.0",
        "method": "subtract",
        "params": {"subtrahend": 23, "minuend": 42},
        "id": 3
    })";

  Request request1;
  EXPECT_TRUE(request1.ParseJson(req_json_str));

  std::string rsp_json_str = R"({
        "json_rpc": "2.0",
        "result": 19,
        "id": 3
    })";

  EXPECT_EQ(Service(request1).ToJson(), Json::parse(rsp_json_str));

  std::string req_json_str_2 = R"({
        "json_rpc": "2.0",
        "method": "subtract",
        "params": {"minuend": 42, "subtrahend": 23},
        "id": 4
    })";

  Request request2;
  EXPECT_TRUE(request2.ParseJson(req_json_str_2));

  std::string rsp_json_str_2 = R"({
        "json_rpc": "2.0",
        "result": 19,
        "id": 4
    })";

  EXPECT_EQ(Service(request2).ToJson(), Json::parse(rsp_json_str_2));
}

// a Notification:
// --> {"jsonrpc": "2.0", "method": "update", "params": [1,2,3,4,5]}
// --> {"jsonrpc": "2.0", "method": "foobar"}
TEST(UnaryJsonRpc, Notification) {
  std::string req_json_str = R"({
        "json_rpc": "2.0",
        "method": "update",
        "params": [1,2,3,4,5],
    })";

  Request request1;
  EXPECT_TRUE(request1.ParseJson(req_json_str));
  EXPECT_TRUE(request1.IsNotification());

  std::string req_json_str_2 = R"({
        "json_rpc": "2.0",
        "method": "foobar",
    })";

  Request request2;
  EXPECT_TRUE(request2.ParseJson(req_json_str_2));
  EXPECT_TRUE(request2.IsNotification());
}

}  // namespace json_rpc