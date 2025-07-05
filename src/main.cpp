#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <optional>
#include <unordered_map>

#define TILESIZE 16

struct window {
  unsigned int width = 960;
  unsigned int height = 528;
  const char *title = "lmao";
};

struct EntityStuff {
  sf::Vector2f velocity;
  sf::Vector2f positions;
  int g = 400;
  int max_g = 600;
  int speed = 200;
  int XIndex = 24;
  int YIndex = 0;
  int spriteSize = 16;
};

// 18

enum class tiles : int {
  background = 0,
  topLeftCorner,
  topMiddleTile,
  topRightCorner,
  middleLeft,
  middleRight,
  bottomLeftCorner,
  bottomMiddleTile,
  bottomRightCorner,
};

struct indexs {
  int x;
  int y;
};

class Entity : public sf::Drawable, public sf::Transformable {
public:
  Entity() : Entitysprite(Entitytexture) { initEntity(); }

  void initEntity() {
    sf::IntRect rect({entityStuff.XIndex * entityStuff.spriteSize,
                      entityStuff.YIndex * entityStuff.spriteSize},
                     {entityStuff.spriteSize, entityStuff.spriteSize});

    if (!Entitytexture.loadFromFile(
            "../assets/colored-transparent_packed.png")) {
      std::cout << "Failed to load texture\n";
    } else {
      Entitysprite.setTexture(Entitytexture);
      Entitysprite.setTextureRect(rect);
      Entitysprite.setPosition({100, 100});
      entityStuff.positions = {100, 100};
    }

    int x = entityStuff.XIndex * entityStuff.spriteSize;
    if (x + entityStuff.spriteSize >
        static_cast<int>(Entitytexture.getSize().x)) {
      std::cout << "Sub-rect out of bounds now suck tits\n";
    }
  }

  void updateEntity(float dt) {
    move(dt);
    gravity(dt);
    entityStuff.positions += entityStuff.velocity * dt;
    entityBounds();
    Entitysprite.setPosition(entityStuff.positions);
  }

public:
  sf::FloatRect entityBounds(){
    return Entitysprite.getGlobalBounds();
  }

private:
  void gravity(float dt){
    entityStuff.velocity.y += entityStuff.g * dt;
    if(entityStuff.velocity.y > entityStuff.max_g){
      entityStuff.velocity.y = entityStuff.max_g;
    }
  }
  
  void move(float dt) {
    entityStuff.velocity = {0, 0};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
      entityStuff.velocity.x += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
      entityStuff.velocity.x -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
      entityStuff.velocity.y -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
      entityStuff.velocity.y += 1;

    if (entityStuff.velocity.length() > 0) {
      entityStuff.velocity = entityStuff.velocity.normalized();
    }
    entityStuff.velocity *= static_cast<float>(entityStuff.speed);
  }

  EntityStuff entityStuff;
  sf::Sprite Entitysprite;
  sf::Texture Entitytexture;

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(Entitysprite, states);
  }
};

