#include "game.hpp"

Game::Game(){
  initGame();
  initStuff();
};

void Game::initGame(){
  wd.width = 960;
  wd.height = 540;
  wd.title = "lmao";
  initWindow();
}

void Game::initStuff(){}

void Game::updateGame(){
  handlePollEvent();
}

void Game::renderGame(){
  window->clear(sf::Color::Cyan);
  window->display();
}

void Game::handlePollEvent(){
  while(const std::optional<sf::Event> event = window->pollEvent()){
    if(event->is<sf::Event::Closed>()){
      window->close();
    }
    if(const auto* key = event->getIf<sf::Event::KeyPressed>()){
      if(key->scancode == sf::Keyboard::Scancode::Escape){
        window->close();
      }
    }
  }
}

void Game::initWindow(){
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
  window = new sf::RenderWindow(sf::VideoMode({wd.width,wd.height}), wd.title, sf::Style::Titlebar | sf::Style::Close, sf::State::Windowed);
  sf::Vector2u windowSize = window->getSize();
  sf::Vector2i centeredPosition(desktopMode.size.x / 2 - windowSize.x / 2,
                                desktopMode.size.y / 2 - windowSize.y / 2);
  window->setPosition(centeredPosition);
  window->setVerticalSyncEnabled(true);
}

const bool Game::isRunning() const{
  return window->isOpen();
}

Game::~Game(){
  delete window;
}
