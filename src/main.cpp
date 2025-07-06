#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>
#include <unordered_map>

#define TILESIZE 16

bool isblah;

struct window {
  unsigned int width = 960;
  unsigned int height = 528;
  const char *title = "Oopsie Economy";
};

struct EntityStuff {
  sf::Vector2f velocity = {0.f, 0.f};
  sf::Vector2f positions;
  bool onGround;
  float g = 1200.f;
  float jumpVelocity = 300.f;
  float max_g = 2000.f;
  float speed = 250.f;
  float friction = 0.7f;
  int XIndex = 26;
  int YIndex = 0;
  int spriteSize = 16;
  bool canJump = false;
};

enum class tiles : int {
  background = 0,
  topMiddleTile,
  bottomMiddleTile,
  pillertop,
  pillerMiddle,
  pillerBottom,
};

struct indexs {
  int x;
  int y;
};

enum class collisionType : int{
  None,
  RightWall,
  bottomGround,
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
      entityStuff.positions = {(5 + (54 - 5) / 2) * TILESIZE, (10 + (22 - 10) / 2) * TILESIZE};
      Entitysprite.setPosition({entityStuff.positions});
    }

    int x = entityStuff.XIndex * entityStuff.spriteSize;
    if (x + entityStuff.spriteSize >
        static_cast<int>(Entitytexture.getSize().x)) {
      std::cout << "Sub-rect out of bounds now suck tits\n";
    }
  }

  void updateEntity(float dt, collisionType type) {
    move(dt);
    gravity(dt); 
    entityBounds();
    resolveCollision(type);
    jump(dt);
    entityStuff.positions += entityStuff.velocity * dt;
    Entitysprite.setPosition(entityStuff.positions);
  }
public:
  sf::FloatRect entityBounds() { return Entitysprite.getGlobalBounds(); }

