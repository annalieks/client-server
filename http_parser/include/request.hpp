#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "message.hpp"

namespace HTTP {

enum Method { GET, POST, PUT, DELETE };

class Request : public Message {
 private:
  Method method{};
  std::string address = "/";
  std::map<std::string, std::string> params;

  void parseTarget(const std::string &t);

 public:
  Request() = default;

  const Method &getMethod() const;
  std::string getMethodStr() const;
  const std::string &getAddress() const;
  const std::map<std::string, std::string> &getParams() const;

  Request &setMethod(std::string_view m);
  Request &setTarget(const std::string &target);

  std::string to_string();

  void setStatusLine(std::string_view line);
};

}  // namespace HTTP

#endif  // REQUEST_HPP
