
#include "json_rpc/error.h"
#include "gtest/gtest.h"

namespace json_rpc {

TEST(ErrorTest, DefaultConstructor) {
  Error error;
  EXPECT_EQ(error.Code(), kSuccess);
  EXPECT_TRUE(error.Message().empty());
  EXPECT_TRUE(error.Data().is_null());
}

TEST(ErrorTest, ConstructorWithCodeAndMessage) {
  const int code = kInvalidRequest;
  const std::string message = "Invalid request";
  Error error(code, message);

  EXPECT_EQ(error.Code(), code);
  EXPECT_EQ(error.Message(), message);
  EXPECT_TRUE(error.Data().is_null());
}

TEST(ErrorTest, ConstructorWithCodeMessageAndData) {
  const int code = kInternalError;
  const std::string message = "Internal error";
  const Json data = {{"details", "Something went wrong"}};
  Error error(code, message, data);

  EXPECT_EQ(error.Code(), code);
  EXPECT_EQ(error.Message(), message);
  EXPECT_EQ(error.Data(), data);
}

TEST(ErrorTest, ToJsonWithCodeAndMessage) {
  const int code = kMethodNotFound;
  const std::string message = "Method not found";
  Error error(code, message);
  Json json = error.ToJson();

  EXPECT_EQ(json["code"], code);
  EXPECT_EQ(json["message"], message);
  EXPECT_TRUE(json["data"].is_null());
}

TEST(ErrorTest, ToJsonWithCodeMessageAndData) {
  const int code = kInvalidParams;
  const std::string message = "Invalid parameters";
  const Json data = {{"param", "id"}, {"reason", "must be integer"}};
  Error error(code, message, data);
  Json json = error.ToJson();

  EXPECT_EQ(json["code"], code);
  EXPECT_EQ(json["message"], message);
  EXPECT_EQ(json["data"], data);
}

}  // namespace json_rpc