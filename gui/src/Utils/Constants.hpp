/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Constants
*/

#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

#include <string>
#include <vector>

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

namespace zappy::structs {

    struct Config {
        int port;
        std::string hostname;
    };

    struct Tile {
        int x;
        int y;
        int food;
        int linemate;
        int deraumere;
        int sibur;
        int mendiane;
        int phiras;
        int thystame;

        Tile(int x = 0, int y = 0, int food = 0, int linemate = 0,
             int deraumere = 0, int sibur = 0, int mendiane = 0,
             int phiras = 0, int thystame = 0)
            : x(x), y(y), food(food), linemate(linemate),
              deraumere(deraumere), sibur(sibur),
              mendiane(mendiane), phiras(phiras), thystame(thystame) {}
    };

    struct Inventory {
        int food;
        int linemate;
        int deraumere;
        int sibur;
        int mendiane;
        int phiras;
        int thystame;

        Inventory(int food = 0, int linemate = 0, int deraumere = 0,
                  int sibur = 0, int mendiane = 0, int phiras = 0,
                  int thystame = 0)
            : food(food), linemate(linemate), deraumere(deraumere),
              sibur(sibur), mendiane(mendiane), phiras(phiras),
              thystame(thystame) {}
    };
    struct Player {
        int number;
        int x;
        int y;
        int orientation;
        int level;
        std::string teamName;
        struct Inventory inventory;

        Player(int number = 0, int x = 0, int y = 0, int orientation = 0,
               int level = 1, const std::string &teamName = "",
               struct Inventory inventory = Inventory())
            : number(number), x(x), y(y), orientation(orientation),
              level(level), teamName(teamName), inventory(inventory) {}
    };

    struct Incantation {
        int x;
        int y;
        int level;
        std::vector<int> players;

        Incantation(int x = 0, int y = 0, int level = 1,
                    const std::vector<int> &players = {})
            : x(x), y(y), level(level), players(players) {}
    };

    struct Egg {
        int eggNumber;
        int playerNumber;
        int x;
        int y;
        bool hatched;
        std::string teamName;

        Egg(int eggNumber = 0, int playerNumber = 0, int x = 0, int y = 0,
            bool hatched = false, const std::string &teamName = "")
            : eggNumber(eggNumber), playerNumber(playerNumber), x(x), y(y),
              hatched(hatched), teamName(teamName) {}
    };
};

#endif /* !CONSTANTS_HPP_ */
