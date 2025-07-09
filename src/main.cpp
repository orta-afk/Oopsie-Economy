#include "game.hpp"
#include <cstdlib>

int main(){
  Game game;
  game.initGame();
  while(game.isRunning()){
    game.updateGame();
    game.renderGame();
  }
  return EXIT_SUCCESS;
}
