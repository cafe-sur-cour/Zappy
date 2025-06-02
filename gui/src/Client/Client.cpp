/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Client
*/

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

#include "Client.hpp"
#include "../CLI/CLI.hpp"

Client::Client(int ac, const char *const *av) :
    _gameInfos(std::make_shared<GameInfos>())
{
    initialize(ac, av);

    std::cout << colors::GREEN << "Client initialized with configuration: "
              << "Port: " << _config.port
              << ", Hostname: " << _config.hostname
              << colors::RESET << std::endl;

    _communication = std::make_shared<Communication>(_config);
    _msgHandler = std::make_unique<MsgHandler>(_gameInfos, _communication);

    run();
}

Client::~Client()
{
}

void Client::initialize(int ac, const char *const *av)
{
    CLI cli(ac, av);

    _config = cli.parseArguments(ac, av);
}

void Client::run()
{
    while (_communication->isConnected()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
