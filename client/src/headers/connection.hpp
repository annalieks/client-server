#ifndef CLIENT_CONNECTION_HPP
#define CLIENT_CONNECTION_HPP

#include <memory>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <string>

#include "response.hpp"

class Connection : public std::enable_shared_from_this<Connection> {
 private:
  boost::asio::ip::tcp::socket socket;
  boost::asio::streambuf buffer;
  HTTP::Response response;

  void handleConnect(const boost::system::error_code& ec,
                     const boost::asio::ip::tcp::endpoint& endpoint,
                     const std::string& req);
  void handleWrite(const boost::system::error_code& ec);
  void handleStatusLineRead(const boost::system::error_code& ec,
                            size_t bytes_transferred);
  void handleHeadersRead(const boost::system::error_code& ec,
                         size_t bytes_transferred);
  void handleBodyRead(const boost::system::error_code& ec);
  std::string getStringFromBuff(size_t bytes_transferred,
                                std::string_view delim);

 public:
  explicit Connection(
      const std::shared_ptr<boost::asio::io_context>& io_context);

  void writeRequest(const std::string& request);
  void showResponse();
  void closeConnection();

  void startConnection(
      const boost::asio::ip::tcp::resolver::results_type& endpoints,
      const std::string& req);
};

#endif  // CLIENT_CONNECTION_HPP
