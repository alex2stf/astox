//include header
#include "astoxtests.h"
#include "socket.h"

using namespace astox;
int main(int argc, char* argv[]) {
cout << "ssss" << endl;
  TCPSocketServer server;

  cout << "22222" << endl;
  //server.setPort(27015).setMaxClients(50);
  server.setPort(7332);

   cout << "232323" << endl;
  server.start();
}
