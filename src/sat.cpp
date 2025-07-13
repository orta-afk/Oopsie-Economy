#include "sat.hpp"
#include "SFML/System/Vector2.hpp"
#include "collisionLayer.hpp"

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

sf::Vector2f Sat::getCenter(const std::vector<sf::Vector2f>& points){
  sf::Vector2f sum = {0,0};
  for(auto& p: points){
    sum += p;
  }
  return sum / static_cast<float>(points.size());
}

std::optional<sf::Vector2f> Sat::collided(){
  float smallestOverlap = std::numeric_limits<float>::infinity();
  sf::Vector2f smallestAxis;
  std::vector<sf::Vector2f>axis = entityNormals;
  axis.insert(axis.end(), tilemapNormals.begin(), tilemapNormals.end());
  for(auto&p : axis){
    auto[minA, maxA] = Project(entityPoints, p);
    auto[minB, maxB] = Project(tilemapPoints, p);
    if (maxA < minB || maxB < minA){
      return std::nullopt;
    }
    float overlap = std::max(maxA, maxB) - std::min(minA, minB);
    if(overlap < smallestOverlap){
      smallestOverlap = overlap;
      smallestAxis = p;
    }
    sf::Vector2f EntityCenter = getCenter(entityPoints);
    sf::Vector2f TilemapCenter = getCenter(tilemapPoints);
    sf::Vector2f direction = EntityCenter - TilemapCenter;
    if(direction.dot(smallestAxis) < 0){
      smallestAxis = -smallestAxis;
    }
  }
  return smallestAxis * smallestOverlap;
}
