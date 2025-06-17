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
            "lastCommand": None
        }

        self.canIncant: bool = False

        self.incantationPhase: str = "checkNbPlayers"
        self.incantationLastCommand: str = None

        self.goToIncantationPhase: str = "goToDirection"
        self.goToIncantation: bool = False
        self.goToIncantationSteps: list[()] = []
        self.goToIncantationLastCommand: str = None
        self.incantationDirection: int = 0

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

    def dropStonesForSurvival(self) -> None:
        dropPriority = ["thystame", "phiras", "mendiane", "sibur", "deraumere", "linemate"]

        for stone in dropPriority:
            if self.inventory.get(stone, 0) > 0:
                self.logger.info(f"Survival mod critical: drop {stone}")
                self.communication.sendSetObject(stone)
                return

    def hasEnoughFoodForIncantation(self) -> bool:
        nbStones = sum(
            LVL_UPGRADES[self.level]["stones"].values()
        )
        return self.inventory["food"] * 126 >= nbStones * 7 + 300

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

    def incantationAction(self) -> None:
        if self.incantationPhase == "checkNbPlayers":
            self.communication.sendLook()
            self.incantationLastCommand = "look"

        elif self.incantationPhase == "dropStones":
            if not self.hasEnoughFoodForIncantation():
                self.canIncant = False
                self.incantationPhase = "checkNbPlayers"
                return
            stones: dict[str, int] = LVL_UPGRADES[self.level]["stones"]
            for stone, quantity in stones.items():
                for _ in range(quantity):
                    self.communication.sendSetObject(stone)
            self.incantationPhase = "canStartIncantation"
            self.incantationLastCommand = "set"

        elif self.incantationPhase == "canStartIncantation":
            self.communication.sendIncantation()
            self.incantationPhase = "startedIncantation"
            self.incantationLastCommand = "incantation"

        elif self.incantationPhase == "needMorePlayers":
            self.broadcaster.broadcastMessage(f"incantation {self.level}")
            self.incantationPhase = "checkNbPlayers"
            self.incantationLastCommand = "broadcast"

    def getStepsFromDirection(self) -> list[()]:
        stepsMap = {
            1: [
                self.communication.sendForward
            ],
            2: [
                self.communication.sendForward,
                self.communication.sendLeft
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
        return stepsMap.get(self.incantationDirection, [])

    def goToIncantationAction(self) -> None:
        if self.goToIncantationPhase == "lookAround":
            self.communication.sendLook()
            self.goToIncantationPhase = "lootTile"
            self.goToIncantationLastCommand = "look"

        elif self.goToIncantationPhase == "lootTile":
            self.look = self.communication.getLook() or self.look
            if self.look:
                if "food" in self.look[0].keys():
                    self.communication.sendTakeObject("food")
            self.goToIncantationPhase = "goToDirection"
            self.goToIncantationLastCommand = "take"

        elif self.goToIncantationPhase == "goToDirection":
            if self.incantationDirection == 0:
                self.inIncantation = True
                self.goToIncantation = False
                return
            if self.goToIncantationSteps == []:
                self.goToIncantationSteps = self.getStepsFromDirection()
            self.goToIncantationSteps[0]()
            self.goToIncantationSteps.pop(0)
            self.goToIncantationLastCommand = "step"
            if self.goToIncantationSteps == []:
                self.incantationDirection = -1
                self.goToIncantationPhase = "lookAround"

    def handleResponseInventory(self) -> None:
        self.inventory = self.communication.getInventory() or self.inventory
        neededStones = self.getNeededStonesByPriority()
        if not neededStones:
            self.canIncant = True

    def handleResponseLook(self) -> None:
        self.look = self.communication.getLook() or self.look
        if self.canIncant and self.incantationPhase == "checkNbPlayers":
            if len(self.look) > 0 and "player" in self.look[0].keys():
                playerCount = self.look[0]["player"]
                if playerCount >= LVL_UPGRADES[self.level]["players"]:
                    self.incantationPhase = "dropStones"
                else:
                    self.incantationPhase = "needMorePlayers"

    def handleResponseKO(self) -> None:
        if not self.canIncant:
            self.logger.error(f"Roomba command '{self.roombaState['lastCommand']}' failed")
        if self.canIncant:
            self.logger.error(f"Incantation command '{self.incantationLastCommand}' failed")
            self.incantationPhase = "checkNbPlayers"
            self.canIncant = False
        if self.goToIncantation:
            self.logger.error(f"Go to incantation command '{self.goToIncantationLastCommand}' failed")
            self.goToIncantation = False
        self.inIncantation = False

    def handleResponseOK(self) -> None:
        return

    def handleResponseElevationUnderway(self) -> None:
        self.inIncantation = True
        self.logger.display("Incantation underway, waiting for elevation...")

    def handleResponseCurrentLevel(self, rest: str) -> None:
        try:
            new_level = int(rest.strip())
            if new_level > self.level:
                self.logger.success(f"Player level increased to {new_level}")
                self.level = new_level
                self.canIncant = False
                self.incantationPhase = "checkNbPlayers"
                self.goToIncantation = False
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
        if message.startswith("incantation "):
            lvl = -1
            try:
                lvl = int(message.split(" ")[1].strip())
            except ValueError:
                self.logger.error(f"lvl in incantation message is not an int (received {lvl})")
            if lvl == self.level:
                self.incantationDirection = direction
                self.goToIncantation = True

    def loop(self) -> None:
        try:
            while not self.communication.playerIsDead():
                if self.communication.hasMessages():
                    data = self.communication.getLastMessage()
                    direction = data[0]
                    message = self.broadcaster.revealMessage(data[1])
                    self.handleMessages(direction, message)

                if self.communication.hasResponses():
                    response = self.communication.getLastResponse()
                    if response:
                        if response.strip() == "dead":
                            self.logger.display("Player died")
                            break
                        self.handleCommandResponse(response)

                if (
                    not self.communication.hasRequests() and
                    not self.communication.hasPendingCommands() and
                    not self.inIncantation and
                    not self.communication.hasResponses()
                ):
                    if self.canIncant:
                        self.incantationAction()
                    elif self.goToIncantation:
                        self.goToIncantationAction()
                    else:
                        self.roombaAction()


        except (CommunicationException, SocketException):
            pass
        except KeyboardInterrupt:
            pass
        except Exception:
            pass
        finally:
            self.communication.stopLoop()
