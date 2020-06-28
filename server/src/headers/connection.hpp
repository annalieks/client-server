#ifndef CLIENT_CONNECTION_HPP
#define CLIENT_CONNECTION_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <boost/asio.hpp>

#include "request.hpp"
#include "data_file.hpp"
#include "request_manager.hpp"

class Connection : public std::enable_shared_from_this<Connection> {
private:
    boost::asio::ip::tcp::socket clientSocket;
    boost::asio::streambuf buffer;
    RequestManager requestManager;
    HTTP::Response response;

    void handleStatusLineRead(const std::error_code& error,
                           std::size_t bytes_transferred);
    void handleHeadersRead(const std::error_code& error,
                           std::size_t bytes_transferred);
    void handleBodyRead(const std::error_code& error);

    void handleWrite(const std::error_code& error);
    void sendResponse();
    std::string getStringFromBuff(size_t bytes_transferred,
                                  std::string_view delim);

public:
    Connection(boost::asio::io_context& io_context,
               const std::shared_ptr<DataFile>& dataFile);

    boost::asio::ip::tcp::socket& get_socket();
    void readFromClient();
};

#endif //CLIENT_CONNECTION_HPP
