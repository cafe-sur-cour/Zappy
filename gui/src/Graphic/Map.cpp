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
#include <cmath>
#include <iostream>
#include <chrono>

#include "Map.hpp"

Map::Map(std::shared_ptr<GameInfos> gameInfos, std::shared_ptr<IDisplay> display)
    : _gameInfos(std::move(gameInfos)), _display(display)
{
    _colors = {CBLUE, CYELLOW, CPURPLE, CORANGE, CPINK, CMAROON, CRED, CGREEN};
}

Map::~Map()
{
}

Color32 Map::getTeamColor(const std::string &teamName)
{
    if (teamName.empty())
        return CWHITE;

    if (_teamColors.find(teamName) == _teamColors.end()) {
        _teamColors[teamName] = _colors[_colorIndex];
        _colorIndex = (_colorIndex + 1) % _colors.size();
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
    drawBroadcastingPlayers();
}

void Map::drawTile(int x, int y, const zappy::structs::Tile &tile)
{
    Vector3f position = {static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
        0.0f, static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)};

    (void)tile;
    int seed = static_cast<int>(x * 73856093 ^ y * 19349663);
    float angle = static_cast<float>((seed % 10) - 5);

    float offsetSeed = static_cast<float>((x * 2654435761u) ^ (y * 1597334677u));
    float offsetX = (static_cast<float>(std::sin(offsetSeed)) * 0.08f);
    float offsetZ = (static_cast<float>(std::cos(offsetSeed)) * 0.08f);
    position.x += offsetX;
    position.z += offsetZ;

    this->_display->drawModelEx("platform", position, {0.0f, 1.0f, 0.0f},
        angle, {0.9f, 0.9f, 0.9f}, CWHITE);
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

    for (size_t i = 0; i < playersOnTile.size(); ++i) {
        Vector3f position = {
            static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
            getOffset(DisplayPriority::PLAYER, x, y, i),
            static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)
        };

        Color32 teamColor = getTeamColor(playersOnTile[i]->teamName);
        float rotationAngle = 0.0f;
        int direction = playersOnTile[i]->orientation - 1;

        if (direction < 0) direction = 0;
        switch (direction) {
            case 0: rotationAngle = 180.0f; break;
            case 1: rotationAngle = 90.0f; break;
            case 2: rotationAngle = 0.0f; break;
            case 3: rotationAngle = 270.0f; break;
        }

        this->_display->drawModelEx("player", position, {0.0f, 1.0f, 0.0f},
            rotationAngle, {zappy::gui::PLAYER_SCALE, zappy::gui::PLAYER_SCALE,
                zappy::gui::PLAYER_SCALE}, teamColor);
        drawOrientationArrow(position, playersOnTile[i]->orientation,
            cylinderHeight);
    }
}

void Map::drawOrientationArrow(const Vector3f &position, int orientation, float playerHeight)
{
    float arrowLength = 0.35f;
    float arrowWidth = 0.15f;
    float arrowHeadSize = 0.2f;

    Vector3f arrowPos = position;
    arrowPos.y += playerHeight / 2 + 0.15f;
    Vector3f start = arrowPos;
    Vector3f end = arrowPos;

    int direction = orientation - 1;
    if (direction < 0) direction = 0;

    switch (direction) {
        case 0: end.z -= arrowLength; break;
        case 1: end.x += arrowLength; break;
        case 2: end.z += arrowLength; break;
        case 3: end.x -= arrowLength; break;
    }
    this->_display->drawCylinderEx(start, end, 0.05f, 0.05f, 8, CRED);

    Vector3f coneStart = end;
    Vector3f coneEnd;

    switch (direction) {
        case 0: coneEnd = {end.x, end.y, end.z - arrowHeadSize}; break;
        case 1: coneEnd = {end.x + arrowHeadSize, end.y, end.z}; break;
        case 2: coneEnd = {end.x, end.y, end.z + arrowHeadSize}; break;
        case 3: coneEnd = {end.x - arrowHeadSize, end.y, end.z}; break;
    }
    this->_display->drawCylinderEx(coneStart, coneEnd, arrowWidth, 0.0f, 8, CRED);
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
        Vector3f position = {
            static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
            getOffset(DisplayPriority::EGG, x, y, i),
            static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)
        };

        Color32 teamColor = getTeamColor(eggsOnTile[i]->teamName);
        this->_display->drawSphere(position, eggRadius, teamColor);
        this->_display->drawSphereWires(position, eggRadius, 8, 8, CBLACK);
    }
}

