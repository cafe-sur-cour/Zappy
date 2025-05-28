/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** main
*/

#include <iostream>

#include "Utils/Constants.hpp"
#include "Client/Client.hpp"
#include "Exceptions/Exceptions.hpp"

int main(int ac, char **av)
{
    if (ac == 2 && std::string(av[1]) == "-help") {
        std::cout << zappy::constants::USAGE_STRING << std::endl;
        return zappy::constants::SUCCESS_EXIT_CODE;
    }

    try {
        Client client(ac, av);
    } catch (const Exceptions::CLIParsingException &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << zappy::constants::USAGE_STRING << std::endl;
        return zappy::constants::FAILURE_EXIT_CODE;
    } catch (const std::exception &e) {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
        return zappy::constants::FAILURE_EXIT_CODE;
    }
    return zappy::constants::SUCCESS_EXIT_CODE;
}
