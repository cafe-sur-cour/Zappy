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

    inline const char *T_BOLD = "\033[1m";
    inline const char *T_RED = "\033[1m\033[31m";
    inline const char *T_GREEN = "\033[1m\033[32m";
    inline const char *T_YELLOW = "\033[1m\033[33m";
    inline const char *T_BLUE = "\033[1m\033[34m";
    inline const char *T_MAGENTA = "\033[1m\033[35m";
    inline const char *T_CYAN = "\033[1m\033[36m";
    inline const char *T_WHITE = "\033[1m\033[37m";
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

        Tile(int _x = 0, int _y = 0, int _food = 0, int _linemate = 0,
             int _deraumere = 0, int _sibur = 0, int _mendiane = 0,
             int _phiras = 0, int _thystame = 0)
            : x(_x), y(_y), food(_food), linemate(_linemate),
              deraumere(_deraumere), sibur(_sibur),
              mendiane(_mendiane), phiras(_phiras), thystame(_thystame) {}
    };

    struct Inventory {
        int food;
        int linemate;
        int deraumere;
        int sibur;
        int mendiane;
        int phiras;
        int thystame;

        Inventory(int _food = 0, int _linemate = 0, int _deraumere = 0,
                  int _sibur = 0, int _mendiane = 0, int _phiras = 0,
                  int _thystame = 0)
            : food(_food), linemate(_linemate), deraumere(_deraumere),
              sibur(_sibur), mendiane(_mendiane), phiras(_phiras),
              thystame(_thystame) {}
    };
    struct Player {
        int number;
        int x;
        int y;
        int orientation;
        int level;
        std::string teamName;
        struct Inventory inventory;

        Player(int _number = 0, int _x = 0, int _y = 0, int _orientation = 0,
               int _level = 1, const std::string &_teamName = "",
               struct Inventory _inventory = Inventory())
            : number(_number), x(_x), y(_y), orientation(_orientation),
              level(_level), teamName(_teamName), inventory(_inventory) {}
    };

    struct Incantation {
        int x;
        int y;
        int level;
        std::vector<int> players;

        Incantation(int _x = 0, int _y = 0, int _level = 1,
                    const std::vector<int> &_players = {})
            : x(_x), y(_y), level(_level), players(_players) {}
    };

    struct Egg {
        int eggNumber;
        int playerNumber;
        int x;
        int y;
        bool hatched;
        std::string teamName;

        Egg(int _eggNumber = 0, int _playerNumber = 0, int _x = 0, int _y = 0,
            bool _hatched = false, const std::string &_teamName = "")
            : eggNumber(_eggNumber), playerNumber(_playerNumber), x(_x), y(_y),
              hatched(_hatched), teamName(_teamName) {}
    };
};

inline const std::string WINDOW_TITLE = "Zappy GUI";

#endif /* !CONSTANTS_HPP_ */
