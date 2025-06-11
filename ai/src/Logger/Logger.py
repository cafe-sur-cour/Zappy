##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Logger
##

from src.Utils.Utils import Colors


class Logger:
    def error(self, message: str) -> None:
        print(
            f"{Colors.RED}",
            message,
            f"{Colors.RESET}"
        )

    def info(self, message: str) -> None:
        print(
            f"{Colors.YELLOW}",
            message,
            f"{Colors.RESET}"
        )

    def help(self, message: str) -> None:
        print(
            f"{Colors.CYAN}",
            message,
            f"{Colors.RESET}"
        )

    def debug(self, message: str) -> None:
        print(message)

    def success(self, message: str) -> None:
        print(
            f"{Colors.GREEN}",
            message,
            f"{Colors.RESET}"
        )

    def display(self, message: str) -> None:
        print(message)
