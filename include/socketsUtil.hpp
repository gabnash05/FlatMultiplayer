#ifndef SOCKETUTIL_SOCKETUTIL_H
#define SOCKETUTIL_SOCKETUTIL_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>

struct AcceptedSocket {
  int socketFD;
  struct sockaddr_in address;
  int error;
  bool acceptedSuccesfully;
};

int createTCPIpV4Socket(void);
sockaddr_in *createIPv4Address(const char *ip, int port);
AcceptedSocket *acceptConnection(int serverSocketFD);

#endif //SOCKETUTIL_SOCKETUTIL_H