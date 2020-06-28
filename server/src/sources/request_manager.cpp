#include <iostream>
#include <json.hpp>

#include "request_manager.hpp"

HTTP::Response RequestManager::executeRequest() {
    if(request.getAddress() != "/") {
        return  handleUnimplemented();
    }
  if (request.getParams().empty()) {
      if (request.getHeader("Content-Type") != "application/json") {
          return handleUnimplemented();
      }
    switch (request.getMethod()) {
      case HTTP::POST:
        return handlePostRequest();
      case HTTP::PUT:
        return handlePutRequest();
      default:
        return handleUnimplemented();
    }
  }
  switch (request.getMethod()) {
    case HTTP::GET:
      return handleGetRequest();
    case HTTP::DELETE:
      return handleDeleteRequest();
    default:
      return handleUnimplemented();
  }
}

HTTP::Response RequestManager::handlePostRequest() {
  nlohmann::json j = nlohmann::json::parse(request.getBody());
  auto pair = j.items().begin();
  return file->addRecord(pair.key(), pair.value());
}

HTTP::Response RequestManager::handlePutRequest() {
  nlohmann::json j = nlohmann::json::parse(request.getBody());
  auto pair = j.items().begin();
  return file->setRecord(pair.key(), pair.value());
}

HTTP::Response RequestManager::handleGetRequest() {
  if (request.getParams().find("key") != request.getParams().end() &&
      request.getParams().size() == 1) {
    return file->getRecord(request.getParams().at("key"));
  }
  return handleUnimplemented();
}

HTTP::Response RequestManager::handleDeleteRequest() {
  if (request.getParams().find("key") != request.getParams().end() &&
      request.getParams().size() == 1) {
    return file->deleteRecord(request.getParams().at("key"));
  }
  return handleUnimplemented();
}

HTTP::Response RequestManager::handleUnimplemented() {
  return HTTP::Response("501");
}
