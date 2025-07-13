#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <vector>

#include "SFML/System/Vector2.hpp"
#include "collisionLayer.hpp"

enum class tiles : int {
  background = 0,
  topMiddleTile,
  bottomMiddleTile,
  pillertop,
  pillerMiddle,
  pillerBottom,
};

struct indexs{
  int x;
  int y;
};

struct tilemapData{
  int tileSize = 16;
  sf::Texture texture;
  sf::VertexArray vert;
  static constexpr int mapWidth = 60;
  static constexpr int mapHeight = 33;
  sf::FloatRect tilemapBoundingBox;

  tilemapData() : vert(sf::PrimitiveType::Triangles){}
};

class Tilemap : public sf::Drawable, public sf::Transformable {
public:
  Tilemap();
  void initTilemap();
  void updateTilemap();
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
  sf::FloatRect getTileBoundBox();

private:
  int getTileIndex(tiles tile, int tilePerRow);

public:
  void setLayer(collisionLayer collisionlayer);
  collisionLayer getLayer();
  void setMask(const std::vector<collisionLayer>& m);
  std::vector<collisionLayer> getMask();

private:
  tiles tile;
  tilemapData td;
  collisionLayer collisionLayerForFuckingTilemap;
  std::vector<collisionLayer> tilemapMask;
  std::array<std::array<int, tilemapData::mapHeight>, tilemapData::mapWidth> map;
  std::unordered_map<tiles, indexs> mapIndex = {
      {tiles::background, {0, 0}},
      {tiles::topMiddleTile, {19, 0}},
      {tiles::bottomMiddleTile, {19, 2}},
      {tiles::pillertop, {3, 11}},
      {tiles::pillerMiddle, {3, 12}},
      {tiles::pillerBottom, {3, 13}},
  };
};
