#pragma once

#include <string>

#include "identifier.h"
#include "json.h"
#include "json_rpc_version.h"
#include "parameter.h"
#include "status.h"

namespace json_rpc {
/// Request object
/// A rpc call is represented by sending a Request object to a Server. The
/// Request object has the following members:
///
/// jsonrpc
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
  /// @brief Default constructor.
  Request() = default;

  /// @brief Constructor with specified JSON-RPC version, method, parameters, and identifier.
  /// @param jsonrpc_version The JSON-RPC version string.
  /// @param method The method name to be invoked.
  /// @param params The parameters for the method.
  /// @param id The identifier for the request.
  Request(std::string jsonrpc_version, std::string method, Parameter params, Identifier id);

  /// @brief Parses a JSON string into a Request object.
  /// @param json_str The JSON string to parse.
  /// @return A Status object indicating success or failure.
  Status ParseJson(const std::string& json_str);

  /// @brief Parses a JSON object into a Request object.
  /// @param json The JSON object to parse.
  /// @return A Status object indicating success or failure.
  Status ParseJson(const Json& json);

  /// @brief Gets the identifier of the request.
  /// @return The identifier object.
  [[nodiscard]] const Identifier& Id() const {
    return id_;
  }

  /// @brief Gets the method name of the request.
  /// @return The method name string.
  [[nodiscard]] const std::string& Method() const {
    return method_;
  }

  /// @brief Gets the parameters of the request.
  /// @return The parameters object.
  [[nodiscard]] const Parameter& Params() const {
    return params_;
  }

  /// @brief Gets the JSON-RPC version of the request.
  /// @return The JSON-RPC version string.
  [[nodiscard]] const std::string& JsonrpcVersion() const {
    return jsonrpc_version_;
  }

  /// @brief Converts the request to a JSON object.
  /// @return A JSON representation of the request.
  [[nodiscard]] Json ToJson() const;

  /// @brief Checks if the method is an internal method (starts with "rpc.").
  /// @return true if the method is internal, otherwise false.
  [[nodiscard]] bool IsInternalMethod() const {
    return method_.find("rpc.") != std::string::npos;
  }

  /// @brief Checks if the request is a notification (has no identifier).
  /// @return true if the request is a notification, otherwise false.
  [[nodiscard]] bool IsNotification() const {
    return id_.Type() == Identifier::IdType::kNull;
  }

 private:
  std::string jsonrpc_version_ = kJsonRpcVersion;
  std::string method_;
  Parameter params_;
  Identifier id_;
};

}  // namespace json_rpc
