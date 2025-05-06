// Copyright (c) The Feeds Team. All rights reserved.

#pragma once

#include <string>

#include "json.h"

namespace json_rpc {

enum ErrorCode {
  kSuccess = 0,

  kParseError = -32700,
  kInvalidRequest = -32600,
  kMethodNotFound = -32601,
  kInvalidParams = -32602,
  kInternalError = -32603,

  // -32000 to -32099	Server error	        Reserved for
  // implementation-defined server-errors.
};

/// Error object
/// When a rpc call encounters an error, the Response Object MUST contain the
/// error member with a value that is a Object with the following members:
///
/// code
/// A Number that indicates the error type that occurred.
/// This MUST be an integer.
///
/// message
/// A String providing a short description of the error.
/// The message SHOULD be limited to a concise single sentence.
///
/// data
/// A Primitive or Structured value that contains additional information about
/// the error. This may be omitted. The value of this member is defined by the
/// Server (e.g. detailed error information, nested errors etc.).
///
/// The error codes from and including -32768 to -32000 are reserved for
/// pre-defined errors. The following errors are predefined:
///
/// Code	        Message	                Meaning
/// -32700	        Parse error	        Invalid JSON was received by the
/// server.
///                                             An error occurred on the server
///                                             while parsing the JSON text.
///
/// -32600	        Invalid Request	        The JSON sent is not a valid
/// Request object.
///
/// -32601	        Method not found        The method does not exist / is
/// not available.
///
/// -32602	        Invalid params	        Invalid method parameter(s).
///
/// -32603	        Internal error	        Internal JSON-RPC error.
///
/// -32000 to -32099	Server error	        Reserved for
/// implementation-defined server-errors.
///
/// The error codes from -32768 to -32000 are reserved for
class Error {
 public:
  Error() = default;
  Error(int code, std::string message);
  Error(int code, std::string message, const Json& data);

  int Code() const {
    return code_;
  }

  const std::string& Message() const {
    return message_;
  }

  const Json& Data() const;

  Json ToJson() const;

 private:
  int code_ = kSuccess;
  std::string message_;
  Json data_;
};

}  // namespace json_rpc