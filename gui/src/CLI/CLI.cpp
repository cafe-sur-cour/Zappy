/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** CLI
*/

#include "CLI.hpp"
#include "../Exceptions/Exceptions.hpp"

CLI::CLI(int ac, char **av) : _ac(ac), _av(av)
{
}

CLI::~CLI()
{
}

zappy::constants::structs::Config CLI::parseArguments(int ac, char **av) const
{
    if (!hasCorrectNumberOfArguments(ac))
        throw Exceptions::CLIParsingException("Invalid number of arguments. Expected 5 arguments.");

    (void)av;
    return {0, ""};
}

bool CLI::hasCorrectNumberOfArguments(int ac) const
{
    return ac == 5;
}