private:
  void resolveCollision(collisionType type) {
    switch (type) {
    case collisionType::RightWall:
      entityStuff.velocity.x = 0;

      // snap X position to tile edge
      if (entityStuff.velocity.x > 0) {
        entityStuff.positions.x =
            std::floor(entityStuff.positions.x / TILESIZE) * TILESIZE;
      } else if (entityStuff.velocity.x < 0) {
        entityStuff.positions.x =
            std::ceil(entityStuff.positions.x / TILESIZE) * TILESIZE;
      }
      break;

    case collisionType::None:
      entityStuff.onGround = false;
      break;

    case collisionType::bottomGround:
      entityStuff.velocity.y = 0;
      entityStuff.onGround = true;
      entityStuff.canJump = true;

      entityStuff.positions.y =
          std::floor(entityStuff.positions.y / TILESIZE) * TILESIZE;
      break;
    }
  }

  void gravity(float dt) {
      if (!entityStuff.onGround) {
          entityStuff.velocity.y += entityStuff.g * dt;
          if (entityStuff.velocity.y > entityStuff.max_g) {
              entityStuff.velocity.y = entityStuff.max_g;
          }
      }
  }

  void move(float dt) {
    float acceleration = 4000.f;
    float maxSpeed = entityStuff.speed;

    float targetSpeed = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
      targetSpeed += maxSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
      targetSpeed -= maxSpeed;

    if (entityStuff.velocity.x < targetSpeed)
      entityStuff.velocity.x = std::min(entityStuff.velocity.x + acceleration * dt, targetSpeed);
    else if (entityStuff.velocity.x > targetSpeed)
      entityStuff.velocity.x = std::max(entityStuff.velocity.x - acceleration * dt, targetSpeed);

    if (std::abs(entityStuff.velocity.x) < 5.f)
      entityStuff.velocity.x = 0.f;
  }

  void jump(float dt) {
      if (entityStuff.onGround && entityStuff.canJump && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
          entityStuff.velocity.y -= entityStuff.jumpVelocity;
          entityStuff.canJump = false;
          entityStuff.onGround = false;
      }
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
  void initTilemap() {
    if (!texture.loadFromFile("../assets/colored-transparent_packed.png")) {
      std::cout << "FUUCKKKKKKK!!!!!! \n";
    }
    int tilePerRow = texture.getSize().x / TILESIZE;

    const int boxTop = 10;
    const int boxBottom = 22;
    const int leftCol = 5;
    const int rightCol = mapWidth - 6;

    for (int j = 0; j < mapHeight; ++j) {
      for (int i = 0; i < mapWidth; ++i) {
        int tile = getTileIndex(tiles::background, tilePerRow);
        if (j == boxTop) {
          if (i == leftCol)
            tile = getTileIndex(tiles::pillertop, tilePerRow);
          else if (i == rightCol)
            tile = getTileIndex(tiles::pillertop, tilePerRow);
          else if (i > leftCol && i < rightCol)
            tile = getTileIndex(tiles::bottomMiddleTile, tilePerRow);
        } else if (j > boxTop && j < boxBottom) {
          if (i == leftCol)
            tile = getTileIndex(tiles::pillerMiddle, tilePerRow);
          else if (i == rightCol)
            tile = getTileIndex(tiles::pillerMiddle, tilePerRow);
        } else if (j == boxBottom) {
          if (i == leftCol)
            tile = getTileIndex(tiles::pillerBottom, tilePerRow);
          else if (i == rightCol)
            tile = getTileIndex(tiles::pillerBottom, tilePerRow);
          else if (i > leftCol && i < rightCol)
            tile = getTileIndex(tiles::topMiddleTile, tilePerRow);
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
        triangles[5].position =
            sf::Vector2f((i + 1) * TILESIZE, (j + 1) * TILESIZE);

        triangles[0].texCoords = sf::Vector2f(tu * TILESIZE, tv * TILESIZE);
        triangles[1].texCoords =
            sf::Vector2f((tu + 1) * TILESIZE, tv * TILESIZE);
        triangles[2].texCoords =
            sf::Vector2f(tu * TILESIZE, (tv + 1) * TILESIZE);
        triangles[3].texCoords =
            sf::Vector2f(tu * TILESIZE, (tv + 1) * TILESIZE);
        triangles[4].texCoords =
            sf::Vector2f((tu + 1) * TILESIZE, tv * TILESIZE);
        triangles[5].texCoords =
            sf::Vector2f((tu + 1) * TILESIZE, (tv + 1) * TILESIZE);
      }
    }
  }

public:
  sf::FloatRect getTileBounds() { return vert.getBounds(); }

public:
  int getTileIndex(tiles t, int tilesPerRow) {
    indexs coords = mapIndex.at(t);
    return coords.x + coords.y * tilesPerRow;
  }

  int getTileperRow() { return texture.getSize().x / TILESIZE; }

  int getTileAt(int x, int y) { return map[y][x]; }

  int getMapWidth() { return mapWidth; }

  int getMapHeight() { return mapHeight; }

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
      {tiles::topMiddleTile, {19, 0}},     
      {tiles::bottomMiddleTile, {19, 2}},
      {tiles::pillertop, {3, 11}},
      {tiles::pillerMiddle, {3, 12}},
      {tiles::pillerBottom, {3, 13}},
  };
};

collisionType collided(Entity &e, Tilemap &t) {
  collisionType finalType = collisionType::None;
  float minOverlap = std::numeric_limits<float>::max();

  auto entityRect = e.entityBounds();

  for (int j = 0; j < t.getMapHeight(); ++j) {
    for (int i = 0; i < t.getMapWidth(); ++i) {
      int TileIndex = t.getTileAt(i, j);
      int tileperRow = t.getTileperRow();

      if (TileIndex == t.getTileIndex(tiles::background, tileperRow))
        continue;

      sf::FloatRect tileRect(
          {static_cast<float>(i) * TILESIZE, static_cast<float>(j) * TILESIZE},
          {TILESIZE, TILESIZE});

      auto intersect = entityRect.findIntersection(tileRect);
      if (intersect.has_value()) {
        sf::Vector2f overlap = intersect->size;

        if (overlap.x < overlap.y) {
          if (entityRect.position.x < tileRect.position.x)
            finalType = collisionType::RightWall;
          else
            finalType = collisionType::RightWall;

          if (overlap.x < minOverlap)
            minOverlap = overlap.x;
        } else {
          if (entityRect.position.y < tileRect.position.y)
            finalType = collisionType::bottomGround;
          else
            finalType = collisionType::bottomGround;

          if (overlap.y < minOverlap)
            minOverlap = overlap.y;
        }
      }
    }
  }

  return finalType;
}


int main() {
  window win;
  Entity entity;
  Tilemap tilemap;
  bool isCollided = false;
  collisionType type = collisionType::None;

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
    type = collided(entity, tilemap);
    entity.updateEntity(dt, type);
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
// todo - refactor this code later <- 12:00 pm almost dead
