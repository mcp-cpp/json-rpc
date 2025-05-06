// Copyright (c) The Feeds Team. All rights reserved.

#include "request.h"

#include <string>

#include "gtest/gtest.h"

#include "parameter.h"

namespace json_rpc {

class RequestTest : public ::testing::Test {};

TEST_F(RequestTest, Constructor) {
  std::string jsonrpc_version = kJsonRpcVersion;
  std::string method = "example_method";
  Json params = {{"key1", "value1"}, {"key2", 42}};
  Identifier id(1);

  Request req(jsonrpc_version, method, Parameter(params), id);

  EXPECT_EQ(req.JsonrpcVersion(), jsonrpc_version);
  EXPECT_EQ(req.Method(), method);
  EXPECT_EQ(req.Params().ToJson(), params);
  EXPECT_EQ(req.Id().Type(), Identifier::IdType::kNumber);
  EXPECT_EQ(req.Id().IntId(), 1);
}

TEST_F(RequestTest, ParseJsonFromString) {
  std::string json_str = R"({
        "jsonrpc": "2.0",
        "method": "example_method",
        "params": {"key1": "value1", "key2": 42},
        "id": 1
    })";

  Request req;
  EXPECT_TRUE(req.ParseJson(json_str));

  EXPECT_EQ(req.JsonrpcVersion(), "2.0");
  EXPECT_EQ(req.Method(), "example_method");
  EXPECT_EQ(req.Params().ToJson(), Json({{"key1", "value1"}, {"key2", 42}}));
  EXPECT_EQ(req.Id().Type(), Identifier::IdType::kNumber);
  EXPECT_EQ(req.Id().IntId(), 1);
}

TEST_F(RequestTest, ParseJsonFromJson) {
  Json json = {{"jsonrpc", "2.0"},
               {"method", "example_method"},
               {"params", {{"key1", "value1"}, {"key2", 42}}},
               {"id", 1}};

  Request req;
  EXPECT_TRUE(req.ParseJson(json));

  EXPECT_EQ(req.JsonrpcVersion(), "2.0");
  EXPECT_EQ(req.Method(), "example_method");
  EXPECT_EQ(req.Params().ToJson(), Json({{"key1", "value1"}, {"key2", 42}}));
  EXPECT_EQ(req.Id().Type(), Identifier::IdType::kNumber);
  EXPECT_EQ(req.Id().IntId(), 1);
}

TEST_F(RequestTest, ToJsonAndFromJson) {
  Request req(
      "2.0", "example_method", Parameter(Json({{"key1", "value1"}, {"key2", 42}})), Identifier(1));

  Json json;
  json_rpc::to_json(json, req);

  Request req_from_json;
  json_rpc::from_json(json, req_from_json);

  EXPECT_EQ(req_from_json.JsonrpcVersion(), req.JsonrpcVersion());
  EXPECT_EQ(req_from_json.Method(), req.Method());
  EXPECT_EQ(req_from_json.Params().ToJson(), req.Params().ToJson());
  EXPECT_EQ(req_from_json.Id().IntId(), req.Id().IntId());
}

TEST_F(RequestTest, IsInternalMethod) {
  Request req1("2.0", "rpc.example_method", Parameter(), Identifier(1));
  EXPECT_TRUE(req1.IsInternalMethod());

  Request req2("2.0", "example_method", Parameter(), Identifier(1));
  EXPECT_FALSE(req2.IsInternalMethod());
}

TEST_F(RequestTest, IsNotification) {
  Request req1("2.0", "example_method", Parameter(), Identifier(1));
  EXPECT_FALSE(req1.IsNotification());

  Request req2("2.0", "example_method", Parameter(), Identifier());
  EXPECT_TRUE(req2.IsNotification());
}

}  // namespace json_rpc