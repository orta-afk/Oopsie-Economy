#include "tilemap.hpp"

Tilemap::Tilemap() : vert(sf::PrimitiveType::Triangles) {
  initTilemap();
}

void Tilemap::initTilemap() {
  if (!texture.loadFromFile("../assets/monochrome-transparent_packed.png")) {
    std::cerr << "fuck man!!\n";
  }

  const int boxTop = 10;
  const int boxBottom = 22;
  const int leftCol = 5;
  const int rightCol = mapWidth - 6;

  int tilePerRow = texture.getSize().x / tileSize;

  for (int i = 0; i < mapWidth; ++i) {
    for (int j = 0; j < mapHeight; ++j) {
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

      // SAFELY store in map[i][j] (i = width, j = height)
      map[i][j] = tile;
    }
  }
}

int Tilemap::getTileIndex(tiles tile, int tilePerRow) {
  auto [x, y] = mapIndex.at(tile);
  return x + y * tilePerRow;
}

void Tilemap::updateTilemap() {
  vert.clear();
  vert.setPrimitiveType(sf::PrimitiveType::Triangles);

  // ✅ Corrected vertex count!
  vert.resize(mapWidth * mapHeight * 6);

  for (int i = 0; i < mapWidth; ++i) {
    for (int j = 0; j < mapHeight; ++j) {
      const int tileNumber = map[i][j];
      const int tu = tileNumber % (texture.getSize().x / tileSize);
      const int tv = tileNumber / (texture.getSize().x / tileSize);

      sf::Vertex* triangles = &vert[(i + j * mapWidth) * 6];

      triangles[0].position = sf::Vector2f(i * tileSize, j * tileSize);
      triangles[1].position = sf::Vector2f((i + 1) * tileSize, j * tileSize);
      triangles[2].position = sf::Vector2f(i * tileSize, (j + 1) * tileSize);
      triangles[3].position = sf::Vector2f(i * tileSize, (j + 1) * tileSize);
      triangles[4].position = sf::Vector2f((i + 1) * tileSize, j * tileSize);
      triangles[5].position = sf::Vector2f((i + 1) * tileSize, (j + 1) * tileSize);

      triangles[0].texCoords = sf::Vector2f(tu * tileSize, tv * tileSize);
      triangles[1].texCoords = sf::Vector2f((tu + 1) * tileSize, tv * tileSize);
      triangles[2].texCoords = sf::Vector2f(tu * tileSize, (tv + 1) * tileSize);
      triangles[3].texCoords = sf::Vector2f(tu * tileSize, (tv + 1) * tileSize);
      triangles[4].texCoords = sf::Vector2f((tu + 1) * tileSize, tv * tileSize);
      triangles[5].texCoords = sf::Vector2f((tu + 1) * tileSize, (tv + 1) * tileSize);
    }
  }
}

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  states.texture = &texture;
  target.draw(vert, states);
}
