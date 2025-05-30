/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** MsgHandler
*/

#include <iostream>
#include <sstream>
#include <memory>
#include <string>

#include "MsgHandler.hpp"

MsgHandler::MsgHandler(std::shared_ptr<GameInfos> gameInfos,
    std::shared_ptr<Communication> communication)
    : _running(false),
    _gameInfos(gameInfos),
    _communication(communication)
{
    _messageHandlers = {
        {"WELCOME", std::bind(&MsgHandler::handleWelcomeMessage, this, std::placeholders::_1)},
        {"msz", std::bind(&MsgHandler::handleMszMessage, this, std::placeholders::_1)},
        {"bct", std::bind(&MsgHandler::handleBctMessage, this, std::placeholders::_1)},
        {"tna", std::bind(&MsgHandler::handleTnaMessage, this, std::placeholders::_1)},
        {"pnw", std::bind(&MsgHandler::handlePnwMessage, this, std::placeholders::_1)},
        // ppo
        // plv
        // pin
        // pex
        // pbc
        // pic
        // pie
        // pfk
        // pdr
        // pgt
        // pdi
        // enw
        // ebo
        // edi
        {"sgt", std::bind(&MsgHandler::handleSgtMessage, this, std::placeholders::_1)},
        // sst
        // seg
        // smg
        // suc
        // sbp
    };

    start();
}

MsgHandler::~MsgHandler()
{
    stop();
}

void MsgHandler::start()
{
    if (!_running) {
        _running = true;
        _thread = std::thread(&MsgHandler::messageLoop, this);
    }
}

void MsgHandler::stop()
{
    if (_running) {
        _running = false;
        _condition.notify_one();

        if (_thread.joinable())
            _thread.join();
    }
}

void MsgHandler::messageLoop()
{
    while (_running) {
        if (_communication->hasMessages()) {
            handleMessage(_communication->popMessage());

        } else {
            std::unique_lock<std::mutex> lock(_mutex);

            _condition.wait_for(lock, std::chrono::milliseconds(100), [this] {
                return !_running || _communication->hasMessages();
            });
        }
    }
}

void MsgHandler::handleMessage(const std::string& message)
{
    if (message.empty())
        return;

    for (auto &handler : _messageHandlers) {
        if (message.find(handler.first) != std::string::npos && handler.second(message))
            return;
    }
    std::cout << colors::RED << "[WARNING] Unhandled message: " << message
              << colors::RESET << std::endl;
}

bool MsgHandler::handleWelcomeMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::cout << colors::YELLOW << "[INFO] Welcome message received: " << message
              << colors::RESET << std::endl;
    _communication->sendMessage("GRAPHIC\n");
    std::cout << colors::YELLOW << "[INFO] Sent GRAPHIC command to server."
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handleMszMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix;
    int width, height;

    iss >> prefix >> width >> height;

    if (iss.fail() || width <= 0 || height <= 0 || prefix != "msz") {
        std::cerr << colors::RED << "[WARNING] Invalid map size format received: " << message
                  << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->setMapSize(width, height);
    }

    std::cout << colors::YELLOW << "[INFO] Map size set to: " << width << "x" << height
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handleSgtMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix;
    int timeUnit;

    iss >> prefix >> timeUnit;

    if (iss.fail() || timeUnit <= 0 || prefix != "sgt") {
        std::cerr << colors::RED << "[WARNING] Invalid time unit format received: " << message
                  << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->setTimeUnit(timeUnit);
    }

    std::cout << colors::YELLOW << "[INFO] Time unit set to: " << timeUnit
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handleBctMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix;
    int x, y, food, linemate, deraumere, sibur, mendiane, phiras, thystame;
    int width, height;

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        auto mapSize = _gameInfos->getMapSize();

        width = mapSize.first;
        height = mapSize.second;
    }

    iss >> prefix >> x >> y >> food >> linemate >> deraumere
        >> sibur >> mendiane >> phiras >> thystame;

    if (iss.fail() || prefix != "bct" || x < 0 || y < 0 || x >= width || y >= height ||
        food < 0 || linemate < 0 || deraumere < 0 ||
        sibur < 0 || mendiane < 0 || phiras < 0 || thystame < 0) {
        std::cerr << colors::RED << "[WARNING] Invalid tile data format received: " << message
                  << colors::RESET << std::endl;
        return false;
    }

    zappy::structs::Tile tile(x, y, food, linemate, deraumere,
                              sibur, mendiane, phiras, thystame);

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->updateTile(tile);
    }

    std::cout << colors::YELLOW << "[INFO] Tile updated: (" << x << ", " << y << ") "
              << "Food: " << food << ", Linemate: " << linemate
              << ", Deraumere: " << deraumere << ", Sibur: " << sibur
              << ", Mendiane: " << mendiane << ", Phiras: " << phiras
              << ", Thystame: " << thystame
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handleTnaMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, teamName;

    iss >> prefix >> teamName;

    if (iss.fail() || prefix != "tna" || teamName.empty()) {
        std::cerr << colors::RED << "[WARNING] Invalid team name format received: " << message
                  << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->updateTeamName(teamName);
    }

    std::cout << colors::YELLOW << "[INFO] Team name added: " << teamName
              << colors::RESET << std::endl;
    return true;
}

bool MsgHandler::handlePnwMessage(const std::string& message)
{
    if (message.empty())
        return false;

    std::istringstream iss(message);
    std::string prefix, teamName;
    int playerNumber, x, y, orientation, level;
    int width, height;

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        auto mapSize = _gameInfos->getMapSize();

        width = mapSize.first;
        height = mapSize.second;
    }

    iss >> prefix >> playerNumber >> x >> y >> orientation >> level >> teamName;

    if (iss.fail() || prefix != "pnw" || playerNumber < 0 || x < 0 || y < 0 ||
        x >= width || y >= height ||
        orientation < 0 || orientation > 3 || level < 1 || teamName.empty()) {
        std::cerr << colors::RED << "[WARNING] Invalid player data format received: "
                  << message << colors::RESET << std::endl;
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        auto teams = _gameInfos->getTeamNames();
        if (std::find(teams.begin(), teams.end(), teamName) == teams.end()) {
            std::cerr << colors::RED << "[WARNING] Team name not found: " << teamName
                      << colors::RESET << std::endl;
            return false;
        }
    }

    zappy::structs::Player player(playerNumber, x, y, orientation, level, teamName);
    {
        std::lock_guard<std::mutex> lock(_gameInfosMutex);
        _gameInfos->addPlayer(player);
    }

    std::cout << colors::YELLOW << "[INFO] Player added: " << playerNumber
              << " at (" << x << ", " << y << ") with orientation "
              << orientation << " and level " << level
              << " from team " << teamName
              << colors::RESET << std::endl;
    return true;
}
