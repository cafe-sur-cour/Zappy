/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GameInfos
*/

#include <utility>
#include <vector>

#include "GameInfos.hpp"

GameInfos::GameInfos() :
    _mapWidth(0),
    _mapHeight(0)
{
}

GameInfos::~GameInfos()
{
}

void GameInfos::setMapSize(int width, int height)
{
    _mapWidth = width;
    _mapHeight = height;
}

std::pair<int, int> GameInfos::getMapSize() const
{
    return std::make_pair(_mapWidth, _mapHeight);
}

void GameInfos::setTimeUnit(int timeUnit)
{
    _timeUnit = timeUnit;
}

int GameInfos::getTimeUnit() const
{
    return _timeUnit;
}

void GameInfos::updateTile(zappy::structs::Tile tile)
{
    if (tile.x < 0 || tile.y < 0 || tile.x >= _mapWidth || tile.y >= _mapHeight)
        return;

    for (auto &existingTile : _tiles) {
        if (existingTile.x == tile.x && existingTile.y == tile.y) {
            existingTile = tile;
            return;
        }
    }
    _tiles.push_back(tile);
}

const std::vector<zappy::structs::Tile> GameInfos::getTiles() const
{
    return _tiles;
}

const zappy::structs::Tile GameInfos::getTile(int x, int y) const
{
    for (const auto &tile : _tiles) {
        if (tile.x == x && tile.y == y)
            return tile;
    }

    return zappy::structs::Tile(x, y);
}
