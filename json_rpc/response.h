
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
  /// @brief Default constructor.
  Response() = default;

  /// @brief Constructor with an identifier.
  /// @param id The identifier for the response.
  explicit Response(Identifier id) : id_(std::move(id)) {};

  /// @brief Converts the response to a JSON object.
  /// @return A JSON representation of the response.
  [[nodiscard]] Json ToJson() const;

  /// @brief Gets the JSON-RPC version.
  /// @return The JSON-RPC version string.
  [[nodiscard]] const std::string& JsonrpcVersion() const {
    return jsonrpc_version_;
  }

  /// @brief Gets the result of the response.
  /// @return The result as a JSON object.
  [[nodiscard]] const Json& Result() const {
    return result_;
  }

  /// @brief Sets an error for the response.
  /// @param error The error object to set.
  void SetError(Error error) {
    error_ = std::move(error);
  }

  /// @brief Sets the result for the response.
  /// @param result The result as a JSON object.
  void SetResult(Json result) {
    result_ = std::move(result);
  }

  /// @brief Gets the identifier of the response.
  /// @return The identifier object.
  [[nodiscard]] const Identifier& Id() const {
    return id_;
  }

  /// @brief Sets the identifier for the response.
  /// @param id The identifier object to set.
  void SetId(Identifier id) {
    id_ = std::move(id);
  }

  /// @brief Gets the error of the response.
  /// @return The error object.
  [[nodiscard]] const Error& Err() const {
    return error_;
  }

 private:
  std::string jsonrpc_version_ = kJsonRpcVersion;
  Json result_;
  Error error_;
  Identifier id_;
};

}  // namespace json_rpc
