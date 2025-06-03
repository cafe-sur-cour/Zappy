##
## EPITECH PROJECT, 2025
## zappy
## File description:
## main
##

from src.Utils.Utils import (
    USAGE_STRING,
    FAILURE,
    SUCCESS,
    Colors
)
from src.CLI.CLI import CLI
from src.Exceptions.Exceptions import CLIParsingException
from src.Player.Player import Player
from sys import argv
from src.Communication.Communication import Communication
from src.App.App import App


def main():
    if len(argv) == 2 and argv[1] == "-help":
        print(Colors.CYAN + USAGE_STRING + Colors.RESET)
        return SUCCESS

    try:
        cli = CLI()
        config = cli.parse_args(argv)

        print(f"{Colors.GREEN}"
            "AI initialized with configuration: "
            f"Port: {config['port']}, "
            f"Team name: {config['name']}, "
            f"Machine: {config['machine']}"
            f"{Colors.RESET}")

        app = App(config)
        return app.run()

    except CLIParsingException as e:
        print(f"{Colors.RED}CLI Parsing Error: {e}{Colors.RESET}")
        print(f"{Colors.YELLOW}{USAGE_STRING}{Colors.RESET}")
        return FAILURE

    except Exception as e:
        print(f"{Colors.RED}An unexpected error occurred: {e}{Colors.RESET}")
        return FAILURE
