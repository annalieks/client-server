#include "server.hpp"

int main() {
  Server server(3200);
  server.run_server();
  return 0;
}