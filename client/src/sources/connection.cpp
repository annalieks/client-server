#include <iostream>

#include "connection.hpp"
#include "response.hpp"

Connection::Connection(
    const std::shared_ptr<boost::asio::io_context>& io_context)
    : socket(*io_context) {}

void Connection::startConnection(
    const boost::asio::ip::tcp::resolver::results_type& endpoints,
    const std::string& req) {
  boost::asio::async_connect(
      socket, endpoints,
      std::bind(&Connection::handleConnect, shared_from_this(),
                std::placeholders::_1, std::placeholders::_2, req));
}

void Connection::handleConnect(const boost::system::error_code& ec,
                               const boost::asio::ip::tcp::endpoint& endpoint,
                               const std::string& req) {
  if (ec) {
    std::cerr << ec.message() << std::endl;
    return;
  }
  writeRequest(req);
}

void Connection::writeRequest(const std::string& req) {
  boost::asio::async_write(
      socket, boost::asio::buffer(req),
      std::bind(&Connection::handleWrite, shared_from_this(),
                std::placeholders::_1));
}

void Connection::handleWrite(const boost::system::error_code& ec) {
  if (ec) {
    std::cerr << "Error occured. Request was not written";
    return;
  }
  boost::asio::async_read_until(
      socket, buffer, HTTP::Message::delim,
      std::bind(&Connection::handleStatusLineRead, shared_from_this(),
                std::placeholders::_1, std::placeholders::_2));
}

void Connection::handleStatusLineRead(const boost::system::error_code& ec,
                                      size_t bytes_transferred) {
  response.setStatusLine(
      getStringFromBuff(bytes_transferred, HTTP::Message::delim));
  buffer.consume(bytes_transferred);
  boost::asio::async_read_until(
      socket, buffer, HTTP::Message::doubleDelim,
      std::bind(&Connection::handleHeadersRead, shared_from_this(),
                std::placeholders::_1, std::placeholders::_2));
}

void Connection::handleHeadersRead(const boost::system::error_code& ec,
                                   size_t bytes_transferred) {
  response.setHeaders(
      getStringFromBuff(bytes_transferred, HTTP::Message::doubleDelim));
  buffer.consume(bytes_transferred);

  if (!response.getHeader("Content-Length").empty()) {
    size_t contentLength = std::stoi(response.getHeader("Content-Length"));
    size_t transferBytes =
        contentLength - std::min(contentLength, buffer.size());

    boost::asio::async_read(
        socket, buffer, boost::asio::transfer_exactly(transferBytes),
        std::bind(&Connection::handleBodyRead, shared_from_this(),
                  std::placeholders::_1));
  } else {
    showResponse();
  }
}

void Connection::handleBodyRead(const boost::system::error_code& ec) {
  response.setBody(
      getStringFromBuff(std::stoi(response.getHeader("Content-Length")), ""));
  showResponse();
}

void Connection::showResponse() {
  std::cout << response.to_string() << std::endl;
  closeConnection();
}

void Connection::closeConnection() { socket.close(); }

std::string Connection::getStringFromBuff(size_t bytes_transferred,
                                          std::string_view delim) {
  return std::string(boost::asio::buffers_begin(buffer.data()),
                     boost::asio::buffers_begin(buffer.data()) +
                         bytes_transferred - delim.length());
}
