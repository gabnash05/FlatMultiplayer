#include "../include/socketsUtil.hpp"

#define PORT 3000

using namespace std;

int main(){
  int clientFD = createTCPIpV4Socket();
  sockaddr_in *serverAddress = createIPv4Address("127.0.0.1", PORT);

  if (connect(clientFD, (struct sockaddr*)serverAddress, sizeof(sockaddr_in)) == -1) {
    perror("Connection failed");
    close(clientFD);
    return -1;
  }

  cout << "Connection Successful" << endl;

  return 0;
}