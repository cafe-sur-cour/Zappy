/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** CLI
*/

#include "CLI.hpp"

CLI::CLI(int ac, char **av) : _ac(ac), _av(av)
{
}

CLI::~CLI()
{
}

zappy::constants::structs::Config CLI::parseArguments(int ac, char **av) const
{
    (void)ac;
    (void)av;
    return {0, ""};
}
