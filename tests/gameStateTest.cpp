#include "../include/GameState.hpp"

int main(void) {
  GameState gameState(10);

  gameState.addClient(4);
  gameState.printGameMap();

  gameState.moveClient(4, Direction::RIGHT);
  gameState.printGameMap();

  gameState.moveClient(4, Direction::DOWN);
  gameState.printGameMap();

  gameState.moveClient(4, Direction::LEFT);
  gameState.printGameMap();

  gameState.moveClient(4, Direction::UP);
  gameState.printGameMap();

  gameState.removeClient(4);
  gameState.printGameMap();
}