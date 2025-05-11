// Copyright (c) The Feeds Team. All rights reserved.

#pragma once

#include <string>

#include "identifier.h"
#include "json.h"
#include "json_rpc_version.h"
#include "parameter.h"

namespace json_rpc {

constexpr auto kJsonRpcVersionName = "json_rpc";
constexpr auto kMethodName = "method";
constexpr auto kParamsName = "params";
constexpr auto kIdName = "id";

/// Request object
/// A rpc call is represented by sending a Request object to a Server. The
/// Request object has the following members:
///
/// json_rpc
/// A String specifying the version of the JSON-RPC protocol. MUST be exactly
/// "2.0".
///
/// method
/// A String containing the name of the method to be invoked. Method names that
/// begin with the word rpc followed by a period character (U+002E or ASCII 46)
/// are reserved for rpc-internal methods and extensions and MUST NOT be used
/// for anything else.
///
/// params
/// A Structured value that holds the parameter values to be used during the
/// invocation of the method. This member MAY be omitted.
///
/// id
/// An identifier established by the Client that MUST contain a String, Number,
/// or NULL value if included. If it is not included it is assumed to be a
/// notification. The value SHOULD normally not be Null and Numbers SHOULD NOT
/// contain fractional parts
///
/// The Server MUST reply with the same value in the Response object if
/// included. This member is used to correlate the context between the two
/// objects. [1] The use of Null as a value for the id member in a Request
/// object is discouraged, because this specification uses a value of Null for
/// Responses with an unknown id. Also, because JSON-RPC 1.0 uses an id value of
/// Null for Notifications this could cause confusion in handling. [2]
/// Fractional parts may be problematic,
///    since many decimal fractions cannot be represented exactly as binary
///    fractions.
///
/// Notification
/// A Notification is a Request object without an "id" member. A Request object
/// that is a Notification signifies the Client's lack of interest in the
/// corresponding Response object, and as such no Response object needs to be
/// returned to the client. The Server MUST NOT reply to a Notification,
/// including those that are within a batch request. Notifications are not
/// confirmable by definition, since they do not have a Response object to be
/// returned. As such, the Client would not be aware of any errors (like e.g.
/// "Invalid params","Internal error").
///
/// Parameter Structures
/// If present, parameters for the rpc call MUST be provided as a Structured
/// value. Either by-position through an Array or by-name through an Object.
/// by-position: params MUST be an Array, containing the values in the Server
/// expected order. by-name: params MUST be an Object, with member names that
/// match the Server expected parameter names. The absence of expected names MAY
/// result in an error being generated. The names MUST match exactly, including
/// case, to the method's expected parameters.

class Request {
 public:
  Request() = default;

  Request(std::string jsonrpc_version, std::string method, Parameter params, Identifier id);

  bool ParseJson(const std::string& json_str);

  bool ParseJson(const Json& json);

  [[nodiscard]] const Identifier& Id() const {
    return id_;
  }

  [[nodiscard]] const std::string& Method() const {
    return method_;
  }

  [[nodiscard]] const Parameter& Params() const {
    return params_;
  }

  [[nodiscard]] const std::string& JsonrpcVersion() const {
    return jsonrpc_version_;
  }

  // IsInternalMethod() returns true if the method is an internal method
  [[nodiscard]] bool IsInternalMethod() const {
    return method_.find("rpc.") != std::string::npos;
  }

  // IsNotification() returns true if the request is a notification
  [[nodiscard]] bool IsNotification() const {
    return id_.Type() == Identifier::IdType::kNull;
  }

 private:
  std::string jsonrpc_version_ = kJsonRpcVersion;
  std::string method_;
  Parameter params_;
  Identifier id_;
};

// to_json() request convert to json
void to_json(Json& j, const Request& req);

// from_json() request convert from json
void from_json(const Json& j, Request& req);

}  // namespace json_rpc
