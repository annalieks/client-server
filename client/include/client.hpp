#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <boost/asio.hpp>
#include <string>

#include "connection.hpp"

enum Commands { add, get, set };

class Client : public std::enable_shared_from_this<Client> {
 private:
  const std::string hostAddress = "127.0.0.1";
  const std::string port = "3200";

  boost::asio::ip::tcp::resolver resolver;

  std::shared_ptr<boost::asio::io_context> io_context;
  std::shared_ptr<Connection> connection;

 public:
  explicit Client(const std::shared_ptr<boost::asio::io_context> &context);
  std::string composeRequest(const std::string &method,
                             const std::string &target,
                             const std::string &body = "");

  void start(const std::string &req);
  void addPair();
  void getValue();
  void setPair();
  void deletePair();
};

#endif  // CLIENT_HPP