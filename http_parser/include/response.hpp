#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "message.hpp"

namespace HTTP {

class Response : public Message {
 private:
  std::string code;

  void setServerInfo();

 public:
  Response() = default;
  explicit Response(std::string_view code) : code(code) {}
  Response(std::string_view code, std::string_view bodyStr);

  std::string getMessage();
  std::string to_string();

  void setStatusLine(std::string_view line);
};

}  // namespace HTTP

#endif  // RESPONSE_HPP
