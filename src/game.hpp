#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "SFML/System/Clock.hpp"
#include "entity.hpp"
#include "tilemap.hpp"

struct windowData{
  unsigned int width;
  unsigned int height;
  const char* title;  
  float dt;
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
  sf::Clock clock;
  sf::RenderWindow* window;
  sf::VideoMode videoMode;

private:
  Tilemap tilemap;
  Entity entity;
};
