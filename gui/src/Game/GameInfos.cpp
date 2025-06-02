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

#include "GameInfos.hpp"

GameInfos::GameInfos() :
    _mapWidth(0),
    _mapHeight(0),
    _gameOver(false)
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

void GameInfos::updateTile(const zappy::structs::Tile tile)
{
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

void GameInfos::updateTeamName(const std::string &teamName)
{
    if (std::find(_teamNames.begin(), _teamNames.end(), teamName) == _teamNames.end())
        _teamNames.push_back(teamName);
}

const std::vector<std::string> GameInfos::getTeamNames() const
{
    return _teamNames;
}

void GameInfos::addPlayer(const zappy::structs::Player player)
{
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

void GameInfos::updatePlayerLevel(int playerNumber, int level)
{
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
    if (playerNumber < 0)
        return;

    _players.erase(std::remove_if(_players.begin(), _players.end(),
        [playerNumber](const zappy::structs::Player &player) {
            return player.number == playerNumber;
        }), _players.end());
}

void GameInfos::updatePlayerResourceAction(int playerNumber, int resourceId, bool isCollecting)
{
    (void)isCollecting;
    if (playerNumber < 0 || resourceId < 0 || resourceId > 6)
        return;
}

void GameInfos::updatePlayerFork(int playerNumber)
{
    (void)playerNumber;
}

const std::vector<zappy::structs::Player> GameInfos::getPlayers() const
{
    return _players;
}

void GameInfos::addPlayerBroadcast(int playerNumber, const std::string &message)
{
    if (playerNumber < 0 || message.empty())
        return;

    _playersBroadcasting.emplace_back(playerNumber, message);
}

std::vector<std::pair<int, std::string>> GameInfos::getPlayersBroadcasting() const
{
    return _playersBroadcasting;
}

void GameInfos::addIncantation(const zappy::structs::Incantation incantation)
{
    _incantations.push_back(zappy::structs::Incantation(
        incantation.x, incantation.y, incantation.level, incantation.players));
}

void GameInfos::removeIncantation(int x, int y, int result)
{
    auto it = std::remove_if(_incantations.begin(), _incantations.end(),
                             [x, y](const zappy::structs::Incantation &inc) {
                                 return inc.x == x && inc.y == y;
                             });

    if (it != _incantations.end())
        _incantations.erase(it, _incantations.end());

    (void)result;
}

void GameInfos::addEgg(const zappy::structs::Egg egg)
{
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
    for (auto &egg : _eggs) {
        if (egg.eggNumber == eggNumber) {
            egg.hatched = true;
            return;
        }
    }
}

void GameInfos::updateEggDeath(int eggNumber)
{
    _eggs.erase(std::remove_if(_eggs.begin(), _eggs.end(),
        [eggNumber](const zappy::structs::Egg &egg) {
            return egg.eggNumber == eggNumber;
        }), _eggs.end());
}

const std::vector<zappy::structs::Egg> GameInfos::getEggs() const
{
    return _eggs;
}

void GameInfos::setGameOver(const std::string &winningTeam)
{
    _gameOver = true;
    _winningTeam = winningTeam;
}

std::pair<bool, std::string> GameInfos::isGameOver() const
{
    return std::make_pair(_gameOver, _winningTeam);
}
