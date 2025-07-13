#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#include "entity.hpp"
#include "tilemap.hpp"
#include "sat.hpp"

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
  Sat sat;
  Tilemap tilemap;
  Entity entity;
};
