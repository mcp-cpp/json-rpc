// Copyright (c) The Feeds Team. All rights reserved.

#pragma once

#include "nlohmann/json.hpp"

namespace json_rpc {

using Json = nlohmann::json;

constexpr auto kJsonRpcVersionName = "jsonrpc";
constexpr auto kMethodName = "method";
constexpr auto kParamsName = "params";
constexpr auto kIdName = "id";
constexpr auto kResultName = "result";
constexpr auto kErrorName = "error";
constexpr auto kCodeName = "code";
constexpr auto kMessageName = "message";
constexpr auto kDataName = "data";

}  // namespace json_rpc