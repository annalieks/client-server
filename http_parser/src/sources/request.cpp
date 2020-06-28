#include <sstream>

#include "request.hpp"

const HTTP::Method& HTTP::Request::getMethod() const { return method; }

HTTP::Request& HTTP::Request::setMethod(std::string_view m) {
  if (m == "GET") {
    method = HTTP::GET;
  } else if (m == "PUT") {
    method = HTTP::PUT;
  } else if (m == "POST") {
    method = HTTP::POST;
  } else if (m == "DELETE") {
    method = HTTP::DELETE;
  }
  return *this;
}

std::string HTTP::Request::getMethodStr() const {
  switch (method) {
    case GET:
      return "GET";
    case PUT:
      return "PUT";
    case POST:
      return "POST";
    case DELETE:
      return "DELETE";
    default:
      return "";
  }
}

const std::map<std::string, std::string>& HTTP::Request::getParams() const {
  return params;
}

void HTTP::Request::setStatusLine(std::string_view line) {
  auto values = splitByDelim(std::string(line), " ");

  setMethod(values[0]);
  parseTarget(values[1]);
  version = values[2];
}

HTTP::Request& HTTP::Request::setTarget(const std::string& target) {
  parseTarget(target);
  return *this;
}

std::string HTTP::Request::to_string() {
  std::string request;
  request = getMethodStr() + " " + address;
  if (!params.empty()) {
    request += "?";
  }
  for (const auto& param : params) {
    request += param.first + "=" + param.second;
  }
  request += " " + version + std::string(delim);
  for (const auto& header : headers) {
    request += header.first + ": " + header.second + std::string(delim);
  }
  request += std::string(delim) + body;
  return request;
}

void HTTP::Request::parseTarget(const std::string& target) {
  int paramPos = target.find('?');
  if (paramPos == std::string::npos) {
    address = target;
    return;
  }
  std::string paramStr = target.substr(paramPos + 1);

  address = target.substr(0, paramPos);

  auto paramVec = HTTP::Request::splitByDelim(paramStr, "&");
  for (const auto& item : paramVec) {
    auto pair = HTTP::Request::splitByDelim(item, "=");
    params.insert({pair[0], pair[1]});
  }
}

const std::string& HTTP::Request::getAddress() const { return address; }
