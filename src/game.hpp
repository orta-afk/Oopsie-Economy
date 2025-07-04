#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <optional>

struct windowData {
  unsigned int width;
  unsigned int height;
  const char *title;
};

class Game {
public:
  Game();
  void initGame();
  void updateGame();
  void renderGame();
  const bool isRunning() const;
  ~Game();

private:
  void initWindow();
  void initStuff();
  void handlePollEvent();

private:
  windowData wd;
  sf::VideoMode videoMode;
  sf::RenderWindow* window;
};
