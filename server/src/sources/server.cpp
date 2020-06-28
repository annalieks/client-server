#include <iostream>
#include <thread>
#include <vector>

#include "connection.hpp"
#include "server.hpp"

using namespace boost::asio::ip;

enum TCPConsts {
  max_connections = 1024,
  max_threads = 4,
};

Server::Server(unsigned int port)
    : _port(port),
      acceptor(io_context, tcp::endpoint(tcp::v4(), port), true),
      dataFileJson(std::make_shared<DataFile>()) {}

void Server::run_server() {
  acceptor.listen(max_connections);

  start_accept();

  std::cout << "Run server on " << _port << " port" << std::endl;

  std::vector<std::thread> _threadPool;
  _threadPool.reserve(max_threads);
  for (int i = 0; i < max_threads; i++)
    _threadPool.emplace_back(std::thread([this] { run(); }));
  for (auto& thread : _threadPool) thread.join();
}

void Server::handle_accept(const std::shared_ptr<Connection>& connection,
                           const boost::system::error_code& error) {
  if (!error) {
    connection->readFromClient();
  }

  start_accept();
}

void Server::start_accept() {
  std::shared_ptr<Connection> connection =
      std::make_shared<Connection>(io_context, dataFileJson);

  acceptor.async_accept(connection->get_socket(),
                        std::bind(&Server::handle_accept, this, connection,
                                  std::placeholders::_1));
}

void Server::run() { io_context.run(); }