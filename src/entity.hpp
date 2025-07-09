#pragma once

#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

struct entityData{
  sf::Vector2f position;
  sf::Vector2f velocity; 
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
  void updateEntity();
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  void initTexture();

private:
  entityData ed;
  entityTexture et;
};
