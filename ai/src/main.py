##
## EPITECH PROJECT, 2025
## zappy
## File description:
## main
##

from src.Utils.Utils import USAGE_STRING, Colors
from src.CLI.CLI import CLI
from src.Exceptions.Exceptions import CLIParsingException
from src.Player.Player import Player
from sys import argv


def main():
    if len(argv) == 2 and argv[1] == "-help":
        print(Colors.CYAN + USAGE_STRING + Colors.RESET)
        return 0

    try:
        cli = CLI()
        config = cli.parse_args(argv)

        print(f"{Colors.GREEN}AI initialized with configuration: "
              f"Port: {config['port']}, "
              f"Team name: {config['name']}, "
              f"Machine: {config['machine']}{Colors.RESET}")
        player = Player(config['name'])
        player.loop()
        # TODO: Implement AI and Communication logic here
        return 0

    except CLIParsingException as e:
        print(f"{Colors.RED}CLI Parsing Error: {e}{Colors.RESET}")
        print(f"{Colors.YELLOW}{USAGE_STRING}{Colors.RESET}")
        return 84

    except Exception as e:
        print(f"{Colors.RED}An unexpected error occurred: {e}{Colors.RESET}")
        return 84
