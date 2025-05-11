
#include "src/batch_response.h"

#include "gtest/gtest.h"

namespace json_rpc {

class BatchResponseTest : public ::testing::Test {};

TEST_F(BatchResponseTest, AddResponse) {
  BatchResponse batch_response;
  Response response1;
  response1.SetResult(Json({{"key1", "value1"}}));
  response1.SetError(Error(ErrorCode::kSuccess, ""));
  response1.SetId(Identifier(1));

  Response response2;
  response2.SetResult(Json({{"key2", 42}}));
  response2.SetError(Error(ErrorCode::kSuccess, ""));
  response2.SetId(Identifier(2));

  EXPECT_TRUE(batch_response.AddResponse(response1));
  EXPECT_TRUE(batch_response.AddResponse(response2));

  EXPECT_EQ(batch_response.Responses().size(), 2);
}

TEST_F(BatchResponseTest, AddResponseDuplicateId) {
  BatchResponse batch_response;
  Response response1;
  response1.SetResult(Json({{"key1", "value1"}}));
  response1.SetError(Error(ErrorCode::kSuccess, ""));
  response1.SetId(Identifier(1));

  Response response2;
  response2.SetResult(Json({{"key2", 42}}));
  response2.SetError(Error(ErrorCode::kSuccess, ""));
  response2.SetId(Identifier(1));

  EXPECT_TRUE(batch_response.AddResponse(response1));
  EXPECT_FALSE(batch_response.AddResponse(response2));

  EXPECT_EQ(batch_response.Responses().size(), 1);
}

TEST_F(BatchResponseTest, ToJson) {
  BatchResponse batch_response;
  Response response1;
  response1.SetResult(Json({{"key1", "value1"}}));
  response1.SetError(Error(ErrorCode::kSuccess, ""));
  response1.SetId(Identifier(1));

  Response response2;
  response2.SetResult(Json({{"key2", 42}}));
  response2.SetError(Error(ErrorCode::kSuccess, ""));
  response2.SetId(Identifier(2));

  batch_response.AddResponse(response1);
  batch_response.AddResponse(response2);

  Json expected_json = {{{"jsonrpc", "2.0"}, {"result", {{"key1", "value1"}}}, {"id", 1}},
                        {{"jsonrpc", "2.0"}, {"result", {{"key2", 42}}}, {"id", 2}}};

  EXPECT_EQ(batch_response.ToJson(), expected_json);
}

TEST_F(BatchResponseTest, ToJsonEmpty) {
  BatchResponse batch_response;
  Json expected_json;
  EXPECT_EQ(batch_response.ToJson(), expected_json);
}

TEST_F(BatchResponseTest, ToJsonSingleResponse) {
  BatchResponse batch_response;
  Response response;
  response.SetResult(Json({{"key1", "value1"}}));
  response.SetError(Error(ErrorCode::kSuccess, ""));
  response.SetId(Identifier(1));

  batch_response.AddResponse(response);

  const Json expected_json = {{{"jsonrpc", "2.0"}, {"result", {{"key1", "value1"}}}, {"id", 1}}};

  EXPECT_EQ(batch_response.ToJson(), expected_json);
}

}  // namespace json_rpc