// Copyright (c) The Feeds Team. All rights reserved.

#include "src/batch_request.h"

#include <string>

#include "gtest/gtest.h"

namespace json_rpc {

TEST(BatchRequestTest, ParseJsonFromString) {
  const std::string json_str = R"([
        {
            "json_rpc": "2.0",
            "method": "example_method1",
            "params": {"key1": "value1"},
            "id": 1
        },
        {
            "json_rpc": "2.0",
            "method": "example_method2",
            "params": {"key2": 42},
            "id": 2
        }
    ])";

  BatchRequest batch_request;
  EXPECT_TRUE(batch_request.ParseJson(json_str));

  const auto& requests = batch_request.Requests();
  EXPECT_EQ(requests.size(), 2);

  EXPECT_EQ(requests[0].Method(), "example_method1");
  EXPECT_EQ(requests[0].Params().ToJson(), Json({{"key1", "value1"}}));
  EXPECT_EQ(requests[0].Id().IntId(), 1);

  EXPECT_EQ(requests[1].Method(), "example_method2");
  EXPECT_EQ(requests[1].Params().ToJson(), Json({{"key2", 42}}));
  EXPECT_EQ(requests[1].Id().IntId(), 2);
}

TEST(BatchRequestTest, ParseJsonFromJson) {
  const Json json = {
      {{"json_rpc", "2.0"},
       {"method", "example_method1"},
       {"params", {{"key1", "value1"}}},
       {"id", 1}},
      {{"json_rpc", "2.0"}, {"method", "example_method2"}, {"params", {{"key2", 42}}}, {"id", 2}}};

  BatchRequest batch_request;
  EXPECT_TRUE(batch_request.ParseJson(json));

  const auto& requests = batch_request.Requests();
  EXPECT_EQ(requests.size(), 2);

  EXPECT_EQ(requests[0].Method(), "example_method1");
  EXPECT_EQ(requests[0].Params().ToJson(), Json({{"key1", "value1"}}));
  EXPECT_EQ(requests[0].Id().IntId(), 1);

  EXPECT_EQ(requests[1].Method(), "example_method2");
  EXPECT_EQ(requests[1].Params().ToJson(), Json({{"key2", 42}}));
  EXPECT_EQ(requests[1].Id().IntId(), 2);
}

TEST(BatchRequestTest, ParseJsonSingleStringId) {
  const std::string json_str = R"({
        "json_rpc": "2.0",
        "method": "example_method",
        "params": {"key1": "value1"},
        "id": "1"
    })";

  BatchRequest batch_request;
  EXPECT_TRUE(batch_request.ParseJson(json_str));

  const auto& requests = batch_request.Requests();
  EXPECT_EQ(requests.size(), 1);

  EXPECT_EQ(requests[0].Method(), "example_method");
  EXPECT_EQ(requests[0].Params().ToJson(), Json({{"key1", "value1"}}));
  EXPECT_EQ(requests[0].Id().StringId(), "1");
}

TEST(BatchRequestTest, ParseJsonInvalidArray) {
  const std::string json_str = R"([
        {
            "json_rpc": "2.0",
            "method": "example_method1",
            "params": {"key1": "value1"},
            "id": 1
        },
        {
            "json_rpc": "2.0",
            "method": "example_method2",
            "params": {"key2": 42},
            "id": 2
        },
        "invalid_item"
    ])";

  BatchRequest batch_request;
  EXPECT_FALSE(batch_request.ParseJson(json_str));
}

TEST(BatchRequestTest, ParseJsonEmptyArray) {
  const std::string json_str = R"([])";

  BatchRequest batch_request;
  EXPECT_TRUE(batch_request.ParseJson(json_str));

  const auto& requests = batch_request.Requests();
  EXPECT_EQ(requests.size(), 0);
}

TEST(BatchRequestTest, ParseJsonSingleObject) {
  const std::string json_str = R"({
        "json_rpc": "2.0",
        "method": "example_method",
        "params": {"key1": "value1"},
        "id": 1
    })";

  BatchRequest batch_request;
  EXPECT_TRUE(batch_request.ParseJson(json_str));

  const auto& requests = batch_request.Requests();
  EXPECT_EQ(requests.size(), 1);

  EXPECT_EQ(requests[0].Method(), "example_method");
  EXPECT_EQ(requests[0].Params().ToJson(), Json({{"key1", "value1"}}));
  EXPECT_EQ(requests[0].Id().IntId(), 1);
}

}  // namespace json_rpc