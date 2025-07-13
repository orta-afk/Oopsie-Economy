#include "tilemap.hpp"

Tilemap::Tilemap() { initTilemap(); }

void Tilemap::initTilemap() {
  if (!td.texture.loadFromFile("../assets/monochrome-transparent_packed.png")) {
    std::cerr << "fuck man!!\n";
  }

  const int boxTop = 10;
  const int boxBottom = 22;
  const int leftCol = 5;
  const int rightCol = td.mapWidth - 6;

  int tilePerRow = td.texture.getSize().x / td.tileSize;

  for (int i = 0; i < td.mapWidth; ++i) {
    for (int j = 0; j < td.mapHeight; ++j) {
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
      map[i][j] = tile;
    }
  }
}

int Tilemap::getTileIndex(tiles tile, int tilePerRow) {
  auto [x, y] = mapIndex.at(tile);
  return x + y * tilePerRow;
}

void Tilemap::setLayer(collisionLayer collisionlayer){
  collisionLayerForFuckingTilemap = collisionlayer;
}

void Tilemap::setMask(const std::vector<collisionLayer>& m) {
  tilemapMask = m;
}

collisionLayer Tilemap::getLayer(){
  return collisionLayerForFuckingTilemap;
}

std::vector<collisionLayer> Tilemap::getMask(){
  return tilemapMask;
}

sf::FloatRect Tilemap::getTileBoundBox(){
  td.tilemapBoundingBox = sf::FloatRect({0, 0}, {static_cast<float>(td.mapWidth)* td.tileSize, static_cast<float>(td.mapHeight) * td.tileSize});
  return td.tilemapBoundingBox;
}

void Tilemap::updateTilemap() {
  td.vert.clear();
  td.vert.setPrimitiveType(sf::PrimitiveType::Triangles);
  td.vert.resize(td.mapWidth * td.mapHeight * 6);

  for (int i = 0; i < td.mapWidth; ++i) {
    for (int j = 0; j < td.mapHeight; ++j) {
      const int tileNumber = map[i][j];
      const int tu = tileNumber % (td.texture.getSize().x / td.tileSize);
      const int tv = tileNumber / (td.texture.getSize().x / td.tileSize);
      sf::Vertex *triangles = &td.vert[(i + j * td.mapWidth) * 6];
      triangles[0].position = sf::Vector2f(i * td.tileSize, j * td.tileSize);
      triangles[1].position = sf::Vector2f((i + 1) * td.tileSize, j * td.tileSize);
      triangles[2].position = sf::Vector2f(i * td.tileSize, (j + 1) * td.tileSize);
      triangles[3].position = sf::Vector2f(i * td.tileSize, (j + 1) * td.tileSize);
      triangles[4].position = sf::Vector2f((i + 1) * td.tileSize, j * td.tileSize);
      triangles[5].position = sf::Vector2f((i + 1) * td.tileSize, (j + 1) * td.tileSize);

      triangles[0].texCoords = sf::Vector2f(tu * td.tileSize, tv * td.tileSize);
      triangles[1].texCoords = sf::Vector2f((tu + 1) * td.tileSize, tv * td.tileSize);
      triangles[2].texCoords = sf::Vector2f(tu * td.tileSize, (tv + 1) * td.tileSize);
      triangles[3].texCoords = sf::Vector2f(tu * td.tileSize, (tv + 1) * td.tileSize);
      triangles[4].texCoords = sf::Vector2f((tu + 1) * td.tileSize, tv * td.tileSize);
      triangles[5].texCoords = sf::Vector2f((tu + 1) * td.tileSize, (tv + 1) * td.tileSize);
    }
  }
}

void Tilemap::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform *= getTransform();
  states.texture = &td.texture;
  target.draw(td.vert, states);
}
