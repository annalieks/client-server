#include <chrono>
#include <sstream>

#include "response.hpp"

HTTP::Response::Response(std::string_view code, std::string_view bodyStr)
    : code(code) {
  body = bodyStr;
}

std::string HTTP::Response::to_string() {
  std::string response;
  response += version + " " + code + " " + getMessage() + delim.data();
  setServerInfo();
  if (getHeader("Content-Type").empty()) {
    setHeader("Content-Type", "text/html");
    body = "<html><body><p>" + body + "</p></body></html>";
  }
  if (body.length() > 0) {
    setHeader("Content-Length", std::to_string(body.length()));
  }
  for (const auto& header : headers) {
    response += header.first + ": " + header.second + delim.data();
  }
  headers.empty() ? response += doubleDelim : response += delim;
  response += body;
  return response;
}

std::string HTTP::Response::getMessage() {
  try {
    switch (std::stoi(code)) {
      case 200:
        return "OK";
      case 404:
        return "Not Found";
      case 409:
        return "Conflict";
      case 500:
        return "Internal Server Error";
      case 501:
        return "Not Implemented";
      default:
        return "";
    }
  } catch (...) {
    return "";
  }
}

void HTTP::Response::setStatusLine(std::string_view line) {
  auto values = splitByDelim(std::string(line), " ");

  version = values[0];
  code = values[1];
}

void HTTP::Response::setServerInfo() {
  setHeader("Server", "client_server_app");
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::stringstream ss;
  ss << time;
  setHeader("Date", ss.str());
}
