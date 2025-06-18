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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

void Map::draw(bool performanceMode)
{
    _performanceMode = performanceMode;

    updatePlayerRotations();
    updatePlayerPositions();

    auto mapSize = _gameInfos->getMapSize();
    int mapWidth = mapSize.first;
    int mapHeight = mapSize.second;

    if (_performanceMode) {
        float minDistance = std::numeric_limits<float>::max();
        int cameraX = mapWidth / 2;
        int cameraY = mapHeight / 2;
        int searchStep = std::max(1, std::min(mapWidth, mapHeight) / 20);

        for (int testY = 0; testY < mapHeight; testY += searchStep) {
            for (int testX = 0; testX < mapWidth; testX += searchStep) {
                Vector3f testPos = {
                    static_cast<float>(testX * zappy::gui::POSITION_MULTIPLIER),
                    0.0f,
                    static_cast<float>(testY * zappy::gui::POSITION_MULTIPLIER)
                };
                float distance = _display->vector3DDistanceFromCamera(testPos);
                if (distance < minDistance) {
                    minDistance = distance;
                    cameraX = testX;
                    cameraY = testY;
                }
            }
        }

        for (int offsetY = -searchStep; offsetY <= searchStep; ++offsetY) {
            for (int offsetX = -searchStep; offsetX <= searchStep; ++offsetX) {
                int testX = cameraX + offsetX;
                int testY = cameraY + offsetY;

                if (testX >= 0 && testX < mapWidth && testY >= 0 && testY < mapHeight) {
                    Vector3f testPos = {
                        static_cast<float>(testX * zappy::gui::POSITION_MULTIPLIER),
                        0.0f,
                        static_cast<float>(testY * zappy::gui::POSITION_MULTIPLIER)
                    };
                    float distance = _display->vector3DDistanceFromCamera(testPos);
                    if (distance < minDistance) {
                        minDistance = distance;
                        cameraX = testX;
                        cameraY = testY;
                    }
                }
            }
        }

        int fogRangeInTiles = static_cast<int>(zappy::gui::FOG_DISTANCE_MAX / zappy::gui::POSITION_MULTIPLIER) + 2;

        for (int y = std::max(0, cameraY - fogRangeInTiles);
             y < std::min(mapHeight, cameraY + fogRangeInTiles + 1); ++y) {
            for (int x = std::max(0, cameraX - fogRangeInTiles);
                 x < std::min(mapWidth, cameraX + fogRangeInTiles + 1); ++x) {

                const auto& tile = _gameInfos->getTileRef(x, y);

                Vector3f tilePosition = {
                    static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
                    0.0f,
                    static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)
                };
                float distanceFromCamera = _display->vector3DDistanceFromCamera(tilePosition);

                if (distanceFromCamera > zappy::gui::FOG_DISTANCE_MAX)
                    continue;

                drawPerformanceTile(tile);
                drawEggs(x, y);
                drawPerformanceFood(x, y, tile);
                drawPerformanceRock(x, y, tile);
            }
        }
    } else {
        for (int y = 0; y < mapHeight; ++y) {
            for (int x = 0; x < mapWidth; ++x) {
                const auto& tile = _gameInfos->getTileRef(x, y);

                drawTile(x, y, tile);
                drawEggs(x, y);
                drawFood(x, y, tile);
                drawRock(x, y, tile);
            }
        }
    }

    drawAllPlayers();
    drawBroadcastingPlayers();
    drawIncantations();
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

void Map::drawPerformanceTile(const zappy::structs::Tile &tile)
{
    Vector3f position = {
        static_cast<float>(tile.x * zappy::gui::POSITION_MULTIPLIER),
        0.0f - 0.2f,
        static_cast<float>(tile.y * zappy::gui::POSITION_MULTIPLIER)
    };

    float cubeSize = 2.0f;
    this->_display->drawCube(position, cubeSize, 0.2f, cubeSize, CPINK);
    this->_display->drawCubeWires(position, cubeSize, 0.2f, cubeSize, CBLACK);
}

