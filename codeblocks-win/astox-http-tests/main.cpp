#include <iostream>

#include "astoxtests.h"
#include "socket.h"

using namespace astox;
int main(int argc, char* argv[]) {

  TCPSocketServer server;
  //server.setPort(27015).setMaxClients(50);
  server.setPort(8080).setMaxClients(50);
  server.start();
}
