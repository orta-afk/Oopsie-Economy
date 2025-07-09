#include "entity.hpp"

Entity::Entity(){
  initEntity();
  initTexture();
}

void Entity::initEntity(){
  ed.position = {100,100};
  ed.velocity = {0,0};
  et.size = 16;
  et.XIndex = 26;
  et.YIndex = 0;
  et.rect = sf::IntRect({et.size*et.XIndex, et.size*et.YIndex},{et.size,et.size});
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

void Entity::updateEntity(){}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const{
  target.draw(et.sprite);
}

