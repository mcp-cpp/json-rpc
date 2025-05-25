#pragma once

#include <string>
#include <utility>

namespace json_rpc {

/**
 * @brief Status class, used to represent the state of an operation result.
 *
 * This class encapsulates the status code and status message.
 */
class Status final {
 public:
  /**
   * @brief Constructor
   * @param code Status code, 0 indicates success, non-zero indicates an error
   * @param msg Status message, describing the specific state
   */
  Status(const int code, std::string msg) : code_(code), msg_(std::move(msg)) {
  }

  /**
   * @brief Get the status code
   * @return Current status code
   */
  [[nodiscard]] int Code() const {
    return code_;
  }

  /**
   * @brief Get the status message
   * @return Constant reference to the current status message
   */
  [[nodiscard]] const std::string& Message() const {
    return msg_;
  }

  /**
   * @brief Check if the status is successful
   * @return true if the status code is 0, otherwise false
   */
  [[nodiscard]] bool Ok() const {
    return code_ == 0;
  }

 private:
  int code_ = 0;         ///< Status code, 0 indicates success, non-zero indicates an error
  std::string msg_;      ///< Status message, describing the specific state
};

}  // namespace json_rpc