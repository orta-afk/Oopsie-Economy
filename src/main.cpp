#include "SFML/System/Clock.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <optional>

#define TILESIZE 16

struct window {
  unsigned int width = 960;
  unsigned int height = 540;
  const char *title = "lmao";
};

struct EntityStuff {
  sf::Vector2f velocity;
  sf::Vector2f positions;
  int speed = 200;
  int XIndex = 0;
  int YIndex = 0;
  int spriteSize = 16;
};

class Entity : public sf::Drawable, public sf::Transformable {
public:
  Entity() : Entitysprite(Entitytexture) { initEntity(); }

  void initEntity() {
    sf::IntRect rect({entityStuff.XIndex * entityStuff.spriteSize,
                      entityStuff.YIndex * entityStuff.spriteSize},
                     {entityStuff.spriteSize, entityStuff.spriteSize});

    if (!Entitytexture.loadFromFile("../assets/Sprite-0001.png", false, rect)) {
      std::cout << "Failed to load texture\n";
    } else {
      Entitysprite.setTexture(Entitytexture);
      Entitysprite.setTextureRect(rect);
      Entitysprite.setPosition({100, 100});
      entityStuff.positions = {100, 100}; 
    }

    int x = entityStuff.XIndex * entityStuff.spriteSize;
    if (x + entityStuff.spriteSize > static_cast<int>(Entitytexture.getSize().x)) {
      std::cout << "Sub-rect out of bounds now suck tits\n";
    }
  }

  void updateEntity(float dt) {
    move(dt);
    entityStuff.positions += entityStuff.velocity * dt;
    Entitysprite.setPosition(entityStuff.positions); 
  }

private:
  void move(float dt) {
    entityStuff.velocity = {0, 0};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
      entityStuff.velocity.x += entityStuff.speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
      entityStuff.velocity.x -= entityStuff.speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
      entityStuff.velocity.y -= entityStuff.speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
      entityStuff.velocity.y += entityStuff.speed;
  }

  EntityStuff entityStuff;
  sf::Sprite Entitysprite;
  sf::Texture Entitytexture;

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(Entitysprite, states);
  }
};

int main() {
  window win;
  Entity entity;

  sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
  sf::RenderWindow window(sf::VideoMode({win.width, win.height}), win.title,
                          sf::Style::Titlebar | sf::Style::Close);
  sf::Vector2u windowSize = window.getSize();
  sf::Vector2i centeredPosition(desktopMode.size.x / 2 - windowSize.x / 2,
                                desktopMode.size.y / 2 - windowSize.y / 2);
  window.setPosition(centeredPosition);
  window.setVerticalSyncEnabled(true);

  sf::Clock clock;
  while (window.isOpen()) {
    float dt = clock.restart().asSeconds();
    entity.updateEntity(dt);
    while (const std::optional<sf::Event> event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }
    window.clear();
    window.draw(entity);
    window.display();
  }
}
