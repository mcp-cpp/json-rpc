
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
    response.SetError({kInvalidRequest, "Invalid Request"});
    return response;
  }
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
// --> {"jsonrpc": "2.0", "method": "subtract", "params": [42, 23], "id": 1}
// <-- {"jsonrpc": "2.0", "result": 19, "id": 1}

// --> {"jsonrpc": "2.0", "method": "subtract", "params": [23, 42], "id": 2}
// <-- {"jsonrpc": "2.0", "result": -19, "id": 2}
TEST(UnaryJsonRpc, PositionalParameters) {
  std::string req_json_str = R"({
        "jsonrpc": "2.0",
        "method": "subtract",
        "params": [42, 23],
        "id": 1
    })";

  Request request1;
  EXPECT_TRUE(request1.ParseJson(req_json_str).Ok());

  std::string rsp_json_str = R"({
        "jsonrpc": "2.0",
        "result": 19,
        "id": 1
    })";

  EXPECT_EQ(Service(request1).ToJson(), Json::parse(rsp_json_str));

  std::string req_json_str_2 = R"({
        "jsonrpc": "2.0",
        "method": "subtract",
        "params": [23, 42],
        "id": 2
    })";

  Request request2;
  EXPECT_TRUE(request2.ParseJson(req_json_str_2).Ok());

  std::string rsp_json_str_2 = R"({
        "jsonrpc": "2.0",
        "result": -19,
        "id": 2
    })";

  EXPECT_EQ(Service(request2).ToJson(), Json::parse(rsp_json_str_2));
}

// rpc call with named parameters:
// --> {"jsonrpc": "2.0", "method": "subtract", "params": {"subtrahend": 23, "minuend": 42}, "id":
// 3}
// <-- {"jsonrpc": "2.0", "result": 19, "id": 3}

// --> {"jsonrpc": "2.0", "method": "subtract", "params": {"minuend": 42, "subtrahend": 23}, "id":
// 4}
// <-- {"jsonrpc": "2.0", "result": 19, "id": 4}
TEST(UnaryJsonRpc, NamedParameters) {
  std::string req_json_str = R"({
        "jsonrpc": "2.0",
        "method": "subtract",
        "params": {"subtrahend": 23, "minuend": 42},
        "id": 3
    })";

  Request request1;
  EXPECT_TRUE(request1.ParseJson(req_json_str).Ok());

  std::string rsp_json_str = R"({
        "jsonrpc": "2.0",
        "result": 19,
        "id": 3
    })";

  EXPECT_EQ(Service(request1).ToJson(), Json::parse(rsp_json_str));

  std::string req_json_str_2 = R"({
        "jsonrpc": "2.0",
        "method": "subtract",
        "params": {"minuend": 42, "subtrahend": 23},
        "id": 4
    })";

  Request request2;
  EXPECT_TRUE(request2.ParseJson(req_json_str_2).Ok());

  std::string rsp_json_str_2 = R"({
        "jsonrpc": "2.0",
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
        "jsonrpc": "2.0",
        "method": "update",
        "params": [1,2,3,4,5]
    })";

  Request request1;
  EXPECT_TRUE(request1.ParseJson(req_json_str).Ok());
  EXPECT_TRUE(request1.IsNotification());

  std::string req_json_str_2 = R"({
        "jsonrpc": "2.0",
        "method": "foobar"
    })";

  Request request2;
  EXPECT_TRUE(request2.ParseJson(req_json_str_2).Ok());
  EXPECT_TRUE(request2.IsNotification());
}

// rpc call of non-existent method:
// --> {"jsonrpc": "2.0", "method": "foobar", "id": "1"}
// <-- {"jsonrpc": "2.0", "error": {"code": -32601, "message": "Method not found"}, "id": "1"}
TEST(UnaryJsonRpc, NonExistentMethod) {
  const std::string req_json_str = R"({
        "jsonrpc": "2.0",
        "method": "foobar",
        "id": "1"
    })";

  Request request;
  EXPECT_TRUE(request.ParseJson(req_json_str).Ok());

  std::string rsp_json_str = R"({
          "jsonrpc": "2.0",
          "error": {"code": -32601, "message": "Method not found"},
          "id": "1"})";
  EXPECT_EQ(Service(request).ToJson(), Json::parse(rsp_json_str));
}

// rpc call with invalid JSON:
// --> {"jsonrpc": "2.0", "method": "foobar, "params": "bar", "baz]
// <-- {"jsonrpc": "2.0", "error": {"code": -32700, "message": "Parse error"}, "id": null}
TEST(UnaryJsonRpc, InvalidJson) {
  const std::string req_json_str = R"({
        "jsonrpc": "2.0",
        "method": "foobar",
        "params": "bar", "baz]
    })";

  Request request;
  EXPECT_FALSE(request.ParseJson(req_json_str).Ok());
  Response response(request.Id());
  response.SetError({kParseError, "Parse error"});

  std::string rsp_json_str = R"({
          "jsonrpc": "2.0",
          "error": {"code": -32700, "message": "Parse error"},
          "id": null
    })";
  EXPECT_EQ(response.ToJson(), Json::parse(rsp_json_str));
}

