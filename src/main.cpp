#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <array>

#define TILESIZE 16

struct window {
  unsigned int width = 960;
  unsigned int height = 528;
  const char *title = "Oopsie Economy";
};

struct EntityStuff {
  sf::Vector2f velocity = {0.f, 0.f};
  sf::Vector2f positions;
  bool onGround = false;
  float g = 1200.f;
  float jumpVelocity = 300.f;
  float max_g = 2000.f;
  float speed = 250.f;
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

class Entity : public sf::Drawable, public sf::Transformable {
public:
  Entity() : Entitysprite(Entitytexture) { initEntity(); }

  void initEntity() {
    sf::IntRect rect({entityStuff.XIndex * entityStuff.spriteSize,
                      entityStuff.YIndex * entityStuff.spriteSize},
                     {entityStuff.spriteSize, entityStuff.spriteSize});

    if (!Entitytexture.loadFromFile("../assets/colored-transparent_packed.png")) {
      std::cout << "Failed to load texture\n";
    } else {
      Entitysprite.setTexture(Entitytexture);
      Entitysprite.setTextureRect(rect);
      entityStuff.positions = {(5 + (54.0f - 5) / 2) * TILESIZE, (10 + (22.0f - 10) / 2) * TILESIZE};
      Entitysprite.setPosition({entityStuff.positions});
    }
  }

  void updateEntity(float dt, const sf::Vector2f& mtv) {
    jump(dt);
    move(dt);
    gravity(dt);
    resolveCollision(mtv);
    entityStuff.positions += entityStuff.velocity * dt;
    Entitysprite.setPosition(entityStuff.positions);
  }

  sf::FloatRect entityBounds() const { return Entitysprite.getGlobalBounds(); }

private:
  void resolveCollision(const sf::Vector2f& mtv) {
    if (mtv != sf::Vector2f(0.f, 0.f)) {
      entityStuff.positions += mtv;

      if (std::abs(mtv.x) > 0.f) {
        entityStuff.velocity.x = 0.f;
      }
      if (mtv.y < 0.f) { // hitting the ground
        entityStuff.velocity.y = 0.f;
        entityStuff.onGround = true;
        entityStuff.canJump = true;
      }
    } else {
      entityStuff.onGround = false;
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
          if (i == leftCol || i == rightCol)
            tile = getTileIndex(tiles::pillertop, tilePerRow);
          else if (i > leftCol && i < rightCol)
            tile = getTileIndex(tiles::bottomMiddleTile, tilePerRow);
        } else if (j > boxTop && j < boxBottom) {
          if (i == leftCol || i == rightCol)
            tile = getTileIndex(tiles::pillerMiddle, tilePerRow);
        } else if (j == boxBottom) {
          if (i == leftCol || i == rightCol)
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

        sf::Vector2f pos(i * TILESIZE, j * TILESIZE);

        triangles[0].position = pos;
        triangles[1].position = pos + sf::Vector2f(TILESIZE, 0);
        triangles[2].position = pos + sf::Vector2f(0, TILESIZE);
        triangles[3].position = triangles[2].position;
        triangles[4].position = triangles[1].position;
        triangles[5].position = pos + sf::Vector2f(TILESIZE, TILESIZE);

        triangles[0].texCoords = sf::Vector2f(tu * TILESIZE, tv * TILESIZE);
        triangles[1].texCoords = triangles[0].texCoords + sf::Vector2f(TILESIZE, 0);
        triangles[2].texCoords = triangles[0].texCoords + sf::Vector2f(0, TILESIZE);
        triangles[3].texCoords = triangles[2].texCoords;
        triangles[4].texCoords = triangles[1].texCoords;
        triangles[5].texCoords = triangles[0].texCoords + sf::Vector2f(TILESIZE, TILESIZE);
      }
    }
  }

  int getTileIndex(tiles t, int tilesPerRow) const {
    indexs coords = mapIndex.at(t);
    return coords.x + coords.y * tilesPerRow;
  }

  int getTileperRow() const { return texture.getSize().x / TILESIZE; }

  int getTileAt(int x, int y) const { return map[y][x]; }
  int getMapWidth() const { return mapWidth; }
  int getMapHeight() const { return mapHeight; }

private:
  const static int mapWidth = 60;
  const static int mapHeight = 33;
  sf::Texture texture;
  sf::VertexArray vert;
  std::array<std::array<int, mapWidth>, mapHeight> map;
  std::unordered_map<tiles, indexs> mapIndex = {
    {tiles::background, {0, 0}},
    {tiles::topMiddleTile, {19, 0}},
    {tiles::bottomMiddleTile, {19, 2}},
    {tiles::pillertop, {3, 11}},
    {tiles::pillerMiddle, {3, 12}},
    {tiles::pillerBottom, {3, 13}},
  };

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    states.transform *= getTransform();
    states.texture = &texture;
    target.draw(vert, states);
  }
};

sf::Vector2f collided(const Entity &e, const Tilemap &t) {
  sf::Vector2f finalMTV(0.f, 0.f);
  float minOverlap = std::numeric_limits<float>::max();
  auto entityRect = e.entityBounds();

  for (int j = 0; j < t.getMapHeight(); ++j) {
    for (int i = 0; i < t.getMapWidth(); ++i) {
      int TileIndex = t.getTileAt(i, j);
      int tileperRow = t.getTileperRow();

      if (TileIndex == t.getTileIndex(tiles::background, tileperRow)) continue;

      sf::FloatRect tileRect(
        sf::Vector2f(i * TILESIZE, j * TILESIZE),
        sf::Vector2f(TILESIZE, TILESIZE)
      );

      auto intersect = entityRect.findIntersection(tileRect);
      if (intersect.has_value()) {
        sf::Vector2f overlap = intersect->size;

        if (overlap.x < overlap.y) {
          float direction = (entityRect.position.x < tileRect.position.x) ? -1.f : 1.f;
          if (overlap.x < minOverlap) {
            finalMTV = {overlap.x * direction, 0.f};
            minOverlap = overlap.x;
          }
        } else {
          float direction = (entityRect.position.y < tileRect.position.y) ? -1.f : 1.f;
          if (overlap.y < minOverlap) {
            finalMTV = {0.f, overlap.y * direction};
            minOverlap = overlap.y;
          }
        }
      }
    }
  }

  return finalMTV;
}

int main() {
  window win;
  Entity entity;
  Tilemap tilemap;

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
    sf::Vector2f mtv = collided(entity, tilemap);
    entity.updateEntity(dt, mtv);

    while (const std::optional<sf::Event> event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    window.clear();
    window.draw(tilemap);
    window.draw(entity);
    window.display();
  }
}
