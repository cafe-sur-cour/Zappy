/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** MsgHandler
*/

#include <iostream>
#include <sstream>

#include "MsgHandler.hpp"

MsgHandler::MsgHandler(std::shared_ptr<GameInfos> gameInfos, std::shared_ptr<Communication> communication)
    : _running(false), _gameInfos(gameInfos), _communication(communication)
{
    _messageHandlers = {
        {"WELCOME", std::bind(&MsgHandler::handleWelcomeMessage, this, std::placeholders::_1)},
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
        std::cout << colors::GREEN << "[INFO] Message handler thread started"
                  << colors::RESET << std::endl;
    }
}

void MsgHandler::stop()
{
    if (_running) {
        _running = false;
        _condition.notify_one();
        if (_thread.joinable()) {
            _thread.join();
        }
        std::cout << colors::GREEN << "[INFO] Message handler thread stopped"
                  << colors::RESET << std::endl;
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
