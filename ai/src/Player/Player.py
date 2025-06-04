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
    def __init__(self, name: str, ip: str, port: int = 4242) -> None:
        self.communication: Communication = Communication(name, ip, port)
        self.help_message_send: int = 0
        self.teamName: str = name
        self.ip: str = ip
        self.port: int = port
        self.level: int = 1
        self.hash: Hash = Hash(name)
        self.inventory: dict[str, int] = {
            "food": 10, "linemate": 0, "deraumere": 0, "sibur": 0,
            "mendiane": 0, "phiras": 0, "thystame": 0
        }
        self.in_incantation: bool = False
        self.x = 0
        self.y = 0

    def __str__(self):
        return (f"Player team: {self.teamName}, "
                f"Level: {self.level}, "
                f"Inventory: {self.inventory}, "
                f"Alive: {not self.communication.is_dead()}, "
                f"In Incantation: {self.in_incantation}")

    def setMapSize(self, x: int, y: int) -> None:
        self.x = x
        self.y = y

    def begin_incantation(self) -> None:
        # TODO: Send to the server the command to start an incantation
        self.in_incantation = True
        print("Incantation started!")

    def lay_an_egg(self) -> None:
        # TODO: Send to the server the command to lay an egg
        print("Lay a new egg!")

    def send_help_message(self) -> None:
        self.help_message_send += 1
        message = self.hash.hashMessage(f"HELP {self.help_message_send}")
        self.communication.sendBroadcast(message)

    def loop(self) -> None:
        # This is a default loop for the player to simulate actions it's not definitive
        while not self.communication.is_dead():
            try:
                self.communication.sendForward()
                # TODO: check if the player is dead after sending the command
                print("Player go forward")
                self.inventory = self.communication.getInventory()
                # TODO: check if the player is dead after sending the command
                if self.communication.get_size_message_queue() > 0:
                    message = self.communication.get_message_from_queue()
                    print(
                        "Message received from the server "
                        f"{self.hash.unHashMessage(str(message[1][:-1]))}"
                    )
                # TODO: Identifier le / les messages reçu / decrypter les messages
                if self.inventory["food"] <= 5:
                    self.send_help_message()
                else:
                    self.help_message_send = 0
                print("Current inventory:", self.inventory)
            except PlayerDead:
                print(f"The player is dead")
