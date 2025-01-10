#include "../include/GameState.hpp"

using namespace std;

GameState::GameState(int size) : gameMap(size), mapSize(size) {}

char GameState::convertPlayerIdToChar(int id) { return (id % 26) + 'A'; }

bool GameState::addClient(int playerId) {
  lock_guard<mutex> lock(gameStateMutex);
  char player = convertPlayerIdToChar(playerId);

  //Random number generation
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, mapSize - 1);

  int attempts = 0;
  while (attempts < mapSize * mapSize) {
    int x = dis(gen);
    int y = dis(gen);

    if (gameMap.addPlayer(player, x, y)) {
      clients[player] = {x, y};
      return true;
    }
    attempts++;
  }

  return false;
}

bool GameState::removeClient(int playerID) {
  lock_guard<mutex> lock(gameStateMutex);
  
  char player = convertPlayerIdToChar(playerID);
  auto it = clients.find(player);
  if (it != clients.end()) {
    int x = it->second.first;
    int y = it->second.second;
    gameMap.removePlayer(player, x, y);
    clients.erase(it);
    return true;
  }

  return false;
}

bool GameState::moveClient(char playerID, Direction direction) {
  lock_guard<mutex> lock(gameStateMutex);

  char player = convertPlayerIdToChar(playerID);
  auto it = clients.find(player);

  if (it == clients.end()) return false;

  int x = it->second.first;
  int y = it->second.second;
  int dx = directionOffsets.at(direction).first;
  int dy = directionOffsets.at(direction).second;

  if (x + dx >= 0 && x + dx < mapSize && y + dy >= 0 && y + dy < mapSize) {
    if (gameMap.movePlayer(player, x, y, x+dx, y+dy)) {
      clients[player] = {x + dx, y + dy};
      return true;
    }
  }

  return false;
}

GameMap GameState::getGameMap() {
  lock_guard<mutex> lock(gameStateMutex);

  return gameMap;
}

void GameState::printGameMap() {
  gameMap.printGameMap();
}