#include "../include/socketsUtil.hpp"

// UTIL FUNCTIONS

int createTCPIpV4Socket(void) {
  int socketFD = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFD < 0) {
    perror("Failed to create socket");
    return -1;
  }

  int opt = 1;
  if (setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
    perror("Error setting socket options");
    close(socketFD);
    return -1;
  }

  return socketFD;
}

sockaddr_in *createIPv4Address(const char *ip, int port) {

  sockaddr_in* ipV4Addr = new sockaddr_in;
  memset(ipV4Addr, 0, sizeof(sockaddr_in));

  ipV4Addr->sin_family = AF_INET;
  ipV4Addr->sin_port = htons(port);

  if (strlen(ip) == 0) {
    ipV4Addr->sin_addr.s_addr = INADDR_ANY;
  } else {
    if (inet_pton(AF_INET, ip, &ipV4Addr->sin_addr) <= 0) {
      perror("Invalid IP Address");
      delete ipV4Addr;
      return nullptr;
    };
  }
  
  return ipV4Addr;
}

AcceptedSocket *acceptConnection(int serverSocketFD) {
  sockaddr_in clientAddress;
  socklen_t clientAddressSize = sizeof(clientAddress);
  int clientSocketFD = accept(serverSocketFD, (struct sockaddr*)&clientAddress, &clientAddressSize);
  
  AcceptedSocket *newSocket = (AcceptedSocket *)malloc(sizeof(struct AcceptedSocket));
  if (socket == NULL) {
    perror("Failed to allocate memory for AcceptedSocket");
    return NULL; 
  }

  newSocket->address = clientAddress;
  newSocket->socketFD = clientSocketFD;
  
  if (clientSocketFD < 0) {
    newSocket->acceptedSuccesfully = 0;
    newSocket->error = errno;  
  } else {
    newSocket->acceptedSuccesfully = 1;
    newSocket->error = 0;
  }

  return newSocket;
}