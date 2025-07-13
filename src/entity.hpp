#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "collisionLayer.hpp"


struct entityData{
  sf::Vector2f position;
  sf::Vector2f velocity; 
  int direction;
  int speed;
  int maxSpeed;
  int friction;
  int accelRate;
  int gravity;
  int maxGravity;
  sf::FloatRect entityBoundingBox;
};

struct entityTexture{
  int XIndex;
  int YIndex;
  int size;
  sf::Texture texture;
  sf::Sprite sprite;
  sf::IntRect rect;

  entityTexture() : sprite(texture){};
};

class Entity : public sf::Drawable, public sf::Transformable{
public:
  Entity();
  void initEntity();
  void updateEntity(float dt, const sf::Vector2f& mtv);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  sf::FloatRect getBoundingBox();

private:
  void initTexture();
  void move(float dt);
  void gravity(float dt);

public:
  void setLayer(collisionLayer collisionlayer);
  collisionLayer getLayer();
  void setMask(const std::vector<collisionLayer>& m);
  std::vector<collisionLayer> getMask();

private:
  collisionLayer entityCollisonLayer;
  std::vector<collisionLayer> entityCollisionMask;
  entityData ed;
  entityTexture et;
};
