#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <map>
#include <string>
#include <vector>

namespace HTTP {

class Message {
 protected:
  std::string version = "HTTP/1";
  std::map<std::string, std::string> headers;
  std::string body;

  static std::vector<std::string> splitByDelim(std::string string,
                                               std::string_view delimiter);

 public:
  static constexpr std::string_view delim = "\r\n";
  static constexpr std::string_view doubleDelim = "\r\n\r\n";

  std::string getHeader(const std::string &key);
  const std::string &getBody() const;

  Message &setBody(std::string_view bodyStr);
  Message &setVersion(std::string_view v);
  Message &setHeader(const std::string &key, const std::string &value);

  void setHeaders(std::string_view headerStr);
};

}  // namespace HTTP
#endif  // MESSAGE_HPP
