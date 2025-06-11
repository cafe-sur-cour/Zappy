#
# EPITECH PROJECT, 2025
# zappy
# File description:
# player.py
#

import os
from threading import Thread
from time import sleep

from src.Broadcaster.Broadcaster import Broadcaster
from src.Exceptions.Exceptions import (
    CommunicationException,
    SocketException
)
from src.Communication.Communication import Communication
from src.Utils.Utils import SUCCESS, FAILURE


LVL_UPGRADES = {
    1: {"linemate": 1, "deraumere": 1, "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0},
    2: {"linemate": 1, "deraumere": 1, "sibur": 1, "mendiane": 0, "phiras": 0, "thystame": 0},
    3: {"linemate": 2, "deraumere": 0, "sibur": 1, "mendiane": 0, "phiras": 2, "thystame": 0},
    4: {"linemate": 1, "deraumere": 1, "sibur": 2, "mendiane": 0, "phiras": 1, "thystame": 0},
    5: {"linemate": 1, "deraumere": 2, "sibur": 1, "mendiane": 3, "phiras": 0, "thystame": 0},
    6: {"linemate": 1, "deraumere": 2, "sibur": 3, "mendiane": 0, "phiras": 1, "thystame": 0},
    7: {"linemate": 2, "deraumere": 2, "sibur": 2, "mendiane": 2, "phiras": 2, "thystame": 1},
}


class Player:
    def __init__(self, name: str, ip: str, port: int = 4242) -> None:
        self.communication: Communication = Communication(name, ip, port)
        self._commThread: Thread = Thread(
            target=self.communication.loop,
            name=f"CommunicationThread-{name}"
        )

        self.childs: list[int] = []

        self.teamName: str = name
        self.ip: str = ip
        self.port: int = port
        self.is_child_process = False

        self.level: int = 1
        self.broadcaster: Broadcaster = Broadcaster(self.communication, name)
        self.inventory: dict[str, int] = {
            "food": 10,
            "linemate": 0,
            "deraumere": 0,
            "sibur": 0,
            "mendiane": 0,
            "phiras": 0,
            "thystame": 0
        }
        self.look: list[dict[str, int]] = []
        self.inIncantation: bool = False
        self.x: int = 0
        self.y: int = 0

        self.roombaState: dict = {
            "forwardCount": 0,
            "targetForward": 10,
            "phase": "forward",
            "lastCommand": None
        }

    def __del__(self):
        try:
            self.communication.stopLoop()
            if self._commThread.is_alive():
                self._commThread.join(timeout=1.0)
        except Exception:
            pass

    def __str__(self):
        return (
            f"Player team: {self.teamName}, "
            f"Level: {self.level}, "
            f"Inventory: {self.inventory}, "
            f"Alive: {not self.communication.playerIsDead()}, "
            f"In Incantation: {self.inIncantation}"
        )

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
        self.roombaState["targetForward"] = max(x, y)

    def getNeededStonesByPriority(self) -> list[str]:
        neededStones = []
        for stone, quantity in LVL_UPGRADES[self.level].items():
            neededQuantity = quantity - self.inventory.get(stone, 0)
            neededStones.append((neededQuantity, stone))
        neededStones.sort(key=lambda x: x[0], reverse=True)
        neededStones = [stone for quantity, stone in neededStones if quantity > 0]
        return neededStones

    def dropStonesForSurvival(self) -> None:
        dropPriority = ["thystame", "phiras", "mendiane", "sibur", "deraumere", "linemate"]

        for stone in dropPriority:
            if self.inventory.get(stone, 0) > 0:
                print(f"Survival mod critical: drop {stone}")
                self.communication.sendSetObject(stone)
                return

    def getDirectionFromSound(self, direction: int) -> str:
        if direction == 0:
            return "self"

        direction_map = {
            1: "forward",
            2: "forwardLeft",
            3: "left",
            4: "backLeft",
            5: "back",
            6: "backRight",
            7: "right",
            8: "forwardRight"
        }
        return direction_map.get(direction, "unknown")

    def roombaAction(self) -> None:
        if self.roombaState["phase"] == "forward":
            if self.roombaState["lastCommand"] in ("left", "forward", None):
                self.communication.sendLook()
                self.roombaState["lastCommand"] = "look"

            elif self.roombaState["lastCommand"] == "look":
                self.look = self.communication.getLook() or self.look
                if self.look:
                    if "food" in self.look[0].keys():
                        self.communication.sendTakeObject("food")
                    neededStones = self.getNeededStonesByPriority()
                    for stone in neededStones:
                        if stone in self.look[0].keys():
                            self.communication.sendTakeObject(stone)
                self.roombaState["lastCommand"] = "take"

            elif self.roombaState["lastCommand"] == "take":
                if self.roombaState["forwardCount"] < self.roombaState["targetForward"]:
                    self.communication.sendForward()
                    self.roombaState["lastCommand"] = "forward"
                    self.roombaState["forwardCount"] += 1

                else:
                    self.roombaState["forwardCount"] = 0
                    self.roombaState["phase"] = "turn"
                    self.communication.sendRight()
                    self.roombaState["lastCommand"] = "right"

        elif self.roombaState["phase"] == "turn":
            if self.roombaState["lastCommand"] == "right":
                self.communication.sendForward()
                self.roombaState["lastCommand"] = "forward"

            elif self.roombaState["lastCommand"] == "forward":
                self.communication.sendLeft()
                self.roombaState["lastCommand"] = "left"
                self.roombaState["phase"] = "forward"

    def handleResponseInventory(self) -> None:
        self.inventory = self.communication.getInventory() or self.inventory

    def handleResponseLook(self) -> None:
        self.look = self.communication.getLook() or self.look

    def handleResponseKO(self) -> None:
        if not self.is_child_process:
            print(f"Command '{self.roombaState['lastCommand']}' failed")

    def handleResponseOK(self) -> None:
        return

    def handleCommandResponse(self, response: str) -> None:
        switcher = {
            "inventory": self.handleResponseInventory,
            "look": self.handleResponseLook,
            "ko": self.handleResponseKO,
            "ok": self.handleResponseOK,
        }
        handler = switcher.get(response.strip(), None)
        if handler:
            handler()
        elif not self.is_child_process:
            print(f"Unknown response: {response.strip()}")

    def loop(self) -> None:
        try:
            while not self.communication.playerIsDead():
                if self.communication.hasMessages():
                    data = self.communication.getLastMessage()
                    direction = data[0]
                    message = self.broadcaster.revealMessage(data[1])
                    if message and not self.is_child_process:
                        print(f"Received message: {message}")

                if self.communication.hasResponses():
                    response = self.communication.getLastResponse()
                    if response.strip() == "dead":
                        break
                    self.handleCommandResponse(response)

                if not self.communication.hasPendingCommands():
                    self.roombaAction()

                sleep(0.1)
        except (CommunicationException, SocketException):
            pass
        except KeyboardInterrupt:
            pass
        except Exception:
            pass
        finally:
            self.communication.stopLoop()
