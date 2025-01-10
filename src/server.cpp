#include "../include/socketsUtil.hpp"
#include "../include/GameState.hpp"

#include <algorithm>

using namespace std;

#define PORT 3000
#define MAP_SIZE 20

vector<int> activeClients;
mutex clientListMutex;

void handleClient(AcceptedSocket *clientSocket, GameState& gameState, int playerId);
void broadcastMap(GameState& gameState);

int main() {
  // SERVER SOCKET
  int serverSocketFD = createTCPIpV4Socket();
  sockaddr_in *serverAddress = createIPv4Address("", PORT);

  if (bind(serverSocketFD, (struct sockaddr*)serverAddress, sizeof(sockaddr_in)) == -1) {
    perror("Error binding socket");
    close(serverSocketFD);
    delete serverAddress;
    return -1;
  }
  delete serverAddress;

  if(listen(serverSocketFD, 10) == -1) {
    perror("Error setting socket to listen");
    close(serverSocketFD);
    return -1;
  }

  cout << "Server is running on port " << PORT << endl;

  // ----------------------------------------------------


  GameState gameState(MAP_SIZE);
  vector<thread> clientThreads;

  // Create threads for each client
  while (true) {
    AcceptedSocket *clientSocket = acceptConnection(serverSocketFD);
    int playerId = clientSocket->socketFD;
    if (!clientSocket->acceptedSuccesfully) {
      perror("Failed to accept client connection");
      delete clientSocket;
      continue;
    }

    gameState.addClient(playerId);

    clientThreads.emplace_back(handleClient, clientSocket, ref(gameState), playerId);
    cout << "New client: " << playerId << endl;
  }

  // Join threads
  for (auto& t : clientThreads) {
    if (t.joinable()) t.join();
  }

  close(serverSocketFD);
  return 0;
}


// Function
void handleClient(AcceptedSocket *clientSocket, GameState& gameState, int playerId) {
  int clientFD = clientSocket->socketFD;
  delete clientSocket;

  {
    lock_guard<mutex> lock(clientListMutex);
    activeClients.push_back(clientFD);
  }

  // Send mapSize initially
  int mapSize = gameState.getGameMap().getMapSize();
  int networkOrderSize = htonl(mapSize);
  if (send(clientFD, &networkOrderSize, sizeof(networkOrderSize), 0) == -1) {
    perror("Failed to send map size");
    close(clientFD);
    return;
  };

  broadcastMap(gameState);

  char command;
  while (true) {
    ssize_t bytesReceived = recv(clientFD, &command, sizeof(char), 0);
    if (bytesReceived == 0) {
      cout << "Client " << playerId << " disconnected." << endl;
      gameState.removeClient(playerId);
      break;
    } else if (bytesReceived == -1) {
      perror("Error receiving command");
      break;
    }

    Direction direction;
    switch (command){
      case 'W': direction = Direction::UP; break;
      case 'A': direction = Direction::LEFT; break;
      case 'S': direction = Direction::DOWN; break;
      case 'D': direction = Direction::RIGHT; break;
      default: continue;
    }

    if (gameState.moveClient(playerId, direction)) {
      broadcastMap(gameState);
    }
  }

  // Remove clients
  {
    lock_guard<mutex> lock(clientListMutex);
    activeClients.erase(
      std::remove(activeClients.begin(), activeClients.end(), clientFD),
      activeClients.end()
    );
  }

  broadcastMap(gameState);
  close(clientFD);
}

void broadcastMap(GameState& gameState) {
  auto mapSnapshot = gameState.getGameMap();
  const auto& grid = mapSnapshot.getGrid();
  int mapSize = grid.size();

  // Save to buffer
  vector<char> buffer(mapSize * mapSize);
  for (int i = 0; i < mapSize; i++) {
    for (int j = 0; j < mapSize; j++) {
      buffer[i * mapSize + j] = grid[i][j];
    }
  }

  // Sending data
  lock_guard<std::mutex> lock(clientListMutex);
  for (int client : activeClients) {
    if (send(client, buffer.data(), buffer.size(), 0) == -1) {
      perror("Failed to send map data to a client");
    }
  }
}