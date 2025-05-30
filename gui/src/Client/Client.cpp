/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Client
*/

#include <iostream>
#include <memory>

#include "Client.hpp"
#include "../CLI/CLI.hpp"

Client::Client(int ac, const char *const *av)
{
    initialize(ac, av);

    std::cout << colors::GREEN << "Client initialized with configuration: "
              << "Port: " << _config.port
              << ", Hostname: " << _config.hostname
              << colors::RESET << std::endl;

    _communication = std::make_unique<Communication>(_config);
    while (true) {
        if (!_communication->isConnected())
            break;
    }
}

Client::~Client()
{
}

void Client::initialize(int ac, const char *const *av)
{
    CLI cli(ac, av);

    _config = cli.parseArguments(ac, av);
}
