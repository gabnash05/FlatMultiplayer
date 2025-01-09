#include <iostream>
#include <vector>

using namespace std;

class GameMap {
  private:
    vector<vector<char>> grid;
    int size;

  public:
    //Constructor
    GameMap(int n);

    char checkGrid(int x, int y);
    
    bool movePlayer(char player, int oldX, int oldY, int newX, int newY);

    bool addPlayer(char player, int x, int y);

    bool removePlayer(char player, int x, int y);

    void printGameMap();
};