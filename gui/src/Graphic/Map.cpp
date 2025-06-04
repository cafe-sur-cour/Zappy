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
        drawEggs(tile.x, tile.y);
        drawPlayers(tile.x, tile.y);
        drawFood(tile.x, tile.y, tile);
        drawRock(tile.x, tile.y, tile);
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

    if (playersOnTile.empty())
        return;

    float cylinderHeight = 0.4f;
    float cylinderRadius = 0.25f;

    for (size_t i = 0; i < playersOnTile.size(); ++i) {
        Vector3 position = {
            static_cast<float>(x),
            getOffset(DisplayPriority::PLAYER, x, y, i),
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

void Map::drawEggs(int x, int y)
{
    const auto& eggs = _gameInfos->getEggs();
    std::vector<const zappy::structs::Egg*> eggsOnTile;

    for (const auto& egg : eggs) {
        if (egg.x == x && egg.y == y && !egg.hatched) {
            eggsOnTile.push_back(&egg);
        }
    }

    float eggRadius = 0.2f;

    for (size_t i = 0; i < eggsOnTile.size(); ++i) {
        Vector3 position = {
            static_cast<float>(x),
            getOffset(DisplayPriority::EGG, x, y, i),
            static_cast<float>(y)
        };

        Color teamColor = getTeamColor(eggsOnTile[i]->teamName);
        _raylib->drawSphere(position, eggRadius, teamColor);
        _raylib->drawSphereWires(position, eggRadius, 8, 8, BLACK);
    }
}

void Map::drawFood(int x, int y, const zappy::structs::Tile &tile)
{
    if (tile.food <= 0)
        return;

    Color foodColor = BROWN;
    float foodSize = 0.25f;

    for (int i = 0; i < tile.food; ++i) {
        Vector3 position = {
            static_cast<float>(x),
            getOffset(DisplayPriority::FOOD, x, y, static_cast<size_t>(i)),
            static_cast<float>(y)
        };

        _raylib->drawCube(position, foodSize, foodSize, foodSize, foodColor);
        _raylib->drawCubeWires(position, foodSize, foodSize, foodSize, BLACK);
    }
}

void Map::drawRock(int x, int y, const zappy::structs::Tile &tile)
{
    if (tile.linemate <= 0 && tile.deraumere <= 0 && tile.sibur <= 0 &&
        tile.mendiane <= 0 && tile.phiras <= 0 && tile.thystame <= 0)
        return;

    Color rockColor = BLUE;
    float foodSize = 0.25f;

    for (int i = 0; i < tile.linemate + tile.deraumere + tile.sibur + tile.mendiane +
            tile.phiras + tile.thystame; ++i) {
        Vector3 position = {
            static_cast<float>(x),
            getOffset(DisplayPriority::ROCK, x, y, static_cast<size_t>(i)),
            static_cast<float>(y)
        };

        _raylib->drawCube(position, foodSize, foodSize, foodSize, rockColor);
        _raylib->drawCubeWires(position, foodSize, foodSize, foodSize, BLACK);
    }
}

float Map::getOffset(DisplayPriority priority, int x, int y, size_t stackIndex)
{
    switch (priority) {
        case DisplayPriority::TILE:
            return BASE_HEIGHT_TILE;

        case DisplayPriority::EGG:
            return BASE_HEIGHT_EGG + (stackIndex * EGG_HEIGHT);

        case DisplayPriority::PLAYER: {
            const auto& eggs = _gameInfos->getEggs();
            size_t eggCount = 0;

            for (const auto& egg : eggs) {
                if (egg.x == x && egg.y == y && !egg.hatched) {
                    eggCount++;
                }
            }

            float basePlayerHeight = BASE_HEIGHT_PLAYER;
            if (eggCount > 0)
                basePlayerHeight = BASE_HEIGHT_EGG + (eggCount * EGG_HEIGHT);

            return basePlayerHeight + (stackIndex * PLAYER_HEIGHT);
        }

        case DisplayPriority::FOOD: {
            const auto& eggs = _gameInfos->getEggs();
            size_t eggCount = 0;

            for (const auto& egg : eggs) {
                if (egg.x == x && egg.y == y && !egg.hatched) {
                    eggCount++;
                }
            }

            const auto& players = _gameInfos->getPlayers();
            size_t playerCount = 0;

            for (const auto& player : players) {
                if (player.x == x && player.y == y) {
                    playerCount++;
                }
            }

            float baseFoodHeight = BASE_HEIGHT_FOOD;
            if (eggCount > 0)
                baseFoodHeight = BASE_HEIGHT_EGG + (eggCount * EGG_HEIGHT);

            if (playerCount > 0)
                baseFoodHeight += (playerCount * PLAYER_HEIGHT);

            return baseFoodHeight + (stackIndex * FOOD_HEIGHT);
        }

        case DisplayPriority::ROCK: {
            const auto& eggs = _gameInfos->getEggs();
            size_t eggCount = 0;

            for (const auto& egg : eggs) {
                if (egg.x == x && egg.y == y && !egg.hatched) {
                    eggCount++;
                }
            }

            const auto& players = _gameInfos->getPlayers();
            size_t playerCount = 0;

            for (const auto& player : players) {
                if (player.x == x && player.y == y) {
                    playerCount++;
                }
            }

            const auto& tiles = _gameInfos->getTiles();
            size_t rockCount = 0;
            for (const auto& tile : tiles) {
                if (tile.x == x && tile.y == y &&
                    (tile.linemate > 0 || tile.deraumere > 0 ||
                     tile.sibur > 0 || tile.mendiane > 0 ||
                     tile.phiras > 0 || tile.thystame > 0)) {
                    rockCount++;
                }
            }

            float baseRockHeight = BASE_HEIGHT_ROCK;
            if (eggCount > 0)
                baseRockHeight = BASE_HEIGHT_EGG + (eggCount * EGG_HEIGHT);
            if (playerCount > 0)
                baseRockHeight += (playerCount * PLAYER_HEIGHT);
            if (rockCount > 0)
                baseRockHeight += (rockCount * ROCK_HEIGHT);
            return baseRockHeight + (stackIndex * ROCK_HEIGHT);
        }

        default:
            return 0.5f + (stackIndex * 0.2f);
    }
}
