#ifndef GAMESTATE_UTIL_HPP
#define GAMESTATE_UTIL_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <mutex>
#include <random>
#include <ctime>


#include "../include/GameMap.hpp"

enum class Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

const map<Direction, pair<int, int>> directionOffsets = {
  {Direction::UP, {-1, 0}},
  {Direction::DOWN, {1, 0}},
  {Direction::LEFT, {0, -1}},
  {Direction::RIGHT, {0, 1}}
};

class GameState {
private:
  unordered_map<char, pair<int, int>> clients;
  GameMap gameMap;
  int mapSize;
  mutex gameStateMutex; 

public:
  GameState(int size);

  char convertPlayerIdToChar(int id);

  bool addClient(int playerId);

  bool removeClient(int playerID);

  bool moveClient(char playerID, Direction direction);

  GameMap getGameMap();

  void printGameMap();
};

#endif