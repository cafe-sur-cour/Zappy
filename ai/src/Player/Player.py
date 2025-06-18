#
# EPITECH PROJECT, 2025
# zappy
# File description:
# player.py
#

import os
from threading import Thread
from time import sleep
from random import randint

from src.Broadcaster.Broadcaster import Broadcaster
from src.Exceptions.Exceptions import (
    CommunicationException,
    SocketException
)
from src.Communication.Communication import Communication
from src.Utils.Utils import (
    SUCCESS,
    FAILURE,
    LVL_UPGRADES
)
from src.Logger.Logger import Logger


class Player:
    def __init__(self, name: str, ip: str, port: int = 4242) -> None:
        self.communication: Communication = Communication(name, ip, port)
        self._commThread: Thread = Thread(
            target=self.communication.loop,
            name=f"CommunicationThread-{name}"
        )

        self.logger = Logger()

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
            "lastCommand": None,
            "tookFood": False,
        }

        self.incantationState: dict = {
            "status": False,
            "phase": None,
            "lastCommand": None,
        }

        self.goToIncantationState: dict = {
            "status": False,
            "phase": "goToDirection",
            "steps": [],
            "lastCommand": None,
            "direction": 0,
        }

    def __del__(self):
        try:
            if hasattr(self, 'communication') and self.communication:
                self.communication.stopLoop()
            if (
                hasattr(self, '_commThread') and
                self._commThread and
                self._commThread.is_alive()
            ):
                self._commThread.join(timeout=1.0)
        except (SystemExit, KeyboardInterrupt):
            pass
        except Exception:
            pass

    def __str__(self):
        return (
            f"Player team: {self.teamName}, "
            f"Level: {self.level}, "
            f"Inventory: {self.inventory}, "
            f"Alive: {not self.communication.playerIsDead()}, "
            f"In Incantation: {self.inIncantation};"
            f"Can Incant: {self.incantationState["canIncant"]}"
            f"Going to incantation: {self.goToIncantationState["status"]}"
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

    def getNeededStonesByPriority(self) -> list[(str, int)]:
        neededStones = []
        if self.level == 8:
            return neededStones
        stones: dict[str, int] = LVL_UPGRADES[self.level]["stones"]
        for stone, quantity in stones.items():
            neededQuantity = quantity - self.inventory.get(stone, 0)
            neededStones.append((neededQuantity, stone))
        neededStones.sort(key=lambda x: x[0], reverse=True)
        neededStones = [(stone, quantity) for quantity, stone in neededStones if quantity > 0]
        return neededStones

    def roombaAction(self) -> None:
        if self.roombaState["phase"] == "forward":
            if self.roombaState["lastCommand"] in ("left", "forward", None):
                self.communication.sendLook()
                self.roombaState["lastCommand"] = "look"

            elif self.roombaState["lastCommand"] in ["look", "take food"]:
                self.look = self.communication.getLook() or self.look
                if self.look and len(self.look) > 0:
                    if "food" in self.look[0].keys() and not self.roombaState["tookFood"]:
                        self.communication.sendTakeObject("food")
                        self.roombaState["tookFood"] = True
                        self.roombaState["lastCommand"] == "take food"
                        return
                    tookStones = False
                    neededStones = self.getNeededStonesByPriority()
                    tile = self.look[0]
                    for stone, _ in neededStones:
                        if stone in tile.keys():
                            tookStones = True
                            self.communication.sendTakeObject(stone)
                            break
                    if tookStones:
                        self.communication.sendInventory()
                self.roombaState["tookFood"] = False
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

    def hasEnoughFoodForIncantation(self) -> bool:
        nbStones = sum(
            LVL_UPGRADES[self.level]["stones"].values()
        )
        return self.inventory["food"] * 126 >= nbStones * 7 + 300

    def incantationAction(self) -> None:
        pass

    def getStepsFromDirection(self) -> list[()]:
        stepsMap = {
            1: [
                self.communication.sendForward
            ],
            2: [
                self.communication.sendForward,
                self.communication.sendLeft,
                self.communication.sendForward,
            ],
            3: [
                self.communication.sendLeft,
                self.communication.sendForward
            ],
            4: [
                self.communication.sendRight,
                self.communication.sendRight,
                self.communication.sendForward,
                self.communication.sendRight,
                self.communication.sendForward
            ],
            5: [
                self.communication.sendRight,
                self.communication.sendRight,
                self.communication.sendForward
            ],
            6: [
                self.communication.sendRight,
                self.communication.sendRight,
                self.communication.sendForward,
                self.communication.sendLeft,
                self.communication.sendForward
            ],
            7: [
                self.communication.sendRight,
                self.communication.sendForward
            ],
            8: [
                self.communication.sendForward,
                self.communication.sendRight,
                self.communication.sendForward
            ]
        }
        return stepsMap.get(self.goToIncantationState["direction"], [])

    def goToIncantationAction(self) -> None:
        pass

    def handleResponseInventory(self) -> None:
        self.inventory = self.communication.getInventory() or self.inventory

        needStones = len(self.getNeededStonesByPriority()) > 0

        if not needStones:
            if not self.goToIncantationState["status"]:
                self.incantationState["status"] = True

    def handleResponseLook(self) -> None:
        self.look = self.communication.getLook() or self.look

    def handleResponseKO(self) -> None:
        self.logger.error(f"Roomba command '{self.roombaState['lastCommand']}' failed")

    def handleResponseOK(self) -> None:
        return

    def handleResponseElevationUnderway(self) -> None:
        self.inIncantation = True
        self.logger.display("Elevation underway, waiting for result...")

    def handleResponseCurrentLevel(self, rest: str) -> None:
        try:
            new_level = int(rest.strip())
            if new_level > self.level:
                self.logger.success(f"Player level increased to {new_level}")
                self.level = new_level
                self.inIncantation = False
            else:
                self.logger.error(
                    f"Unexpected level response: got {new_level}, old level = {self.level}"
                )
        except ValueError:
            self.logger.error(f"Invalid level response: {rest.strip()}")

    def handleCommandResponse(self, response: str) -> None:
        switcher = {
            "inventory": self.handleResponseInventory,
            "look": self.handleResponseLook,
            "ko": self.handleResponseKO,
            "ok": self.handleResponseOK,
            "Elevation underway": self.handleResponseElevationUnderway,
            "Current level: ": self.handleResponseCurrentLevel,
        }
        for key in switcher.keys():
            if response.startswith(key):
                rest = response[len(key):].strip()
                if rest:
                    switcher[key](rest)
                else:
                    switcher[key]()
                return
        self.logger.error(f"Unknown response: {response.strip()}")

    def handleMessages(self, direction: int, message: str) -> None:
        if message.startswith("are_you_level "):
            lvl = -1
            try:
                message_parts = message.split(" ")
                if len(message_parts) >= 2:
                    lvl = int(message_parts[1].strip())
                else:
                    self.logger.error("Invalid incantation message format")
                    return
            except (ValueError, IndexError) as e:
                self.logger.error(
                    f"Error parsing incantation message '{message}': {e}"
                )
                return
            if lvl == self.level:
                self.goToIncantationState["status"] = True
                self.incantationState["status"] = False
            else:
                self.broadcaster.broadcastMessage("not_same_level")
        elif message.startswith("not_same_level"):
            # Continue roaming to gather ressources
            pass
        elif message.startswith("come_incant"):
            # Go to the incantation spot
            pass
        elif message.startswith("is_everybody_here"):
            # if direction != 0 send i_am_not_here
            pass
        elif message.startswith("i_am_not_here"):
            # Wait for the players
            pass

    def loop(self) -> None:
        try:
            while not self.communication.playerIsDead():
                if self.communication.hasMessages():
                    data = self.communication.getLastMessage()
                    direction = data[0]
                    message = self.broadcaster.revealMessage(data[1])
                    if message.strip():
                        self.handleMessages(direction, message)

                if self.communication.hasResponses():
                    response = self.communication.getLastResponse()
                    if response:
                        if response.strip() == "dead":
                            self.logger.display("Player died")
                            break
                        self.handleCommandResponse(response)

                if (
                    not self.inIncantation and
                    not self.communication.hasRequests() and
                    not self.communication.hasPendingCommands() and
                    not self.communication.hasResponses()
                ):
                    if self.incantationState["status"]:
                        self.incantationAction()
                    elif self.goToIncantationState["status"]:
                        self.goToIncantationAction()
                    else:
                        self.roombaAction()

        except (CommunicationException, SocketException) as e:
            self.logger.error(f"Communication exception: {e}")
        except KeyboardInterrupt as e:
            self.logger.error(f"Keyboard Interrupt: {e}")
        except Exception as e:
            self.logger.error(f"Exception: {e}")
        finally:
            self.communication.stopLoop()
