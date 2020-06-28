#ifndef SERVER_HPP
#define SERVER_HPP

#include <unordered_map>
#include <string>
#include <boost/asio.hpp>

#include "connection.hpp"
#include "data_file.hpp"

class Server {
private:
    const unsigned int _port;
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::acceptor acceptor;
    std::shared_ptr<DataFile> dataFileJson;

    void start_accept();
    void run();
    void handle_accept(const std::shared_ptr<Connection>& connection,
                       const boost::system::error_code& e);

public:
    explicit Server(unsigned int port = 80);
    void run_server();

};


#endif //SERVER_HPP
