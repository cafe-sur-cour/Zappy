/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** CLI
*/

#ifndef CLI_HPP_
#define CLI_HPP_

#include "../Utils/Constants.hpp"

class CLI {
    public:
        CLI(int ac, char **av);
        ~CLI();

        zappy::constants::structs::Config parseArguments(int ac, char **av) const;

    private:
        int _ac;
        char **_av;
};

#endif /* !CLI_HPP_ */
