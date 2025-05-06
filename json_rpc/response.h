// Copyright (c) The Feeds Team. All rights reserved.

#pragma once

#include <string>

#include "error.h"
#include "identifier.h"
#include "json.h"
#include "json_rpc_version.h"

namespace json_rpc {

/// Response object
/// When a rpc call is made, the Server MUST reply with a Response, except for
/// in the case of Notifications. The Response is expressed as a single JSON
/// Object, with the following members:
///
/// json_rpc
/// A String specifying the version of the JSON-RPC protocol. MUST be exactly
/// "2.0".
///
/// result
/// This member is REQUIRED on success.
/// This member MUST NOT exist if there was an error invoking the method.
/// The value of this member is determined by the method invoked on the Server.
///
/// error
/// This member is REQUIRED on error.
/// This member MUST NOT exist if there was no error triggered during
/// invocation. The value for this member MUST be an Error Object
///
/// id
/// This member is REQUIRED.
/// It MUST be the same as the value of the id member in the Request Object.
/// If there was an error in detecting the id in the Request object (e.g. Parse
/// error/Invalid Request), it MUST be Null.
///
/// Either the result member or error member MUST be included, but both members
/// MUST NOT be included.
class Response {
 public:
  Response() = default;
  Response(Identifier id) : id_(std::move(id)) {};

  Json ToJson() const;

  const std::string& JsonrpcVersion() const {
    return jsonrpc_version_;
  }

  const Json& Result() const {
    return result_;
  }

  void SetError(Error error) {
    error_ = std::move(error);
  }

  void SetResult(Json result) {
    result_ = std::move(result);
  }

  const Identifier& Id() const {
    return id_;
  }

  void SetId(Identifier id) {
    id_ = std::move(id);
  }

  const Error& Err() const {
    return error_;
  }

 private:
  std::string jsonrpc_version_ = kJsonRpcVersion;
  Json result_;
  Error error_;
  Identifier id_;
};

}  // namespace json_rpc
