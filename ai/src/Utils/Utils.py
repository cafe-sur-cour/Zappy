##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Utils
##

USAGE_STRING = "Usage: ./zappy_ai -p port -n name [-h machine]\n" \
               "option\t\tdescription\n" \
               "-p port\t\tport number\n" \
               "-n name\t\tname of the team\n" \
               "-h machine\tname of the machine; localhost by default"

FAILURE = 84
SUCCESS = 0

LVL_UPGRADES = {
    1: {
        "players": 1,
        "stones": {
            "linemate": 1,
            "deraumere": 0,
            "sibur": 0,
            "mendiane": 0,
            "phiras": 0,
            "thystame": 0
        }
    },
    2: {
        "players": 2,
        "stones": {
            "linemate": 1,
            "deraumere": 1,
            "sibur": 1,
            "mendiane": 0,
            "phiras": 0,
            "thystame": 0
        }
    },
    3: {
        "players": 2,
        "stones": {
            "linemate": 2,
            "deraumere": 0,
            "sibur": 1,
            "mendiane": 0,
            "phiras": 2,
            "thystame": 0
        }
    },
    4: {
        "players": 4,
        "stones": {
            "linemate": 1,
            "deraumere": 1,
            "sibur": 2,
            "mendiane": 0,
            "phiras": 1,
            "thystame": 0
        }
    },
    5: {
        "players": 4,
        "stones": {
            "linemate": 1,
            "deraumere": 2,
            "sibur": 1,
            "mendiane": 3,
            "phiras": 0,
            "thystame": 0
        }
    },
    6: {
        "players": 6,
        "stones": {
            "linemate": 1,
            "deraumere": 2,
            "sibur": 3,
            "mendiane": 0,
            "phiras": 1,
            "thystame": 0
        }
    },
    7: {
        "players": 6,
        "stones": {
            "linemate": 2,
            "deraumere": 2,
            "sibur": 2,
            "mendiane": 2,
            "phiras": 2,
            "thystame": 1
        }
    },
}


class Colors:
    BOLD = "\033[1m"
    RED = "\033[1m\033[31m"
    GREEN = "\033[1m\033[32m"
    YELLOW = "\033[1m\033[33m"
    BLUE = "\033[1m\033[34m"
    MAGENTA = "\033[1m\033[35m"
    CYAN = "\033[1m\033[36m"
    WHITE = "\033[1m\033[37m"
    RESET = "\033[0m"
