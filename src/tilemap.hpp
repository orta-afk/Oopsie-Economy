#pragma once

#include "SFML/Graphics/Texture.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>

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

class Tilemap : public sf::Drawable, public sf::Transformable {
public:
  Tilemap();
  void initTilemap();
  void updateTilemap();
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
  int getTileIndex(tiles tile, int tilePerRow);

private:
  tiles tile;
  int tileSize = 16;
  sf::Texture texture;
  sf::VertexArray vert;
  static const int mapWidth = 60;
  static const int mapHeight = 33;
  std::array<std::array<int, mapHeight>, mapWidth> map;

  std::unordered_map<tiles, indexs> mapIndex = {
      {tiles::background, {0, 0}},
      {tiles::topMiddleTile, {19, 0}},
      {tiles::bottomMiddleTile, {19, 2}},
      {tiles::pillertop, {3, 11}},
      {tiles::pillerMiddle, {3, 12}},
      {tiles::pillerBottom, {3, 13}},
  };
};
