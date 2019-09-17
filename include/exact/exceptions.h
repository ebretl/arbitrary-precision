#pragma once

#include <exception>
#include <string>

namespace exact {

class OperationException : public std::exception {
 private:
  std::string msg;

 public:
  explicit OperationException(const std::string& explanation) {
    msg = "[OperationException] " + explanation;
  }

  const char* what() const noexcept override { return msg.c_str(); }
};

}  // namespace exact
