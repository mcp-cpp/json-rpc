
#include "json_rpc/response.h"

#include "gtest/gtest.h"

namespace json_rpc {

class ResponseTest : public ::testing::Test {};

TEST_F(ResponseTest, DefaultConstructor) {
  const Identifier id;
  const Response response(id);
  EXPECT_EQ(response.JsonrpcVersion(), kJsonRpcVersion);
  EXPECT_TRUE(response.Result().is_null());
  EXPECT_TRUE(response.Err().Code() == ErrorCode::kSuccess);
  EXPECT_TRUE(response.Id().Type() == Identifier::IdType::kNull);
}

TEST_F(ResponseTest, SetResult) {
  const Identifier id;
  Response response(id);
  const Json result = {{"key1", "value1"}, {"key2", 42}};
  response.SetResult(result);

  EXPECT_EQ(response.Result(), result);
}

TEST_F(ResponseTest, SetError) {
  const Identifier id;
  Response response(id);
  const Error error(ErrorCode::kInvalidRequest, "Invalid Request");
  response.SetError(error);

  EXPECT_EQ(response.Err().Code(), ErrorCode::kInvalidRequest);
  EXPECT_EQ(response.Err().Message(), "Invalid Request");
}

TEST_F(ResponseTest, ToJsonWithResult) {
  const Identifier id(1);
  Response response(id);
  Json result = {{"key1", "value1"}, {"key2", 42}};
  response.SetResult(result);

  const Json expected_json = {{"jsonrpc", "2.0"}, {"result", result}, {"id", id.ToJson()}};

  EXPECT_EQ(response.ToJson(), expected_json);
}

TEST_F(ResponseTest, ToJsonWithError) {
  Identifier id(1);
  Response response(id);
  Error error(ErrorCode::kInvalidRequest, "Invalid Request");
  response.SetError(error);

  Json expected_json = {{"jsonrpc", "2.0"}, {"error", error.ToJson()}, {"id", id.ToJson()}};

  EXPECT_EQ(response.ToJson(), expected_json);
}

TEST_F(ResponseTest, ToJsonWithNullId) {
  const Identifier id;
  Response response(id);
  Json result = {{"key1", "value1"}, {"key2", 42}};
  response.SetResult(result);

  const Json expected_json = {{"jsonrpc", "2.0"}, {"result", result}, {"id", nullptr}};

  EXPECT_EQ(response.ToJson(), expected_json);
}
}  // namespace json_rpc