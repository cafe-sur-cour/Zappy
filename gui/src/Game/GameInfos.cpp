/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GameInfos
*/

#include <utility>
#include <vector>
#include <algorithm>
#include <string>
#include <mutex>
#include <chrono>

#include "GameInfos.hpp"

GameInfos::GameInfos(std::shared_ptr<ICommunication> communication) :
    _mapWidth(0),
    _mapHeight(0),
    _gameOver(false)
{
    _communication = communication;
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

void GameInfos::setTimeUnit(int timeUnit, bool sendToServer)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (sendToServer)
        _communication->sendMessage("sst " + std::to_string(timeUnit) + "\n");
    else
        _timeUnit = timeUnit;
}

int GameInfos::getTimeUnit() const
{
    return _timeUnit;
}

void GameInfos::updateTile(const zappy::structs::Tile tile)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (tile.x < 0 || tile.y < 0 || tile.x >= _mapWidth || tile.y >= _mapHeight)
        return;

    for (auto &existingTile : _tiles) {
        if (existingTile.x == tile.x && existingTile.y == tile.y) {
            existingTile = zappy::structs::Tile(
                tile.x, tile.y, tile.food, tile.linemate, tile.deraumere,
                tile.sibur, tile.mendiane, tile.phiras, tile.thystame);
            return;
        }
    }
    _tiles.push_back(zappy::structs::Tile(
        tile.x, tile.y, tile.food, tile.linemate, tile.deraumere,
        tile.sibur, tile.mendiane, tile.phiras, tile.thystame));
}

const std::vector<zappy::structs::Tile> GameInfos::getTiles() const
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    return _tiles;
}

const zappy::structs::Tile GameInfos::getTile(int x, int y) const
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    for (const auto &tile : _tiles) {
        if (tile.x == x && tile.y == y)
            return tile;
    }

    return zappy::structs::Tile(x, y);
}

void GameInfos::updateTeamName(const std::string &teamName)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    if (std::find(_teamNames.begin(), _teamNames.end(), teamName) == _teamNames.end())
        _teamNames.push_back(teamName);
}

const std::vector<std::string> GameInfos::getTeamNames() const
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    return _teamNames;
}

void GameInfos::addPlayer(const zappy::structs::Player player)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    auto it = std::find_if(_players.begin(), _players.end(),
                           [&player](const zappy::structs::Player &p) {
                               return p.number == player.number;
                           });

    if (it != _players.end())
        *it = player;
    else {
        _players.push_back(zappy::structs::Player(
            player.number, player.x, player.y, player.orientation,
            player.level, player.teamName, player.inventory));
    }
}

void GameInfos::updatePlayerPosition(int playerNumber, int x, int y)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (x < 0 || y < 0 || x >= _mapWidth || y >= _mapHeight)
        return;

    for (auto &player : _players) {
        if (player.number == playerNumber) {
            player.x = x;
            player.y = y;
            return;
        }
    }
}

void GameInfos::updatePlayerOrientation(int playerNumber, int orientation)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (orientation < 1 || orientation > 4)
        return;

    for (auto &player : _players) {
        if (player.number == playerNumber) {
            player.orientation = orientation;
            return;
        }
    }
}

void GameInfos::updatePlayerLevel(int playerNumber, int level)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    for (auto &player : _players) {
        if (player.number == playerNumber) {
            player.level = level;
            return;
        }
    }
}

void GameInfos::updatePlayerInventory(int playerNumber,
    const zappy::structs::Inventory inventory)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    for (auto &player : _players) {
        if (player.number == playerNumber) {
            player.inventory = zappy::structs::Inventory(
                inventory.food, inventory.linemate, inventory.deraumere,
                inventory.sibur, inventory.mendiane, inventory.phiras,
                inventory.thystame);
            return;
        }
    }
}

void GameInfos::updatePlayerExpulsion(int playerNumber)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (playerNumber < 0)
        return;

    for (auto &expulsions : _playersExpulsing) {
        if (expulsions.first == playerNumber) {
            _playersExpulsing.erase(std::remove(_playersExpulsing.begin(),
                _playersExpulsing.end(), expulsions), _playersExpulsing.end());
        }
    }
    _playersExpulsing.emplace_back(playerNumber, true);
}

