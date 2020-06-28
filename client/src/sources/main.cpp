#include <iostream>

#include "client.hpp"

int main(int argc, char* argv[]) {
  std::shared_ptr<boost::asio::io_context> context(new boost::asio::io_context);
  Client client(context);
  std::string command;
  do {
    context->reset();
    std::cin >> command;
    if (command == "add") {
      client.addPair();
    } else if (command == "get") {
      client.getValue();
    } else if (command == "set") {
      client.setPair();
    } else if (command == "delete") {
      client.deletePair();
    }
    context->run();
  } while (command != "q");
  return 0;
}