void Map::drawAllPlayers()
{
    const auto& players = _gameInfos->getPlayers();

    for (const auto& player : players) {
        Vector3f interpolatedPosition = getPlayerInterpolatedPosition(player.number,
            player.x, player.y);

        if (_performanceMode) {
            float distanceFromCamera = _display->vector3DDistanceFromCamera(interpolatedPosition);
            if (distanceFromCamera > zappy::gui::FOG_DISTANCE_MAX) {
                continue;
            }
        }

        size_t stackIndex = 0;
        for (const auto& otherPlayer : players) {
            if (otherPlayer.number != player.number &&
                otherPlayer.x == player.x && otherPlayer.y == player.y &&
                otherPlayer.number < player.number) {
                stackIndex++;
            }
        }

        interpolatedPosition.y = getOffset(DisplayPriority::PLAYER, player.x, player.y,
            stackIndex);

        Color32 teamColor = getTeamColor(player.teamName);
        float rotationAngle = getPlayerInterpolatedRotation(player.number,
            player.orientation);

        try {
            std::string playerModel = zappy::gui::PLAYER_MODELS_INFO.at(player.level - 1).name;
            Vector3f playerScale = zappy::gui::PLAYER_MODELS_INFO.at(player.level - 1).scale;
            float playerInitialRotation =
                zappy::gui::PLAYER_MODELS_INFO.at(player.level - 1).rotation;
            rotationAngle += playerInitialRotation;

            this->_display->drawModelEx(playerModel, interpolatedPosition, {0.0f, 1.0f, 0.0f},
                rotationAngle, playerScale, teamColor);
        } catch (const std::out_of_range&) {
            std::string playerModel = zappy::gui::PLAYER_MODELS_INFO.at(0).name;
            Vector3f playerScale = zappy::gui::PLAYER_MODELS_INFO.at(0).scale;
            float playerInitialRotation = zappy::gui::PLAYER_MODELS_INFO.at(0).rotation;
            rotationAngle += playerInitialRotation;

            this->_display->drawModelEx(playerModel, interpolatedPosition, {0.0f, 1.0f, 0.0f},
                rotationAngle, playerScale, teamColor);
        }
    }
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

    for (size_t i = 0; i < eggsOnTile.size(); ++i) {
        Vector3f position = {
            static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
            getOffset(DisplayPriority::EGG, x, y, i),
            static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)
        };

        Color32 teamColor = getTeamColor(eggsOnTile[i]->teamName);

        static float timeAccumulator = 0.0f;
        timeAccumulator += this->_display->getFrameTime();

        float rotationAngle = timeAccumulator * 0.50f +
            static_cast<float>(x * 10 + y * 15 + i * 20);

        this->_display->drawModelEx("egg", position, {0.0f, 1.0f, 0.0f},
            rotationAngle, {zappy::gui::EGG_SCALE,
                            zappy::gui::EGG_SCALE,
                            zappy::gui::EGG_SCALE},
                            teamColor);
    }
}

void Map::drawFood(int x, int y, const zappy::structs::Tile &tile)
{
    if (tile.food <= 0)
        return;

    static float timeAccumulator = 0.0f;
    timeAccumulator += this->_display->getFrameTime();

    for (int i = 0; i < tile.food; ++i) {
        float baseHeight = getOffset(DisplayPriority::FOOD, x, y, static_cast<size_t>(i));
        float phase = static_cast<float>(x * 10 + y * 15 + i * 20) * 0.1f;
        float floatOffset = sin(timeAccumulator * zappy::gui::FOOD_FLOAT_SPEED + phase) *
            zappy::gui::FOOD_FLOAT_AMPLITUDE;
        Vector3f position = {
            static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
            baseHeight + floatOffset,
            static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)
        };

        float rotationAngle = timeAccumulator * 0.50f +
            static_cast<float>(x * 10 + y * 15 + i * 20);

        this->_display->drawModelEx("food", position, {0.0f, 1.0f, 0.0f},
            rotationAngle,
            {zappy::gui::FOOD_SCALE, zappy::gui::FOOD_SCALE, zappy::gui::FOOD_SCALE},
            CWHITE);
    }
}

void Map::drawPerformanceFood(int x, int y, const zappy::structs::Tile &tile)
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
            0.0f,
            {zappy::gui::FOOD_SCALE, zappy::gui::FOOD_SCALE, zappy::gui::FOOD_SCALE},
            CWHITE);
    }
}

