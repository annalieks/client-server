#include <sstream>

#include "message.hpp"

std::string HTTP::Message::getHeader(const std::string &key) {
  if (headers.find(key) != headers.end()) {
    return headers.at(key);
  }
  return "";
}

HTTP::Message &HTTP::Message::setHeader(const std::string &key,
                                        const std::string &value) {
  headers.insert({key, value});
  return *this;
}

const std::string &HTTP::Message::getBody() const { return body; }

void HTTP::Message::setHeaders(std::string_view headerStr) {
  auto values = splitByDelim(std::string(headerStr), delim);

  for (const auto &line : values) {
    std::string key, value;
    std::istringstream is(line);
    is >> key >> value;
    key.pop_back();
    setHeader(key, value);
  }
}

HTTP::Message &HTTP::Message::setBody(std::string_view bodyStr) {
  body = bodyStr;
  return *this;
}

HTTP::Message &HTTP::Message::setVersion(std::string_view v) {
  version = v;
  return *this;
}

std::vector<std::string> HTTP::Message::splitByDelim(
    std::string str, std::string_view delimiter) {
  std::string token;
  int pos;
  std::vector<std::string> result;
  while ((pos = str.find(delimiter)) != std::string::npos) {
    token = str.substr(0, pos);
    result.emplace_back(token);
    str.erase(0, pos + delimiter.length());
  }
  if (!str.empty()) {
    result.emplace_back(str);
  }
  return result;
}