class Tilemap : public sf::Drawable, public sf::Transformable {
public:
  Tilemap() : vert(sf::PrimitiveType::Triangles, 3) { initTilemap(); }
  void initTilemap(){
    if(!texture.loadFromFile("../assets/colored-transparent_packed.png")){
      std::cout << "FUUCKKKKKKK!!!!!! \n";
    }
    int tilePerRow = texture.getSize().x / TILESIZE;

    for (int j = 0; j < mapHeight; ++j) {
      for (int i = 0; i < mapWidth; ++i) {
        int tile = getTileIndex(tiles::background, tilePerRow);

        if (j == 28) {
          if (i == 5)
            tile = getTileIndex(tiles::topLeftCorner, tilePerRow);
          else if (i == mapWidth - 6)
            tile = getTileIndex(tiles::topRightCorner, tilePerRow);
          else if (i > 5 && i < mapWidth - 6)
            tile = getTileIndex(tiles::topMiddleTile, tilePerRow);
        } else if (j > 28 && j < mapHeight - 1) {
          if (i == 5)
            tile = getTileIndex(tiles::middleLeft, tilePerRow);
          else if (i == mapWidth - 6)
            tile = getTileIndex(tiles::middleRight, tilePerRow);
        } else if (j == mapHeight - 1) {
          if (i == 5)
            tile = getTileIndex(tiles::bottomLeftCorner, tilePerRow);
          else if (i == mapWidth - 6)
            tile = getTileIndex(tiles::bottomRightCorner, tilePerRow);
          else if (i > 5 && i < mapWidth - 6)
            tile = getTileIndex(tiles::bottomMiddleTile, tilePerRow);
        }
        map[j][i] = tile;
      }
    }
  }
  void updateTilemap() {
    vert.clear();
    vert.setPrimitiveType(sf::PrimitiveType::Triangles);
    vert.resize(mapWidth * mapHeight * 6);
    for (int j = 0; j < mapHeight; j++) {
      for (int i = 0; i < mapWidth; i++) {
        const int index = map[j][i]; 
        const int tu = index % (texture.getSize().x / 16);
        const int tv = index / (texture.getSize().x / 16);
        sf::Vertex *triangles = &vert[(i + j * mapWidth) * 6];

        triangles[0].position = sf::Vector2f(i * TILESIZE, j * TILESIZE);
        triangles[1].position = sf::Vector2f((i + 1) * TILESIZE, j * TILESIZE);
        triangles[2].position = sf::Vector2f(i * TILESIZE, (j + 1) * TILESIZE);
        triangles[3].position = sf::Vector2f(i * TILESIZE, (j + 1) * TILESIZE);
        triangles[4].position = sf::Vector2f((i + 1) * TILESIZE, j * TILESIZE);
        triangles[5].position = sf::Vector2f((i + 1) * TILESIZE, (j + 1) * TILESIZE);

        triangles[0].texCoords = sf::Vector2f(tu * TILESIZE, tv * TILESIZE);
        triangles[1].texCoords = sf::Vector2f((tu + 1) * TILESIZE, tv * TILESIZE);
        triangles[2].texCoords = sf::Vector2f(tu * TILESIZE, (tv + 1) * TILESIZE);
        triangles[3].texCoords = sf::Vector2f(tu * TILESIZE, (tv + 1) * TILESIZE);
        triangles[4].texCoords = sf::Vector2f((tu + 1) * TILESIZE, tv * TILESIZE);
        triangles[5].texCoords = sf::Vector2f((tu + 1) * TILESIZE, (tv + 1) * TILESIZE);
      }
    }
  }

public:
  sf::FloatRect getTileBounds(){
    return vert.getBounds();
  }
  
private:
  int getTileIndex(tiles t, int tilesPerRow) {
    indexs coords = mapIndex.at(t);
    return coords.x + coords.y * tilesPerRow;
  }
  
private:
  const static int mapWidth = 60;
  const static int mapHeight = 33;
  sf::Texture texture;
  sf::VertexArray vert;
  std::array<std::array<int, mapWidth>, mapHeight> map;
  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = &texture;
    target.draw(vert, states);
  };
  std::unordered_map<tiles, indexs> mapIndex = {
    {tiles::background, {0, 0}},
    {tiles::topLeftCorner, {18, 0}},
    {tiles::topMiddleTile, {19, 0}},
    {tiles::topRightCorner, {20, 0}}, 
    {tiles::middleLeft, {18, 1}},
    {tiles::middleRight, {20, 1}},
    {tiles::bottomLeftCorner, {18, 2}},
    {tiles::bottomMiddleTile, {19, 2}},
    {tiles::bottomRightCorner, {20, 2}},
  };
};

bool collided(Entity& e, Tilemap& t){
  if(e.entityBounds().findIntersection(t.getTileBounds())){
    std::cout << "HOLY FUCK";
  }
  return true;
}

int main() {
  window win;
  Entity entity;
  Tilemap tilemap;
  bool isCollided = false;

  sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
  sf::RenderWindow window(sf::VideoMode({win.width, win.height}), win.title,
                          sf::Style::Titlebar | sf::Style::Close);
  sf::Vector2u windowSize = window.getSize();
  sf::Vector2i centeredPosition(desktopMode.size.x / 2 - windowSize.x / 2,
                                desktopMode.size.y / 2 - windowSize.y / 2);
  window.setPosition(centeredPosition);
  window.setVerticalSyncEnabled(true);

  tilemap.initTilemap();
  tilemap.updateTilemap();
  sf::Clock clock;
  while (window.isOpen()) {
    float dt = clock.restart().asSeconds();
    entity.updateEntity(dt);
    isCollided = collided(entity, tilemap);
    while (const std::optional<sf::Event> event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }
    window.clear();
    window.draw(entity);
    window.draw(tilemap);
    window.display();
  }
}