void Map::drawRock(int x, int y, const zappy::structs::Tile &tile)
{
    if (tile.linemate <= 0 && tile.deraumere <= 0 && tile.sibur <= 0 &&
        tile.mendiane <= 0 && tile.phiras <= 0 && tile.thystame <= 0)
        return;

    static float timeAccumulator = 0.0f;
    timeAccumulator += this->_display->getFrameTime();

    int index = 0;

    for (int i = 0; i < tile.linemate; ++i) {
        Vector3f position = {
            static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
            getOffset(DisplayPriority::ROCK, x, y, static_cast<size_t>(index++)),
            static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)
        };

        float rotationAngle = timeAccumulator * 0.50f +
            static_cast<float>(x * 10 + y * 15 + i * 20);

        this->_display->drawModelEx("linemate", position, {0.0f, 1.0f, 0.0f},
            rotationAngle,
            {zappy::gui::LINEMATE_SCALE,
             zappy::gui::LINEMATE_SCALE,
             zappy::gui::LINEMATE_SCALE},
            CWHITE);
    }

    for (int i = 0; i < tile.deraumere; ++i) {
        Vector3f position = {
            static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
            getOffset(DisplayPriority::ROCK, x, y, static_cast<size_t>(index++)),
            static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)
        };

        float rotationAngle = timeAccumulator * 0.50f +
            static_cast<float>(x * 10 + y * 15 + i * 20);

        this->_display->drawModelEx("deraumere", position, {0.0f, 1.0f, 0.0f},
            rotationAngle,
            {zappy::gui::DERAUMERE_SCALE,
             zappy::gui::DERAUMERE_SCALE,
             zappy::gui::DERAUMERE_SCALE},
            CWHITE);
    }

    for (int i = 0; i < tile.sibur; ++i) {
        Vector3f position = {
            static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
            getOffset(DisplayPriority::ROCK, x, y, static_cast<size_t>(index++)),
            static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)
        };

        float rotationAngle = timeAccumulator * 0.50f +
            static_cast<float>(x * 10 + y * 15 + i * 20);

        this->_display->drawModelEx("sibur", position, {0.0f, 1.0f, 0.0f},
            rotationAngle,
            {zappy::gui::SIBUR_SCALE, zappy::gui::SIBUR_SCALE, zappy::gui::SIBUR_SCALE},
            CWHITE);
    }

    for (int i = 0; i < tile.mendiane; ++i) {
        Vector3f position = {
            static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
            getOffset(DisplayPriority::ROCK, x, y, static_cast<size_t>(index++)),
            static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)
        };

        float rotationAngle = timeAccumulator * 0.50f +
            static_cast<float>(x * 10 + y * 15 + i * 20);

        this->_display->drawModelEx("mendiane", position, {0.0f, 1.0f, 0.0f},
            rotationAngle,
            {zappy::gui::MENDIANE_SCALE,
             zappy::gui::MENDIANE_SCALE,
             zappy::gui::MENDIANE_SCALE},
            CWHITE);
    }

    for (int i = 0; i < tile.phiras; ++i) {
        Vector3f position = {
            static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
            getOffset(DisplayPriority::ROCK, x, y, static_cast<size_t>(index++)),
            static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)
        };

        float rotationAngle = timeAccumulator * 0.50f +
            static_cast<float>(x * 10 + y * 15 + i * 20);

        this->_display->drawModelEx("phiras", position, {0.0f, 1.0f, 0.0f},
            rotationAngle,
            {zappy::gui::PHIRAS_SCALE, zappy::gui::PHIRAS_SCALE, zappy::gui::PHIRAS_SCALE},
            CWHITE);
    }

    for (int i = 0; i < tile.thystame; ++i) {
        Vector3f position = {
            static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
            getOffset(DisplayPriority::ROCK, x, y, static_cast<size_t>(index++)),
            static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)
        };

        float rotationAngle = timeAccumulator * 0.50f +
            static_cast<float>(x * 10 + y * 15 + i * 20);

        this->_display->drawModelEx("thystame", position, {0.0f, 1.0f, 0.0f},
            rotationAngle,
            {zappy::gui::THYSTAME_SCALE,
             zappy::gui::THYSTAME_SCALE,
             zappy::gui::THYSTAME_SCALE},
            CWHITE);
    }
}

