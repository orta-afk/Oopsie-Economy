#include "game.hpp"

int main(){
  Game game;
  game.initGame();
  while(game.isRunning()){
    game.updateGame();
    game.renderGame();
  }
  return EXIT_SUCCESS;
}
