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
  int spriteSize = 16;
  int XIndex = 0;
  int YIndex = 0;
};

class Entity : public sf::Drawable, public sf::Transformable {
public:
  Entity() : Entitysprite(Entitytexture) { initEntity(); };

  void initEntity() {
    sf::IntRect rect({entityStuff.XIndex * entityStuff.spriteSize,
                      entityStuff.YIndex * entityStuff.spriteSize},
                     {entityStuff.spriteSize, entityStuff.spriteSize});
    if (!Entitytexture.loadFromFile("../assets/Sprite-0001.png", false,
                                    rect)) {
      std::cout << "Fuck";
    } else {
      Entitysprite.setTexture(Entitytexture);
      Entitysprite.setTextureRect(rect);
      Entitysprite.setPosition({100, 100});
    }
    if(entityStuff.XIndex+16 > Entitytexture.getSize().x){
      std::cout << "fuck";
    }
  };

  void updateEntity();

private:
  EntityStuff entityStuff;
  sf::Sprite Entitysprite;
  sf::Texture Entitytexture;

private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(Entitysprite);
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

  while (window.isOpen()) {
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
