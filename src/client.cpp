#include "../include/socketsUtil.hpp"

#include <termios.h>
#include <vector>
#include <atomic>

#define PORT 3000
#define SERVER_IP "172.28.132.107"

using namespace std;

void sendCommands(int clientFD, atomic<bool>& isRunning);
void receiveData(int socketFD, int mapSize, atomic<bool>& isRunning);

int main(){
  int clientFD = createTCPIpV4Socket();
  sockaddr_in *serverAddress = createIPv4Address(SERVER_IP, PORT);

  if (connect(clientFD, (struct sockaddr*)serverAddress, sizeof(sockaddr_in)) == -1) {
    perror("Connection failed");
    close(clientFD);
    delete serverAddress;
    return -1;
  }
  delete serverAddress;

  cout << "Connection Successful" << endl;
  cout << "Your player ID is: " << convertSocketFDToPlayerId(clientFD) << endl;

  //---------------------------------------------------

  // Get MapSize
  int mapSize;
  if (recv(clientFD, &mapSize, sizeof(mapSize), 0) <= 0) {
    perror("Connection lost");
    return -1;
  }
  mapSize = ntohl(mapSize);

  atomic<bool> isRunning(true);

  // Create threads
  thread sender(sendCommands, clientFD, ref(isRunning));
  thread receiver(receiveData, clientFD, mapSize, ref(isRunning));

  // Close threads
  sender.join();
  isRunning = false;
  receiver.join();

  // Close the socket
  close(clientFD);

  return 0;
}

void sendCommands(int clientFD, atomic<bool>& isRunning) {
  cout << "Use WASD for controls" << endl;
  char command;

  // Terminal settings
  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  while(true) {
    command = getchar();
    command = toupper(command);

    if (command == 'W' || command == 'A' || command == 'S' || command == 'D') {
      if(send(clientFD, &command, sizeof(char), 0) == -1) {
        perror("Error sending command to server.");
        break;
      }
    } else if (command == 'Q') {
      cout << "\nExiting Game." << endl;
      isRunning = false;
      break;
    }
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void receiveData(int socketFD, int mapSize, atomic<bool>& isRunning) {
  vector<char> buffer(mapSize * mapSize);

  while (isRunning) {
    ssize_t bytesReceived = recv(socketFD, buffer.data(), buffer.size(), 0);
    if (bytesReceived <= 0) {
      if (bytesReceived == 0) {
        cerr << "Server closed the connection." << endl;
      } else {
        perror("Error receiving data");
      }
      break;
    }
    
    // Populate map
    vector<vector<char>> map(mapSize, vector<char>(mapSize, '.'));
    for (int i = 0; i < mapSize; i++) {
      for (int j = 0; j < mapSize; j++) {
        map[i][j] = buffer[i * mapSize + j];
      }
    }
    
    // Display map
    cout << "\033[2J\033[1;1H";
    for (int i = 0; i < mapSize; i++) {
      for (int j = 0; j < mapSize; j++) {
        cout << map[i][j] << ' ';
      }
      cout << '\n';
    }
    cout << '\n';
  }
}
