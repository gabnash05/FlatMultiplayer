#include "../include/GameMap.hpp"

using namespace std;

//Constructor
GameMap::GameMap(int n) : size(n), grid(n, vector<char>(n, '.')) {}

char GameMap::checkGrid(int x, int y) {
  return grid[x][y];
}

bool GameMap::movePlayer(char player, int oldX, int oldY, int newX, int newY) {
  if (oldX >= 0 && oldX < size && oldY >= 0 && oldY < size && grid[oldX][oldY] == player && grid[newX][newY] == '.') {
    grid[oldX][oldY] = '.';
    grid[newX][newY] = player;
    return true;
  }

  return false;
}

bool GameMap::addPlayer(char player, int x, int y) {
  if (x >= 0 && x < size && y >= 0 && y < size && grid[x][y] == '.') {
    grid[x][y] = player;
    return true;
  }
  
  return false;
}

bool GameMap::removePlayer(char player, int x, int y) {
  if (x >= 0 && x < size && y >= 0 && y < size && grid[x][y] == player) {
    grid[x][y] = '.';
    return true;
  }

  return false;
}

void GameMap::printGameMap() {
  for (const auto& row : grid) {
    for (const auto& cell : row) {
        cout << cell << " ";
    }
    cout << "\n";
  }
  cout << "\n";
}