void GameInfos::updatePlayerDeath(int playerNumber)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    if (playerNumber < 0)
        return;

    _players.erase(std::remove_if(_players.begin(), _players.end(),
        [playerNumber](const zappy::structs::Player &player) {
            return player.number == playerNumber;
        }), _players.end());
}

void GameInfos::updatePlayerResourceAction(int playerNumber, int resourceId, bool isCollecting)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    (void)isCollecting;
    if (playerNumber < 0 || resourceId < 0 || resourceId > 6)
        return;
}

void GameInfos::updatePlayerFork(int playerNumber)
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    (void)playerNumber;
}

const std::vector<zappy::structs::Player> GameInfos::getPlayers() const
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    return _players;
}

void GameInfos::addPlayerBroadcast(int playerNumber, const std::string &message)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    if (playerNumber < 0 || message.empty())
        return;

    auto currentTime = std::chrono::steady_clock::now();
    _playersBroadcasting.emplace_back(playerNumber, message, currentTime);
}

const std::vector<std::pair<int, std::string>> GameInfos::getPlayersBroadcasting()
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    std::vector<std::pair<int, std::string>> result;
    for (const auto& broadcast : _playersBroadcasting) {
        result.emplace_back(std::get<0>(broadcast), std::get<1>(broadcast));
    }

    _playersBroadcasting.clear();
    return result;
}

void GameInfos::addIncantation(const zappy::structs::Incantation incantation)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    _incantations.push_back(zappy::structs::Incantation(
        incantation.x, incantation.y, incantation.level, incantation.players));
}

void GameInfos::removeIncantation(int x, int y, int result)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    auto it = std::remove_if(_incantations.begin(), _incantations.end(),
                             [x, y](const zappy::structs::Incantation &inc) {
                                 return inc.x == x && inc.y == y;
                             });

    if (it != _incantations.end())
        _incantations.erase(it, _incantations.end());

    (void)result;
}

const std::vector<zappy::structs::Incantation> GameInfos::getIncantations()
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    return _incantations;
}

void GameInfos::addEgg(const zappy::structs::Egg egg)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    auto it = std::find_if(_eggs.begin(), _eggs.end(),
                           [&egg](const zappy::structs::Egg &e) {
                               return e.eggNumber == egg.eggNumber;
                           });

    if (it != _eggs.end())
        *it = egg;
    else {
        _eggs.push_back(zappy::structs::Egg(
            egg.eggNumber, egg.playerNumber, egg.x, egg.y, egg.hatched, egg.teamName));
    }
}

void GameInfos::updateEggHatched(int eggNumber)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    for (auto &egg : _eggs) {
        if (egg.eggNumber == eggNumber) {
            egg.hatched = true;
            return;
        }
    }
}

void GameInfos::updateEggDeath(int eggNumber)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    _eggs.erase(std::remove_if(_eggs.begin(), _eggs.end(),
        [eggNumber](const zappy::structs::Egg &egg) {
            return egg.eggNumber == eggNumber;
        }), _eggs.end());
}

const std::vector<zappy::structs::Egg> GameInfos::getEggs() const
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    return _eggs;
}

void GameInfos::setGameOver(const std::string &winningTeam)
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    _gameOver = true;
    _winningTeam = winningTeam;
}

std::pair<bool, std::string> GameInfos::isGameOver() const
{
    std::lock_guard<std::mutex> lock(_dataMutex);
    return std::make_pair(_gameOver, _winningTeam);
}

const zappy::structs::Player GameInfos::getPlayer(int playerNumber) const
{
    std::lock_guard<std::mutex> lock(_dataMutex);

    for (const auto &player : _players) {
        if (player.number == playerNumber)
            return player;
    }
    return zappy::structs::Player();
}

void GameInfos::notifyStateChange() {
    notifyObservers();
}
