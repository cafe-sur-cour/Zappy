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
            "tookFood": False
        }

        self.canIncant: bool = False

        self.incantationPhase: str = "checkNbPlayers"
        self.incantationLastCommand: str = None

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
            f"Can Incant: {self.canIncant}"
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

    def incantationAction(self) -> None:
        if self.incantationPhase == "waitForLook":
            return

        elif self.incantationPhase == "checkNbPlayers":
            self.communication.sendLook()
            self.incantationLastCommand = "look"
            self.incantationPhase = "waitForLook"

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
            self.incantationLastCommand = "broadcast"
            self.incantationPhase = "waitingForGathering"

        elif self.incantationPhase == "waitingForGathering":
            self.communication.sendLook()
            self.incantationLastCommand = "look"
            self.incantationPhase = "waitForLook"

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
        if self.goToIncantationState["waitingForResponse"]:
            return

        if self.goToIncantationState["phase"] == "goToDirection":
            if self.goToIncantationState["direction"] == 0:
                self.goToIncantationState["phase"] = "lookAround"
                return

            if len(self.goToIncantationState["steps"]) == 0:
                self.goToIncantationState["steps"] = self.getStepsFromDirection()

            if len(self.goToIncantationState["steps"]) > 0:
                self.goToIncantationState["steps"][0]()
                self.goToIncantationState["steps"].pop(0)
                self.goToIncantationState["lastCommand"] = "step"
                self.goToIncantationState["waitingForResponse"] = True
            else:
                self.goToIncantationState["phase"] = "lookAround"

        elif self.goToIncantationState["phase"] == "lookAround":
            self.communication.sendLook()
            self.goToIncantationState["phase"] = "waitForLook"
            self.goToIncantationState["lastCommand"] = "look"
            self.goToIncantationState["waitingForResponse"] = True

        elif self.goToIncantationState["phase"] == "lootTile":
            self.look = self.communication.getLook() or self.look
            if self.look and len(self.look) > 0:
                if "food" in self.look[0].keys():
                    self.communication.sendTakeObject("food")
                    self.goToIncantationState["lastCommand"] = "take"
                    self.goToIncantationState["waitingForResponse"] = True
                    return
            self.goToIncantationState["phase"] = "waitAtLocation"
            self.goToIncantationState["lastCommand"] = None

        elif self.goToIncantationState["phase"] == "waitAtLocation":
            self.communication.sendInventory()
            self.goToIncantationState["lastCommand"] = "inventory"
            self.goToIncantationState["waitingForResponse"] = True

        elif self.goToIncantationState["phase"] == "checkIfCanIncant":
            neededStones = self.getNeededStonesByPriority()
            if not neededStones:
                self.logger.info("Gathered enough resources, can now incant")
                self.canIncant = True
                self.goToIncantationState["status"] = False
                self.goToIncantationState["steps"] = []
                self.goToIncantationState["waitingForResponse"] = False
            else:
                self.goToIncantationState["phase"] = "waitAtLocation"

    def handleResponseInventory(self) -> None:
        self.inventory = self.communication.getInventory() or self.inventory

        if self.goToIncantationState["status"] and self.goToIncantationState["lastCommand"] == "inventory":
            self.goToIncantationState["waitingForResponse"] = False
            self.goToIncantationState["phase"] = "checkIfCanIncant"
        else:
            neededStones = self.getNeededStonesByPriority()
            if not neededStones:
                self.canIncant = True
                self.goToIncantationState["status"] = False
                self.goToIncantationState["steps"] = []
                self.goToIncantationState["waitingForResponse"] = False

    def handleResponseLook(self) -> None:
        self.look = self.communication.getLook() or self.look

        if self.canIncant and self.incantationPhase == "waitForLook":
            if len(self.look) > 0 and "player" in self.look[0].keys():
                playerCount = self.look[0]["player"]
                if playerCount >= LVL_UPGRADES[self.level]["players"]:
                    self.incantationPhase = "dropStones"
                else:
                    self.incantationPhase = "needMorePlayers"

        elif self.goToIncantationState["status"] and self.goToIncantationState["lastCommand"] == "look":
            self.goToIncantationState["waitingForResponse"] = False
            if self.goToIncantationState["phase"] == "waitForLook":
                self.goToIncantationState["phase"] = "lootTile"

    def handleResponseOK(self) -> None:
        if self.goToIncantationState["status"] and self.goToIncantationState["waitingForResponse"]:
            self.goToIncantationState["waitingForResponse"] = False

            if self.goToIncantationState["lastCommand"] == "step":
                if len(self.goToIncantationState["steps"]) == 0:
                    self.goToIncantationState["phase"] = "lookAround"
            elif self.goToIncantationState["lastCommand"] == "take":
                self.goToIncantationState["phase"] = "waitAtLocation"

    def handleResponseKO(self) -> None:
        if self.goToIncantationState["status"]:
            self.logger.error(
                f"Go to incantation command '{self.goToIncantationState['lastCommand']}' failed"
            )
            self.goToIncantationState["waitingForResponse"] = False

            if self.goToIncantationState["phase"] == "goToDirection":
                self.goToIncantationState["steps"] = []
                self.goToIncantationState["phase"] = "lookAround"
            elif self.goToIncantationState["phase"] in ["waitForLook", "lootTile"]:
                self.goToIncantationState["phase"] = "waitAtLocation"
            else:
                self.logger.warning("Resetting gathering state due to error")
                self.goToIncantationState["status"] = False
                self.goToIncantationState["phase"] = "goToDirection"
                self.goToIncantationState["steps"] = []

        elif self.canIncant:
            self.logger.error(f"Incantation command '{self.incantationLastCommand}' failed")
            self.incantationPhase = "checkNbPlayers"
            if self.incantationLastCommand == "incantation":
                self.canIncant = False
                self.inIncantation = False
        else:
            self.logger.error(f"Roomba command '{self.roombaState['lastCommand']}' failed")

    def handleResponseElevationUnderway(self) -> None:
        self.inIncantation = True
        self.logger.display("Elevation underway, waiting for result...")

    def handleResponseCurrentLevel(self, rest: str) -> None:
        try:
            new_level = int(rest.strip())
            if new_level > self.level:
                self.logger.success(f"Player level increased to {new_level}")
                self.level = new_level
                self.canIncant = False
                self.incantationPhase = "checkNbPlayers"
                self.goToIncantationState["status"] = False
                self.goToIncantationState["steps"] = []
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

            if (
                lvl == self.level and 
                not self.goToIncantationState["status"] and 
                not self.canIncant and 
                not self.inIncantation
            ):
                self.logger.info(f"Received incantation call for level {lvl}, going to direction {direction}")
                self.goToIncantationState["direction"] = direction
                self.goToIncantationState["status"] = True
                self.goToIncantationState["phase"] = "goToDirection"
                self.goToIncantationState["steps"] = []
                self.goToIncantationState["waitingForResponse"] = False
                self.goToIncantationState["lastCommand"] = None

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
                    if self.canIncant:
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
