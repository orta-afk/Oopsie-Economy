#include "entity.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "collisionLayer.hpp"
#include <vector>

Entity::Entity(){
  initEntity();
  initTexture();
}

void Entity::initEntity(){
  et.size = 16;
  et.XIndex = 26;
  et.YIndex = 0;
  et.rect = sf::IntRect({et.size*et.XIndex, et.size*et.YIndex},{et.size,et.size});

  ed.position = {100,100};
  ed.direction = 0;
  ed.velocity = {0,0};
  ed.speed = 100;
  ed.maxSpeed = 300;
  ed.accelRate = 230;
  ed.friction = 400;
  ed.gravity = 400;
  ed.maxGravity = 500;
}

void Entity::initTexture(){
  if(!et.texture.loadFromFile("../assets/monochrome-transparent_packed.png")){
    std::cerr << "grrr\n";
  } else{
    et.sprite.setTexture(et.texture);
    et.sprite.setTextureRect(et.rect);
    et.sprite.setOrigin(sf::Vector2f(et.size/2.f, et.size/2.f));
    et.texture.setRepeated(false);
    et.texture.setSmooth(false);
    et.sprite.setPosition(ed.position);
  }
}

sf::FloatRect Entity::getBoundingBox(){
  ed.entityBoundingBox = et.sprite.getGlobalBounds();
  return ed.entityBoundingBox;
}

void Entity::setLayer(collisionLayer collisionlayer){
  entityCollisonLayer = collisionlayer;
}

void Entity::setMask(const std::vector<collisionLayer>& m) {
  entityCollisionMask = m;
}

collisionLayer Entity::getLayer(){
  return entityCollisonLayer;
}

std::vector<collisionLayer> Entity::getMask(){
  return entityCollisionMask;
}

void Entity::move(float dt){
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
    ed.direction = -1;    
  }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
    ed.direction = 1;
  }else{
    ed.direction = 0;
  }
  if(ed.direction != 0){
    ed.velocity.x += ed.direction * ed.accelRate * dt;
    if(ed.velocity.x > ed.maxSpeed) ed.velocity.x = ed.maxSpeed;
    if(ed.velocity.x < -ed.maxSpeed) ed.velocity.x = -ed.maxSpeed;
  } else {
    if (ed.velocity.x > 0) {
      ed.velocity.x -= ed.friction * dt;
      if (ed.velocity.x < 0) {
        ed.velocity.x = 0;
      }
    }
    if (ed.velocity.x < 0) {
      ed.velocity.x += ed.friction * dt;
      if (ed.velocity.x > 0) {
        ed.velocity.x = 0;
      }
    }
  }
}

void Entity::gravity(float dt){
  ed.velocity.y += ed.gravity * dt;
  if(ed.velocity.y > ed.maxGravity){
    ed.velocity.y = ed.maxGravity;
  }
}

void Entity::updateEntity(float dt){
  move(dt);
  gravity(dt);
  ed.position += ed.velocity * dt;
  et.sprite.setPosition(ed.position);
  getBoundingBox();
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const{
  target.draw(et.sprite, states);
}

