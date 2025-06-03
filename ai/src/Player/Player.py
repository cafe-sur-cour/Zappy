#
# EPITECH PROJECT, 2025
# zappy
# File description:
# player.py
#

from time import sleep
from src.Hash.Hash import Hash
from src.Exceptions.Exceptions import PlayerDead
from src.Communication.Communication import Communication


class Player:
    def __init__(self, name: str, com: Communication):
        self.communication: Communication = com
        self.teamName: str = name
        self.level: int = 1
        self.hash: Hash = Hash(name)
        self.inventory: dict[str, int] = {
            "food": 10, "linemate": 0, "deraumere": 0, "sibur": 0,
            "mendiane": 0, "phiras": 0, "thystame": 0
        }
        self.alive: bool = True
        self.in_incantation: bool = False
        self.communication.connectToServer()
        print("Ai connected to the server successfully!")

    def __str__(self):
        return (f"Player team: {self.teamName}, "
                f"Level: {self.level}, "
                f"Inventory: {self.inventory}, "
                f"Alive: {self.alive}, "
                f"In Incantation: {self.in_incantation}")

    def begin_incantation(self) -> None:
        print(f"Player team: {self.teamName} has started an incantation!")
        self.in_incantation = True
        sleep(10)
        print(f"Player team: {self.teamName} has ended an incantation!")
        self.in_incantation = False
        self.level = self.level + 1

    def lay_an_egg(self) -> None:
        # TODO: Send to the server the command to lay an egg
        print("Lay an new egg!")

    def loop(self) -> None:
        # This is a default loop for the player to simulate actions it's not definitive
        while self.alive:
            try:
                self.communication.sendForward()
                print("Player go forward")
                self.inventory = self.communication.getInventory()
                print("Current inventory:", self.inventory)
            except PlayerDead:
                print(f"The player is dead")
                self.alive = False
