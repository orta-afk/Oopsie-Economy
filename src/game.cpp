#include "game.hpp"

Game::Game(){
  initGame();
  initStuff();
}

void Game::initGame(){
  win.width = 960;
  win.height = 528;
  win.title = "shit";
  window = new sf::RenderWindow(sf::VideoMode({win.width, win.height}), win.title, sf::Style::Titlebar | sf::Style::Close, sf::State::Windowed);
  sf::Vector2i getDesktopSize = (sf::Vector2i)videoMode.getDesktopMode().size;
  sf::Vector2i getWindowSize = (sf::Vector2i)window->getSize();
  window->setPosition(sf::Vector2i(getDesktopSize.x/2 - getWindowSize.x/2, getDesktopSize.y/2 - getWindowSize.y/2));
  window->setVerticalSyncEnabled(true);
}

void Game::initStuff(){
  entity.initEntity();
  tilemap.initTilemap();
  tilemap.updateTilemap();
  sat.initSat();
}

void Game::updateGame() {
  win.dt = clock.restart().asSeconds();
  if (auto mtv = sat.collided()) {
    win.isCollided = true;
    entity.updateEntity(win.dt, *mtv);
  } else {
    win.isCollided = false;
    entity.updateEntity(win.dt, *mtv);
  }

  while (const std::optional<sf::Event> event = window->pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window->close();
    }
    if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
      if (key->scancode == sf::Keyboard::Scancode::Escape) {
        window->close();
      }
    }
  }
}


void Game::renderGame(){
  window->clear(sf::Color::Black);
  window->draw(entity);
  window->draw(tilemap);
  window->display();  
}

const bool Game::isRunning() const{
  return window->isOpen();
}

Game::~Game(){
  delete window;
}
