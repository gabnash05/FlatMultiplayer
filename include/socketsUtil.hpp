#ifndef SOCKETUTIL_HPP
#define SOCKETUTIL_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>

// Structure used to store accepted sockets when server is accepting connections
struct AcceptedSocket {
  int socketFD;
  struct sockaddr_in address;
  int error;
  bool acceptedSuccesfully;
};

/*
  Creates an IPV4 Socket with reusable address options.

  Returns a file descriptor
*/ 
int createTCPIpV4Socket(void);

/*
  Creates an IPV4 Address 

  Returns a socket address 
*/ 
sockaddr_in *createIPv4Address(const char *ip, int port);

/*
  Accepts connections for server

  Returns an Accepted Socket Struct with all the information from the connection
*/
AcceptedSocket *acceptConnection(int serverSocketFD);

#endif //SOCKETUTIL_HPP