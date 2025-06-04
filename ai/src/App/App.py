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
        self.childs = []

    def forkProg(self):
        pid = os.fork()

        if (pid < 0):
            return FAILURE

        if (pid > 0):
            self.childs.append(pid)

        if (pid == 0):
            player = Player(self.name, self.ip, self.port)
            try:
                slots, x, y = player.communication.connectToServer()
                if (slots > 0):
                    self.forkProg()
                player.setMapSize(x, y)
            except CommunicationException:
                exit(FAILURE)
            player.loop()

        return SUCCESS

    def run(self):
        result = self.forkProg()
        # TODO: Maybe handle the childs in another methode
        return result