void Map::drawPerformanceRock(int x, int y, const zappy::structs::Tile &tile)
{
    if (tile.linemate <= 0 && tile.deraumere <= 0 && tile.sibur <= 0 &&
        tile.mendiane <= 0 && tile.phiras <= 0 && tile.thystame <= 0)
        return;

    int index = 0;
    float sphereRadius = 0.15f;

    for (int i = 0; i < tile.linemate + tile.deraumere + tile.sibur + tile.mendiane
                + tile.phiras + tile.thystame; ++i) {
        Vector3f position = {
            static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
            getOffset(DisplayPriority::ROCK, x, y, static_cast<size_t>(index++)),
            static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)
        };
        this->_display->drawSphere(position, sphereRadius, CBLUE);
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

            const auto& currentTile = _gameInfos->getTileRef(x, y);
            bool hasRocks = (currentTile.linemate > 0 || currentTile.deraumere > 0 ||
                            currentTile.sibur > 0 || currentTile.mendiane > 0 ||
                            currentTile.phiras > 0 || currentTile.thystame > 0);

            float baseRockHeight = BASE_HEIGHT_ROCK;
            if (eggCount > 0)
                baseRockHeight = BASE_HEIGHT_EGG + (eggCount * EGG_HEIGHT);
            if (playerCount > 0)
                baseRockHeight += (playerCount * PLAYER_HEIGHT);
            if (hasRocks)
                baseRockHeight += ROCK_HEIGHT;
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

        Vector3f interpolatedPosition = getPlayerInterpolatedPosition(playerNumber,
            playerInfo.x, playerInfo.y);

        if (_performanceMode) {
            float distanceFromCamera = _display->vector3DDistanceFromCamera(interpolatedPosition);
            if (distanceFromCamera > zappy::gui::FOG_DISTANCE_MAX) {
                ++it;
                continue;
            }
        }

        Vector3f position = {
            interpolatedPosition.x,
            getOffset(DisplayPriority::PLAYER, playerInfo.x, playerInfo.y) + 0.3f,
            interpolatedPosition.z
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

void Map::drawIncantations()
{
    const auto& incantations = _gameInfos->getIncantations();
    if (incantations.empty())
        return;

    static float timeAccumulator = 0.0f;
    timeAccumulator += this->_display->getFrameTime();

    for (const auto& incantation : incantations) {
        Vector3f incantationWorldPos = {
            static_cast<float>(incantation.x * zappy::gui::POSITION_MULTIPLIER),
            0.0f,
            static_cast<float>(incantation.y * zappy::gui::POSITION_MULTIPLIER)
        };

        if (_performanceMode) {
            float distanceFromCamera = _display->vector3DDistanceFromCamera(incantationWorldPos);
            if (distanceFromCamera > zappy::gui::FOG_DISTANCE_MAX) {
                continue;
            }
        }

        float levelProgress = static_cast<float>(incantation.level - 1) / 7.0f;

        unsigned char red = static_cast<unsigned char>(50 + levelProgress * 180);
        unsigned char green = static_cast<unsigned char>(150 + (1.0f - levelProgress) * 80);
        unsigned char blue = static_cast<unsigned char>(255 - levelProgress * 200);
        Color32 incantationColor = {red, green, blue, 70};

        for (size_t i = 0; i < incantation.players.size(); ++i) {
            Vector3f basePosition = {
                static_cast<float>(incantation.x * zappy::gui::POSITION_MULTIPLIER),
                getOffset(DisplayPriority::PLAYER, incantation.x, incantation.y, i) + 0.4f,
                static_cast<float>(incantation.y * zappy::gui::POSITION_MULTIPLIER)
            };

            float phase =
                static_cast<float>(incantation.x * 10 + incantation.y * 15 + i * 25) * 0.1f;
            float floatOffset = sin(timeAccumulator * 0.8f + phase) * 0.12f;
            float radiusVariation = 0.7f + sin(timeAccumulator * 1.2f + phase) * 0.08f;

            Vector3f bubblePosition = basePosition;
            bubblePosition.y += floatOffset;

            this->_display->drawSphere(bubblePosition, radiusVariation, incantationColor);

            Vector3f highlightPos = bubblePosition;
            highlightPos.x += radiusVariation * 0.3f;
            highlightPos.y += radiusVariation * 0.4f;
            highlightPos.z += radiusVariation * 0.2f;

            Color32 highlightColor = {255, 255, 255, 180};
            this->_display->drawSphere(highlightPos, radiusVariation * 0.15f,
                highlightColor);

            Vector3f highlight2Pos = bubblePosition;
            highlight2Pos.x -= radiusVariation * 0.2f;
            highlight2Pos.y += radiusVariation * 0.3f;
            highlight2Pos.z -= radiusVariation * 0.1f;

            Color32 highlight2Color = {255, 255, 255, 90};
            this->_display->drawSphere(highlight2Pos, radiusVariation * 0.08f,
                highlight2Color);
        }
    }
}

float Map::orientationToRotation(int orientation)
{
    switch (orientation) {
        case 1: return 180.0f;
        case 2: return 90.0f;
        case 3: return 0.0f;
        case 4: return 270.0f;
        default: return 0.0f;
    }
}

float Map::normalizeAngle(float angle)
{
    while (angle < 0.0f) angle += 360.0f;
    while (angle >= 360.0f) angle -= 360.0f;
    return angle;
}

float Map::getShortestAngleDifference(float from, float to)
{
    float diff = normalizeAngle(to - from);
    if (diff > 180.0f)
        diff -= 360.0f;
    return diff;
}

void Map::updatePlayerRotations()
{
    auto now = std::chrono::steady_clock::now();
    const auto& players = _gameInfos->getPlayers();

    for (const auto& player : players) {
        int playerId = player.number;
        float targetRotation = orientationToRotation(player.orientation);

        if (_playerRotations.find(playerId) == _playerRotations.end()) {
            _playerRotations[playerId] = PlayerRotationState();
            _playerRotations[playerId].currentRotation = targetRotation;
            _playerRotations[playerId].targetRotation = targetRotation;
            _playerRotations[playerId].lastUpdateTime = now;
            continue;
        }

        PlayerRotationState& rotState = _playerRotations[playerId];

        if (std::abs(normalizeAngle(rotState.targetRotation) - normalizeAngle(targetRotation))
            > zappy::gui::ROTATION_INTERPOLATION_THRESHOLD) {
            rotState.targetRotation = targetRotation;
            rotState.isRotating = true;
        }

        if (rotState.isRotating) {
            auto deltaTime =
                std::chrono::duration<float>(now - rotState.lastUpdateTime).count();
            float angleDiff = getShortestAngleDifference(rotState.currentRotation,
                rotState.targetRotation);

            if (std::abs(angleDiff) < zappy::gui::ROTATION_INTERPOLATION_THRESHOLD) {
                rotState.currentRotation = rotState.targetRotation;
                rotState.isRotating = false;
            } else {
                float currentTps = static_cast<float>(_gameInfos->getTimeUnit());
                float referenceTps = 10.0f;
                float maxTpsForScaling = 50.0f;
                float effectiveTps = std::min(currentTps, maxTpsForScaling);
                float scalingFactor = std::sqrt(effectiveTps / referenceTps);
                float scaledRotationSpeed = zappy::gui::PLAYER_ROTATION_SPEED * scalingFactor;
                float rotationStep = scaledRotationSpeed * deltaTime;
                if (angleDiff > 0)
                    rotState.currentRotation += std::min(rotationStep, angleDiff);
                else
                    rotState.currentRotation += std::max(-rotationStep, angleDiff);
                rotState.currentRotation = normalizeAngle(rotState.currentRotation);
            }
        }

        rotState.lastUpdateTime = now;
    }

    auto it = _playerRotations.begin();
    while (it != _playerRotations.end()) {
        bool playerExists = std::any_of(players.begin(), players.end(),
            [&](const zappy::structs::Player& p) { return p.number == it->first; });

        if (!playerExists)
            it = _playerRotations.erase(it);
        else
            ++it;
    }
}

float Map::getPlayerInterpolatedRotation(int playerId, int serverOrientation)
{
    updatePlayerRotations();

    auto it = _playerRotations.find(playerId);
    if (it != _playerRotations.end()) {
        return it->second.currentRotation;
    }

    return orientationToRotation(serverOrientation);
}

Vector3f Map::calculatePlayerWorldPosition(int x, int y)
{
    return {
        static_cast<float>(x * zappy::gui::POSITION_MULTIPLIER),
        0.0f,
        static_cast<float>(y * zappy::gui::POSITION_MULTIPLIER)
    };
}

float Map::getDistance(const Vector3f& from, const Vector3f& to)
{
    float dx = to.x - from.x;
    float dy = to.y - from.y;
    float dz = to.z - from.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

Vector3f Map::lerpVector3f(const Vector3f& from, const Vector3f& to, float t)
{
    return {
        from.x + (to.x - from.x) * t,
        from.y + (to.y - from.y) * t,
        from.z + (to.z - from.z) * t
    };
}

void Map::updatePlayerPositions()
{
    auto now = std::chrono::steady_clock::now();
    const auto& players = _gameInfos->getPlayers();

    for (const auto& player : players) {
        int playerId = player.number;
        Vector3f targetPosition = calculatePlayerWorldPosition(player.x, player.y);

        if (_playerPositions.find(playerId) == _playerPositions.end()) {
            _playerPositions[playerId] = PlayerPositionState();
            _playerPositions[playerId].currentPosition = targetPosition;
            _playerPositions[playerId].targetPosition = targetPosition;
            _playerPositions[playerId].lastUpdateTime = now;
            continue;
        }

        PlayerPositionState& posState = _playerPositions[playerId];

        float maxTileDistance = zappy::gui::POSITION_MULTIPLIER * 1.5f;
        float distanceToTarget = getDistance(posState.currentPosition, targetPosition);

        if (distanceToTarget > maxTileDistance) {
            posState.currentPosition = targetPosition;
            posState.targetPosition = targetPosition;
            posState.isMoving = false;
            posState.lastUpdateTime = now;
            continue;
        }

        if (getDistance(posState.targetPosition, targetPosition) >
            zappy::gui::MOVEMENT_INTERPOLATION_THRESHOLD) {
            posState.targetPosition = targetPosition;
            posState.isMoving = true;
        }

        if (posState.isMoving) {
            auto deltaTime =
                std::chrono::duration<float>(now - posState.lastUpdateTime).count();
            float distance = getDistance(posState.currentPosition, posState.targetPosition);

            if (distance < zappy::gui::MOVEMENT_INTERPOLATION_THRESHOLD) {
                posState.currentPosition = posState.targetPosition;
                posState.isMoving = false;
            } else {
                float currentTps = static_cast<float>(_gameInfos->getTimeUnit());
                float referenceTps = 10.0f;
                float maxTpsForScaling = 50.0f;
                float effectiveTps = std::min(currentTps, maxTpsForScaling);
                float scalingFactor = std::sqrt(effectiveTps / referenceTps);
                float scaledMovementSpeed = zappy::gui::PLAYER_MOVEMENT_SPEED * scalingFactor;
                float movementStep = scaledMovementSpeed * deltaTime;

                float t = std::min(1.0f, movementStep / distance);
                posState.currentPosition = lerpVector3f(posState.currentPosition,
                    posState.targetPosition, t);
            }
        }

        posState.lastUpdateTime = now;
    }

    auto it = _playerPositions.begin();
    while (it != _playerPositions.end()) {
        bool playerExists = std::any_of(players.begin(), players.end(),
            [&](const zappy::structs::Player& p) { return p.number == it->first; });

        if (!playerExists)
            it = _playerPositions.erase(it);
        else
            ++it;
    }
}

Vector3f Map::getPlayerInterpolatedPosition(int playerId, int serverX, int serverY)
{
    updatePlayerPositions();

    auto it = _playerPositions.find(playerId);
    if (it != _playerPositions.end()) {
        return it->second.currentPosition;
    }

    return calculatePlayerWorldPosition(serverX, serverY);
}
