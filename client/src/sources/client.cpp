#include <iostream>
#include <json.hpp>
#include <memory>

#include "client.hpp"
#include "request.hpp"

Client::Client(const std::shared_ptr<boost::asio::io_context> &context)
    : io_context(context), resolver(*context) {}

void Client::start(const std::string &req) {
  auto endpoints = resolver.resolve(hostAddress, port);
  connection = std::make_shared<Connection>(io_context);
  connection->startConnection(endpoints, req);
}

void Client::addPair() {
  std::string key, value;
  std::cin >> key >> value;
  nlohmann::json j;
  j[key] = value;
  std::string requestBody = j.dump(4);
  start(composeRequest("POST", "/", requestBody));
}

void Client::getValue() {
  std::string key;
  std::cin >> key;
  start(composeRequest("GET", "/?key=" + key));
}

void Client::setPair() {
  std::string key, value;
  std::cin >> key >> value;
  nlohmann::json j;
  j[key] = value;
  std::string requestBody = j.dump(4);
  start(composeRequest("PUT", "/", requestBody));
}

void Client::deletePair() {
  std::string key;
  std::cin >> key;
  start(composeRequest("DELETE", "/?key=" + key));
}

std::string Client::composeRequest(const std::string &method,
                                   const std::string &target,
                                   const std::string &body) {
  HTTP::Request request;
  request.setMethod(method).setHeader("Host", hostAddress + ":" + port);
  request.setTarget(target);
  if (method != "GET") {
    request.setHeader("Content-Length", std::to_string(body.size()))
        .setHeader("Content-Type", "application/json")
        .setBody(body);
  }
  return request.to_string();
}
