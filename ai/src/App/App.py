##
## EPITECH PROJECT, 2025
## zappy
## File description:
## App
##

import os
from src.Utils.Utils import (
    FAILURE,
    SUCCESS
)
from src.Player.Player import Player
from src.Exceptions.Exceptions import (
    CommunicationException
)


class App:
    def __init__(self, config: dict[str]):
        self.port = config["port"]
        self.name = config["name"]
        self.ip = config["machine"]

    def run(self):
        print(f"Starting Zappy AI for team: {self.name}...")
        player = Player(self.name, self.ip, self.port)
        player.startComThread()
        player.loop()
        return SUCCESS
