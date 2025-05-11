#include <string>
#include <utility>

namespace json_rpc {

class Status final {
 public:
  Status(const int code, std::string msg) : code_(code), msg_(std::move(msg)) {
  }

  [[nodiscard]] int Code() const {
    return code_;
  }

  [[nodiscard]] const std::string& Message() const {
    return msg_;
  }

  [[nodiscard]] bool Ok() const {
    return code_ == 0;
  }

 private:
  int code_ = 0;
  std::string msg_;
};

}  // namespace json_rpc