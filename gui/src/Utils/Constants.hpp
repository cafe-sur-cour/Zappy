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
                               "-h machine\thostname of the server";

    inline const int FAILURE_EXIT_CODE = 84;
    inline const int SUCCESS_EXIT_CODE = 0;
};

namespace colors {

    inline const char *BOLD = "\033[1m";
    inline const char *RED = "\033[1m\033[31m";
    inline const char *GREEN = "\033[1m\033[32m";
    inline const char *YELLOW = "\033[1m\033[33m";
    inline const char *BLUE = "\033[1m\033[34m";
    inline const char *MAGENTA = "\033[1m\033[35m";
    inline const char *CYAN = "\033[1m\033[36m";
    inline const char *WHITE = "\033[1m\033[37m";
    inline const char *RESET = "\033[0m";

};

namespace zappy::constants::structs {

    struct Config {
        int port;
        std::string hostname;
    };
};

#endif /* !CONSTANTS_HPP_ */
