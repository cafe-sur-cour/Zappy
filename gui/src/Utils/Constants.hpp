/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Constants
*/

#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

#include <string>

namespace zappy::constants {

    inline const char *USAGE_STRING = "USAGE: ./zappy_gui -p port -h machine\n"
                               "option\t\tdescription\n"
                               "-p port\t\tport number\n"
                               "-h machine\t\thostname of the server";

    inline const int FAILURE_EXIT_CODE = 84;
    inline const int SUCCESS_EXIT_CODE = 0;
};

namespace zappy::constants::structs {

    struct Config {
        int port;
        std::string hostname;
    };
};

#endif /* !CONSTANTS_HPP_ */
