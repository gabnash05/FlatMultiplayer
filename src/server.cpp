#include "../include/socketsUtil.hpp"
using namespace std;

#define PORT 3000

int main() {
  // SERVER SOCKET
  int serverSocketFD = createTCPIpV4Socket();
  sockaddr_in *serverAddress = createIPv4Address("", PORT);

  if (bind(serverSocketFD, (struct sockaddr*)serverAddress, sizeof(sockaddr_in)) == -1) {
    perror("Error binding socket");
    close(serverSocketFD);
    return -1;
  }

  if(listen(serverSocketFD, 10) == -1) {
    perror("Error setting socket to listen");
    close(serverSocketFD);
    return -1;
  }

  cout << "Server is running on port " << PORT << endl;

  // ----------------------------------------------------

  while (true) {
    AcceptedSocket *clientSocket = acceptConnection(serverSocketFD);
    if (!clientSocket->acceptedSuccesfully) {
      perror("Failed to accept client connection");
      continue;
    }

    cout << "New client: " << clientSocket->socketFD << endl;
  }

  close(serverSocketFD);
  return 0;
}
