/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Client
*/

#include <iostream>

#include "Client.hpp"
#include "../CLI/CLI.hpp"

Client::Client(int ac, char **av)
{
    initialize(ac, av);
}

Client::~Client()
{
}

void Client::initialize(int ac, char **av)
{
    CLI cli(ac, av);

    _config = cli.parseArguments(ac, av);
}
