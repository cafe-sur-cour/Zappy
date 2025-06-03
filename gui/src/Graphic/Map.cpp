/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Map
*/

#include <memory>
#include "RayLib/RayLib.hpp"
#include "Map.hpp"

Map::Map(std::shared_ptr<GameInfos> gameInfos, std::shared_ptr<RayLib> raylib)
    : _gameInfos(std::move(gameInfos)), _raylib(raylib)
{
}

Map::~Map()
{
}

void Map::draw()
{
    auto tiles = _gameInfos->getTiles();

    for (const auto &tile : tiles) {
        drawTile(tile.x, tile.y, tile);
    }
}

void Map::drawTile(int x, int y, const zappy::structs::Tile &tile)
{
    Vector3 position = {static_cast<float>(x), 0.0f, static_cast<float>(y)};

    (void)tile;
    _raylib->drawCube(position, 0.9f, 0.2f, 0.9f, LIGHTGRAY);
    _raylib->drawCubeWires(position, 0.9f, 0.2f, 0.9f, BLACK);
}
