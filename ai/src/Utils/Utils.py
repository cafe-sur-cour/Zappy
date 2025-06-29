##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Utils
##

from typing import Any


def isNumber(str: str):
    try:
        int(str)
        return True
    except Exception:
        return False


class State:
    def __init__(self, status: bool, **attributes):
        self.status = status
        for name, value in attributes.items():
            self.__setattr__(name, value)
