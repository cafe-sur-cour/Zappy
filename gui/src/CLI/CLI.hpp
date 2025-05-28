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

        bool hasCorrectNumberOfArguments(int ac) const;
        int parsePort(const char *portStr) const;
        std::string parseHostname(const char *hostnameStr) const;
        void validateConfig(bool portFound, bool hostFound) const;
};

#endif /* !CLI_HPP_ */