// rpc call with invalid Request object:
// --> {"jsonrpc": "2.0", "method": 1, "params": "bar"}
// <-- {"jsonrpc": "2.0", "error": {"code": -32600, "message": "Invalid Request"}, "id": null}
TEST(UnaryJsonRpc, InvalidRequest) {
  const std::string req_json_str = R"({
        "jsonrpc": "2.0",
        "method": 1,
        "params": "bar"
    })";

  Request request;
  EXPECT_FALSE(request.ParseJson(req_json_str).Ok());
  Response response(request.Id());
  response.SetError({kInvalidRequest, "Invalid Request"});

  std::string rsp_json_str = R"({
          "jsonrpc": "2.0",
          "error": {"code": -32600, "message": "Invalid Request"},
          "id": null
    })";
  EXPECT_EQ(response.ToJson(), Json::parse(rsp_json_str));
}

// rpc call Batch, invalid JSON:
// --> [
// {"jsonrpc": "2.0", "method": "sum", "params": [1,2,4], "id": "1"},
// {"jsonrpc": "2.0", "method"
// ]
// <-- {"jsonrpc": "2.0", "error": {"code": -32700, "message": "Parse error"}, "id": null}
TEST(BatchJsonRpc, InvalidJson) {
  const std::string batch_req_json_str = R"([
    {"jsonrpc": "2.0", "method": "sum", "params": [1,2,4], "id": "1"},
    {"jsonrpc": "2.0", "method"
 ])";

  BatchRequest batch_request;
  auto status = batch_request.ParseJson(batch_req_json_str);
  EXPECT_FALSE(status.Ok());
  EXPECT_TRUE(batch_request.Requests().empty());
  Response response{Identifier()};
  response.SetError({status.Code(), status.Message()});

  std::string rsp_json_str = R"({
          "jsonrpc": "2.0",
          "error": {"code": -32700, "message": "Parse error"},
          "id": null
    })";
  EXPECT_EQ(response.ToJson(), Json::parse(rsp_json_str));
}

// rpc call with an empty Array:
// --> []
// <-- {"jsonrpc": "2.0", "error": {"code": -32600, "message": "Invalid Request"}, "id": null}
TEST(BatchJsonRpc, EmptyArray) {
  const std::string batch_req_json_str = R"([])";

  BatchRequest batch_request;
  auto status = batch_request.ParseJson(batch_req_json_str);
  EXPECT_FALSE(status.Ok());
  EXPECT_TRUE(batch_request.Requests().empty());
  Response response{Identifier()};
  response.SetError({status.Code(), status.Message()});

  std::string rsp_json_str = R"({
          "jsonrpc": "2.0",
          "error": {"code": -32600, "message": "Invalid Request"},
          "id": null
    })";
  EXPECT_EQ(response.ToJson(), Json::parse(rsp_json_str));
}

// rpc call with an invalid Batch (but not empty):
// --> [1]
// <-- [
// {"jsonrpc": "2.0", "error": {"code": -32600, "message": "Invalid Request"}, "id": null}
// ]
TEST(BatchJsonRpc, InvalidBatchNotEmptyArray) {
  const std::string batch_req_json_str = R"([1])";

  BatchRequest batch_request;
  EXPECT_TRUE(batch_request.ParseJson(batch_req_json_str).Ok());
  EXPECT_EQ(batch_request.Requests().size(), 1);

  const auto& status = batch_request.Requests().front().second;
  Response response{Identifier()};
  response.SetError({status.Code(), status.Message()});
  std::string rsp_json_str = R"({
          "jsonrpc": "2.0",
          "error": {"code": -32600, "message": "Invalid Request"},
          "id": null
    })";
  EXPECT_EQ(response.ToJson(), Json::parse(rsp_json_str));
}

// rpc call with invalid Batch:
// --> [1,2,3]
// <-- [
// {"jsonrpc": "2.0", "error": {"code": -32600, "message": "Invalid Request"}, "id": null},
// {"jsonrpc": "2.0", "error": {"code": -32600, "message": "Invalid Request"}, "id": null},
// {"jsonrpc": "2.0", "error": {"code": -32600, "message": "Invalid Request"}, "id": null}
// ]
TEST(BatchJsonRpc, InvalidBatch) {
  const std::string batch_req_json_str = R"([1,2,3])";

  BatchRequest batch_request;
  EXPECT_TRUE(batch_request.ParseJson(batch_req_json_str).Ok());
  BatchResponse batch_response;
  for (const auto& [request, status] : batch_request.Requests()) {
    Response response{Identifier()};
    response.SetError({status.Code(), status.Message()});
    batch_response.AddResponse(response);
  }
  EXPECT_EQ(batch_response.Responses().size(), 3);
  std::string rsp_json_str = R"([
         {
          "jsonrpc": "2.0",
          "error": {"code": -32600, "message": "Invalid Request"},
          "id": null
         },
         {
          "jsonrpc": "2.0",
          "error": {"code": -32600, "message": "Invalid Request"},
          "id": null
         },
         {
          "jsonrpc": "2.0",
          "error": {"code": -32600, "message": "Invalid Request"},
          "id": null
         }
  ])";
  EXPECT_EQ(batch_response.ToJson(), Json::parse(rsp_json_str));
}

}  // namespace json_rpc