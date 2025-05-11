

#include "src/parameter.h"

#include "gtest/gtest.h"

namespace json_rpc {

class ParameterTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // 初始化一些常用的 JSON 数据
    array_json_ = Json::array({1, 2, 3});
    map_json_ = Json::object({{"key1", 1}, {"key2", "value2"}});
  }

  Json array_json_;
  Json map_json_;
};

TEST_F(ParameterTest, TestDefaultConstructor) {
  const Parameter param;
  EXPECT_EQ(param.Type(), Parameter::ParamType::kNull);
  EXPECT_TRUE(param.ToJson().is_null());
}

TEST_F(ParameterTest, TestArrayConstructor) {
  const Parameter param(array_json_);
  EXPECT_EQ(param.Type(), Parameter::ParamType::kArray);
  EXPECT_EQ(param.ToJson(), array_json_);
  EXPECT_EQ(param.Get(0).get<int>(), 1);
  EXPECT_EQ(param.Get(1).get<int>(), 2);
  EXPECT_EQ(param.Get(2).get<int>(), 3);
  EXPECT_FALSE(param.Has(3));
}

TEST_F(ParameterTest, TestMapConstructor) {
  const Parameter param(map_json_);
  EXPECT_EQ(param.Type(), Parameter::ParamType::kMap);
  EXPECT_EQ(param.ToJson(), map_json_);
  EXPECT_EQ(param.Get("key1").get<int>(), 1);
  EXPECT_EQ(param.Get("key2").get<std::string>(), "value2");
  EXPECT_FALSE(param.Has("key3"));
}

TEST_F(ParameterTest, TestGetWithDefault) {
  const Parameter param(map_json_);
  EXPECT_EQ(param.Get("key1", 0), 1);
  EXPECT_EQ(param.Get<std::string>("key2", ""), "value2");
  EXPECT_EQ(param.Get("key3", 10), 10);
  EXPECT_EQ(param.Get<std::string>("key3", "default"), "default");
}

TEST_F(ParameterTest, TestParseJson) {
  Parameter param;
  param.ParseJson(array_json_);
  EXPECT_EQ(param.Type(), Parameter::ParamType::kArray);
  EXPECT_EQ(param.ToJson(), array_json_);

  param.ParseJson(map_json_);
  EXPECT_EQ(param.Type(), Parameter::ParamType::kMap);
  EXPECT_EQ(param.ToJson(), map_json_);
}

TEST_F(ParameterTest, TestInvalidJson) {
  Parameter param;
  const Json invalid_json = Json::object({{"key", Json::array({1, 2})}});
  param.ParseJson(invalid_json);
  EXPECT_EQ(param.Type(), Parameter::ParamType::kMap);
}

}  // namespace json_rpc