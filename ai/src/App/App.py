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
        self.childs: list[int] = []

    def __del__(self):
        for pid in self.childs:
            try:
                os.waitpid(pid, os.WNOHANG)
            except ChildProcessError:
                pass

    def create_new_player(self) -> int:
        pid: int = os.fork()
        if pid < 0:
            return -1
        if pid == 0:
            try:
                p = Player(self.name, self.ip, self.port)
                _, x, y = p.communication.connectToServer()
                p.setMapSize(x, y)
                p.startComThread()
                p.loop()
            except CommunicationException:
                exit(FAILURE)
            exit(SUCCESS)
        return pid

    def run(self):
        print(f"Starting Zappy AI for team: {self.name}...")
        player = Player(self.name, self.ip, self.port)
        slots, x, y = player.communication.connectToServer()
        player.setMapSize(x, y)
        for _ in range(slots):
            self.childs.append(self.create_new_player())
        player.startComThread()
        player.loop()
        print(f"Zappy AI for team {self.name} has finished running.")
        return SUCCESS
