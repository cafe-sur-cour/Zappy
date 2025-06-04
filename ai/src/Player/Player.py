#
# EPITECH PROJECT, 2025
# zappy
# File description:
# player.py
#

import os
from random import randint
from threading import Thread
from time import sleep
from src.Hash.Hash import Hash
from src.Exceptions.Exceptions import (
    CommunicationException
)

from src.Communication.Communication import Communication
from src.Utils.Utils import SUCCESS, FAILURE


class Player:
    def __init__(self, name: str, ip: str, port: int = 4242) -> None:
        self.communication: Communication = Communication(name, ip, port)
        self.childs: list[int] = []
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
        self._commThread: Thread = Thread(
            target=self.communication.loop,
            name=f"CommunicationThread-{self.teamName}"
        )
        slots, x, y = self.communication.connectToServer()
        self.setMapSize(x, y)
        if slots > 0:
            self.childs.append(self.create_child())

    def __del__(self):
        if len(self.childs) == 0:
            return
        for pid in self.childs:
            os.waitpid(pid, os.WNOHANG)

    def __str__(self):
        return (f"Player team: {self.teamName}, "
                f"Level: {self.level}, "
                f"Inventory: {self.inventory}, "
                f"Alive: {not self.communication.is_dead()}, "
                f"In Incantation: {self.in_incantation}")

    def create_child(self) -> int:
        pid: int = os.fork()
        if pid < 0:
            return -1
        if pid == 0:
            try:
                p = Player(self.teamName, self.ip, self.port)
                p.startComThread()
                p.loop()
            except CommunicationException:
                exit(FAILURE)
            exit(SUCCESS)
        return pid

    def startComThread(self) -> None:
        self._commThread.start()

    def setMapSize(self, x: int, y: int) -> None:
        self.x = x
        self.y = y

    def loop(self) -> None:
        while not self.communication.is_dead():
            direction = randint(0, 3)
            if direction == 0:
                self.communication.sendLeft()
            elif direction == 1:
                self.communication.sendRight()
            self.communication.sendForward()

            newInventory = self.communication.getInventory()
            if newInventory:
                self.inventory = newInventory

            if self.communication.get_size_message_queue() > 0:
                message = self.communication.get_message_from_queue()
                print(
                    "Message received from the server "
                    f"{self.hash.unHashMessage(str(message[1][:-1]))}"
                )
