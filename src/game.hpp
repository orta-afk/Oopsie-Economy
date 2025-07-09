#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "entity.hpp"

struct windowData{
  unsigned int width;
  unsigned int height;
  const char* title;  
};

class Game{
public:
  Game();
  void initGame();
  void updateGame();
  void renderGame();
  const bool isRunning() const;
  ~Game();

private:
  void initStuff();

private:
  windowData win;
  sf::RenderWindow* window;
  sf::VideoMode videoMode;

private:
  Entity entity;
};
