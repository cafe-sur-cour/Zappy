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
