#include <iostream>

#include "connection.hpp"

Connection::Connection(boost::asio::io_context& io_context,
                       const std::shared_ptr<DataFile>& jsonFile)
    : clientSocket(io_context), requestManager(jsonFile) {}

boost::asio::ip::tcp::socket& Connection::get_socket() { return clientSocket; }

void Connection::readFromClient() {
  boost::asio::async_read_until(
      clientSocket, buffer, HTTP::Message::delim,
      std::bind(&Connection::handleStatusLineRead, shared_from_this(),
                std::placeholders::_1, std::placeholders::_2));
}

void Connection::handleStatusLineRead(const std::error_code& error,
                                      std::size_t bytes_transferred) {
  if (error) {
    std::cerr << error.message() << std::endl;
    return;
  }
  requestManager.request.setStatusLine(
      getStringFromBuff(bytes_transferred, HTTP::Message::delim));

  buffer.consume(bytes_transferred);

  boost::asio::async_read_until(
      clientSocket, buffer, HTTP::Message::doubleDelim,
      std::bind(&Connection::handleHeadersRead, shared_from_this(),
                std::placeholders::_1, std::placeholders::_2));
}

void Connection::handleHeadersRead(const std::error_code& error,
                                   std::size_t bytes_transferred) {
  if (error) {
    std::cerr << error.message() << std::endl;
    return;
  }
  requestManager.request.setHeaders(
      getStringFromBuff(bytes_transferred, HTTP::Message::doubleDelim));
  buffer.consume(bytes_transferred);

  if (requestManager.request.getMethod() != HTTP::GET) {
    size_t contentLength =
        std::stoi(requestManager.request.getHeader("Content-Length"));
    size_t transferBytes =
        contentLength - std::min(contentLength, buffer.size());
    boost::asio::async_read(
        clientSocket, buffer, boost::asio::transfer_exactly(transferBytes),
        std::bind(&Connection::handleBodyRead, shared_from_this(),
                  std::placeholders::_1));
  } else {
    std::cout << requestManager.request.to_string();
    response = requestManager.executeRequest();
    sendResponse();
  }
}

void Connection::handleBodyRead(const std::error_code& error) {
  if (error) {
    std::cerr << error.message() << std::endl;
    return;
  }
  requestManager.request.setBody(getStringFromBuff(
      std::stoi(requestManager.request.getHeader("Content-Length")), ""));

  std::cout << requestManager.request.to_string() << std::endl;
  response = requestManager.executeRequest();
  sendResponse();
}

void Connection::sendResponse() {
  boost::asio::async_write(
      clientSocket, boost::asio::buffer(response.to_string()),
      std::bind(&Connection::handleWrite, shared_from_this(),
                std::placeholders::_1));
}

void Connection::handleWrite(const std::error_code& error) {
  clientSocket.close();
}

std::string Connection::getStringFromBuff(size_t bytes_transferred,
                                          std::string_view delim) {
  return std::string(boost::asio::buffers_begin(buffer.data()),
                     boost::asio::buffers_begin(buffer.data()) +
                         bytes_transferred - delim.length());
}