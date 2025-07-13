#pragma once

#include <SFML/Graphics.hpp>
#include <utility>

#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"
#include "entity.hpp"
#include "tilemap.hpp"
#include "collisionLayer.hpp"

class Sat {
public:
  void initSat();
  bool collided();
  
private:
  std::vector<sf::Vector2f> getPoints(sf::FloatRect& rect);
  std::vector<sf::Vector2f> getNormals(const std::vector<sf::Vector2f>& points);
  std::pair<float, float> Project(std::vector<sf::Vector2f>& points, sf::Vector2f& axis);

private:
  Entity entity;
  Tilemap tilemap;
  sf::FloatRect entityBox;
  sf::FloatRect tilemapBox;
  collisionLayer entityCollisionLayer;
  collisionLayer tilemapCollisionLayer;
  std::vector<collisionLayer> entityMask;
  std::vector<collisionLayer> tilemapMask;
  std::vector<sf::Vector2f> entityPoints;
  std::vector<sf::Vector2f> tilemapPoints;
  std::vector<sf::Vector2f> entityNormals;
  std::vector<sf::Vector2f> tilemapNormals;
};
