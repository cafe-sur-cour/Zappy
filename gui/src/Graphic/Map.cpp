/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Map
*/

#include <memory>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <string>

#include "RayLib/RayLib.hpp"
#include "Map.hpp"

Map::Map(std::shared_ptr<GameInfos> gameInfos, std::shared_ptr<RayLib> raylib)
    : _gameInfos(std::move(gameInfos)), _raylib(raylib)
{
}

Map::~Map()
{
}

Color Map::getTeamColor(const std::string &teamName)
{
    if (_teamColors.find(teamName) == _teamColors.end()) {
        unsigned int seed = static_cast<unsigned int>(time(nullptr));
        _teamColors[teamName] = {
            static_cast<unsigned char>(rand_r(&seed) % 200 + 55),
            static_cast<unsigned char>(rand_r(&seed) % 200 + 55),
            static_cast<unsigned char>(rand_r(&seed) % 200 + 55),
            255
        };
    }
    return _teamColors[teamName];
}

void Map::draw()
{
    auto tiles = _gameInfos->getTiles();

    for (const auto &tile : tiles) {
        drawTile(tile.x, tile.y, tile);
        drawPlayers(tile.x, tile.y);
    }
}

void Map::drawTile(int x, int y, const zappy::structs::Tile &tile)
{
    Vector3 position = {static_cast<float>(x), 0.0f, static_cast<float>(y)};

    (void)tile;
    _raylib->drawCube(position, 0.9f, 0.2f, 0.9f, LIGHTGRAY);
    _raylib->drawCubeWires(position, 0.9f, 0.2f, 0.9f, BLACK);
}

void Map::drawPlayers(int x, int y)
{
    const auto& players = _gameInfos->getPlayers();
    std::vector<const zappy::structs::Player*> playersOnTile;

    for (const auto& player : players) {
        if (player.x == x && player.y == y) {
            playersOnTile.push_back(&player);
        }
    }

    float cylinderHeight = 0.4f;
    float cylinderRadius = 0.25f;
    float baseHeight = 0.1f;

    for (size_t i = 0; i < playersOnTile.size(); ++i) {
        Vector3 position = {
            static_cast<float>(x),
            baseHeight + cylinderHeight * (i + 0.5f),
            static_cast<float>(y)
        };

        Color teamColor = getTeamColor(playersOnTile[i]->teamName);
        _raylib->drawCylinder(position, cylinderRadius, cylinderRadius,
                             cylinderHeight, 12, teamColor);
        _raylib->drawCylinderWires(position, cylinderRadius, cylinderRadius,
                                  cylinderHeight, 12, BLACK);
        drawOrientationArrow(position, playersOnTile[i]->orientation,
            cylinderHeight);
    }
}

void Map::drawOrientationArrow(const Vector3 &position, int orientation, float playerHeight)
{
    float arrowLength = 0.35f;
    float arrowWidth = 0.15f;
    float arrowHeadSize = 0.2f;

    Vector3 arrowPos = position;
    arrowPos.y += playerHeight / 2 + 0.15f;
    Vector3 start = arrowPos;
    Vector3 end = arrowPos;

    int direction = orientation - 1;
    if (direction < 0) direction = 0;

    switch (direction) {
        case 0: end.z -= arrowLength; break;
        case 1: end.x += arrowLength; break;
        case 2: end.z += arrowLength; break;
        case 3: end.x -= arrowLength; break;
    }
    _raylib->drawCylinderEx(start, end, 0.05f, 0.05f, 8, RED);

    Vector3 coneStart = end;
    Vector3 coneEnd;

    switch (direction) {
        case 0: coneEnd = {end.x, end.y, end.z - arrowHeadSize}; break;
        case 1: coneEnd = {end.x + arrowHeadSize, end.y, end.z}; break;
        case 2: coneEnd = {end.x, end.y, end.z + arrowHeadSize}; break;
        case 3: coneEnd = {end.x - arrowHeadSize, end.y, end.z}; break;
    }
    _raylib->drawCylinderEx(coneStart, coneEnd, arrowWidth, 0.0f, 8, RED);
}
