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
#include "../Communication/Communication.hpp"
#include "../CLI/CLI.hpp"

Client::Client(int ac, const char *const *av)
{
    initialize(ac, av);

    std::cout << colors::T_GREEN << "Client initialized with configuration: "
              << "Port: " << _config.port
              << ", Hostname: " << _config.hostname
              << colors::RESET << std::endl;

    _communication = std::make_shared<Communication>(_config);
    _gameInfos = std::make_shared<GameInfos>(_communication);
    _msgHandler = std::make_unique<MsgHandler>(_gameInfos, _communication);

    if (!_communication->isConnected())
        return;

    _gui = std::make_unique<GUI>(_gameInfos);
    _gui->run();
}

Client::~Client()
{
}

void Client::initialize(int ac, const char *const *av)
{
    CLI cli(ac, av);

    _config = cli.parseArguments(ac, av);
}
