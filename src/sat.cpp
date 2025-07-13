#include "sat.hpp"
#include "SFML/System/Vector2.hpp"
#include "collisionLayer.hpp"
#include <vector>

void Sat::initSat(){
  entity.setLayer(collisionLayer::entity);
  entity.setMask({collisionLayer::tilemap});

  tilemap.setLayer(collisionLayer::tilemap);
  tilemap.setMask({collisionLayer::entity});
  
  entityCollisionLayer = entity.getLayer();
  tilemapCollisionLayer = tilemap.getLayer();

  entityMask = entity.getMask();
  tilemapMask = tilemap.getMask();
  
  entityBox = entity.getBoundingBox();
  tilemapBox = tilemap.getTileBoundBox();
  entityPoints = getPoints(entityBox);
  tilemapPoints = getPoints(tilemapBox);

  entityNormals = getNormals(entityPoints);
  tilemapNormals = getNormals(tilemapPoints);
}

std::vector<sf::Vector2f> Sat::getPoints(sf::FloatRect& rect){
  return {
    {rect.position.x, rect.position.y},                                         
    {rect.position.x + rect.size.x, rect.position.y},                          
    {rect.position.x + rect.size.x, rect.position.y + rect.size.y},            
    {rect.position.x, rect.position.y + rect.size.y}                           
  };
}

std::pair<float, float> Sat::Project(std::vector<sf::Vector2f>& points, sf::Vector2f& axis){
  float min = points[0].dot(axis);
  float max = min;
  for(auto &p : points){
    float proj = p.dot(axis);
    if (proj < min) min = proj;
    if (proj > max) max = proj;
  }
  return {min,max};
}

std::vector<sf::Vector2f> Sat::getNormals(const std::vector<sf::Vector2f>& points){
  std::vector<sf::Vector2f> normals;
  for(int i = 0; i < points.size(); i++){
    sf::Vector2f p = points[i];
    sf::Vector2f p1 = points[(i + 1) % points.size()];
    sf::Vector2f edge = p1 - p;

    if (edge != sf::Vector2f(0.f, 0.f)) {
      normals.push_back(edge.perpendicular().normalized());
    }
  }
  return normals;
}


bool Sat::collided() {
  std::vector<sf::Vector2f> axes = entityNormals;
  axes.insert(axes.end(), tilemapNormals.begin(), tilemapNormals.end());

  for (auto& axis : axes) {
    auto [minA, maxA] = Project(entityPoints, axis);
    auto [minB, maxB] = Project(tilemapPoints, axis);

    if (maxA < minB || maxB < minA) {
      return false; 
    }
  }

  return true;
}