void Map::drawFood(int x, int y, const zappy::structs::Tile &tile)
{
    if (tile.food <= 0)
        return;

    for (int i = 0; i < tile.food; ++i) {
        Vector3f position = {
            static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
            getOffset(DisplayPriority::FOOD, x, y, static_cast<size_t>(i)),
            static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)
        };

        this->_display->drawModelEx("food", position, {0.0f, 1.0f, 0.0f},
            0.0f, {0.005f, 0.005f, 0.005f}, CWHITE);
    }
}

void Map::drawRock(int x, int y, const zappy::structs::Tile &tile)
{
    if (tile.linemate <= 0 && tile.deraumere <= 0 && tile.sibur <= 0 &&
        tile.mendiane <= 0 && tile.phiras <= 0 && tile.thystame <= 0)
        return;

    for (int i = 0; i < tile.linemate + tile.deraumere + tile.sibur + tile.mendiane +
            tile.phiras + tile.thystame; ++i) {
        Vector3f position = {
            static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
            getOffset(DisplayPriority::ROCK, x, y, static_cast<size_t>(i)),
            static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)
        };

        this->_display->drawModelEx("rock", position, {0.0f, 1.0f, 0.0f},
            0.0f, {0.3f, 0.3f, 0.3f}, CWHITE);
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

void Map::drawBroadcastingPlayers()
{
    auto currentTime = std::chrono::steady_clock::now();
    const float ANIMATION_DURATION = 2.0f;
    const int NUM_RINGS = 3;
    const float RING_DELAY = 0.5f;

    const auto& newBroadcasts = _gameInfos->getPlayersBroadcasting();

    for (const auto& broadcast : newBroadcasts) {
        if (_broadcastStartTimes.find(broadcast.first) == _broadcastStartTimes.end())
            _broadcastStartTimes[broadcast.first] = currentTime;
    }

    auto it = _broadcastStartTimes.begin();
    while (it != _broadcastStartTimes.end()) {
        int playerNumber = it->first;
        auto startTime = it->second;

        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
        float elapsedTime = duration.count() / 1000.0f;

        if (elapsedTime >= ANIMATION_DURATION) {
            it = _broadcastStartTimes.erase(it);
            continue;
        }

        const auto& playerInfo = _gameInfos->getPlayer(playerNumber);
        if (playerInfo.teamName.empty()) {
            ++it;
            continue;
        }

        Vector3f position = {
            static_cast<float>(playerInfo.x * zappy::gui::POSITION_MULTIPLIER),
            getOffset(DisplayPriority::PLAYER, playerInfo.x, playerInfo.y) + 0.3f,
            static_cast<float>(playerInfo.y * zappy::gui::POSITION_MULTIPLIER)
        };

        for (int ring = 0; ring < NUM_RINGS; ++ring) {
            float ringStartTime = ring * RING_DELAY;

            if (elapsedTime >= ringStartTime) {
                float ringElapsedTime = elapsedTime - ringStartTime;
                float ringDuration = ANIMATION_DURATION - ringStartTime;
                float progress = std::min(1.0f, ringElapsedTime / ringDuration);
                float maxRadius = 2.5f;
                float animatedRadius = 0.3f + (progress * maxRadius);
                float alpha = std::max(0.0f, 1.0f - progress);

                if (alpha > 0.05f) {
                    Color32 ringColor = CYELLOW;
                    ringColor.a = static_cast<unsigned char>(alpha * 255);

                    float thickness = 0.12f * (0.5f + 0.5f * alpha);

                    drawTorus(position, animatedRadius, thickness, 16, ringColor);
                }
            }
        }

        ++it;
    }
}

void Map::drawTorus(const Vector3f &position, float radius, float thickness,
    int radialSegments, Color32 color)
{
    float angleStep = 2.0f * M_PI / radialSegments;

    for (int i = 0; i < radialSegments; ++i) {
        float angle1 = i * angleStep;
        float angle2 = (i + 1) * angleStep;

        Vector3f point1 = {
            position.x + radius * std::cos(angle1),
            position.y,
            position.z + radius * std::sin(angle1)
        };

        Vector3f point2 = {
            position.x + radius * std::cos(angle2),
            position.y,
            position.z + radius * std::sin(angle2)
        };

        this->_display->drawCylinderEx(point1, point2, thickness, thickness, 8, color);
    }
}
