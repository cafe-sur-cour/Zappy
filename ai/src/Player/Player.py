#
# EPITECH PROJECT, 2025
# zappy
# File description:
# player.py
#

import os
from threading import Thread
from typing import Callable
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
    LVL_UPGRADES,
    TOTAL_NEEDED_STONES
)
from src.Logger.Logger import Logger


class Player:
    def __init__(self, name: str, ip: str, port: int = 4242) -> None:
        self.communication: Communication = Communication(name, ip, port)
        self.commThread: Thread = Thread(
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
        self.nbTeamSlots: int = -1
        self.nbConnectedPlayers: int = 1
        self.sentNbSlots: bool = True

        self.pid: int = os.getpid()
        self.id: str = name + "-" + str(self.pid)
        self.senderID: str = None
        self.needToBroadcastInventory = False

        self.roombaState: dict = {
            "forwardCount": 0,
            "targetForward": 10,
            "phase": "lookAround",
            "lastCommand": None,
            "lastPhase": None,
            "teamInventories": [],
            "commandSentTime": 0,
            "highestPidSeen": 0,
        }

        self.incantationState: dict = {
            "status": False,
            "phase": "sendComeIncant",
            "lastCommand": None,
            "playerResponses": [],
            "nbResponsesWait": 0,
        }

        self.goToIncantationState: dict = {
            "status": False,
            "steps": [],
            "lastCommand": None,
            "direction": 0,
            "arrived": False,
            "movementStarted": False,
            "droppingStones": False,
        }
        self.teamHasEnoughStones = False

    def __del__(self):
        try:
            if hasattr(self, 'communication') and self.communication:
                self.communication.stopLoop()
            if (
                hasattr(self, 'commThread') and
                self.commThread and
                self.commThread.is_alive()
            ):
                self.commThread.join(timeout=1.0)
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
            f"In Incantation: {self.inIncantation}; "
            f"Incantation Status: {self.incantationState['status']}; "
            f"Going to incantation: {self.goToIncantationState['status']}"
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
        self.commThread.start()

    def setMapSize(self, x: int, y: int) -> None:
        self.x = x
        self.y = y
        self.roombaState["targetForward"] = max(x, y)

    def setNbSlots(self, slots: int) -> None:
        self.nbTeamSlots = slots

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

    def doesTeamHaveEnoughStones(self) -> bool:
        totalStones = {
            "linemate": self.inventory["linemate"],
            "deraumere": self.inventory["deraumere"],
            "sibur": self.inventory["sibur"],
            "mendiane": self.inventory["mendiane"],
            "phiras": self.inventory["phiras"],
            "thystame": self.inventory["thystame"]
        }
        for inventory in self.roombaState["teamInventories"]:
            totalStones["linemate"] += inventory["linemate"]
            totalStones["deraumere"] += inventory["deraumere"]
            totalStones["sibur"] += inventory["sibur"]
            totalStones["mendiane"] += inventory["mendiane"]
            totalStones["phiras"] += inventory["phiras"]
            totalStones["thystame"] += inventory["thystame"]
        for key in TOTAL_NEEDED_STONES.keys():
            if totalStones[key] < TOTAL_NEEDED_STONES[key]:
                self.teamHasEnoughStones = False
                return False
        self.teamHasEnoughStones = True
        return True

    def enoughFoodForIncantation(self, nbFood: int) -> bool:
        incantationCost = 300

        stonesToDrop = sum(LVL_UPGRADES[self.level]["stones"].values())
        droppingCost = stonesToDrop * 7

        maxLength = (max(self.x, self.y) / 2) * 4
        movementCost = 7 * maxLength

        totalCost = incantationCost + droppingCost + movementCost

        return nbFood * 126 > totalCost

    def teamHasEnoughFoodForIncantation(self) -> bool:
        if not self.enoughFoodForIncantation(self.inventory["food"]):
            return False

        for inventory in self.roombaState["teamInventories"]:
            if not self.enoughFoodForIncantation(inventory["food"] * 126):
                return False

        return True

    def roombaAction(self) -> None:
        phase = self.roombaState["phase"]

        if phase == "lookAround":
            self.communication.sendLook()
            self.roombaState["lastCommand"] = "look"
            self.roombaState["phase"] = "vacuum"
            self.roombaState["lastPhase"] = "lookAround"

        elif phase == "vacuum":
            self.look = self.communication.getLook() or self.look
            if self.look and len(self.look) > 0:
                for item, quantity in self.look[0].items():
                    if self.teamHasEnoughStones and item != "food":
                        continue
                    for _ in range(quantity):
                        self.communication.sendTakeObject(item)
            self.roombaState["lastCommand"] = "take"
            self.roombaState["phase"] = "forward"
            self.roombaState["lastPhase"] = "vacuum"

        elif phase == "updateInventory":
            self.communication.sendInventory()
            self.roombaState["lastCommand"] = "inventory"
            self.roombaState["phase"] = "forward"
            self.roombaState["lastPhase"] = "updateInventory"

        elif phase == "checkOnTeammates":
            if self.roombaState["lastCommand"] == "Connect_nbr":
                self.broadcaster.broadcastMessage(f"sendInventory {self.id}")
                self.roombaState["lastCommand"] = "broadcast sendInventory"

            elif self.roombaState["lastCommand"] == "broadcast sendInventory":
                expectedTeammates = self.nbConnectedPlayers - 1
                receivedInventories = len(self.roombaState["teamInventories"])

                if receivedInventories >= expectedTeammates:
                    if self.doesTeamHaveEnoughStones() and self.teamHasEnoughFoodForIncantation():
                        should_initiate = True
                        for inventory in self.roombaState["teamInventories"]:
                            teammate_id = inventory["id"]
                            if "-" in teammate_id:
                                teammate_pid = int(teammate_id.split("-")[1])
                                if teammate_pid > self.pid:
                                    should_initiate = False
                                    break

                        if should_initiate:
                            self.incantationState["status"] = True
                    self.roombaState["phase"] = "forward"
                else:
                    self.communication.sendGetConnectNbr()
                    self.roombaState["lastCommand"] = "Connect_nbr"

            else:
                self.communication.sendGetConnectNbr()
                self.roombaState["lastCommand"] = "Connect_nbr"

        elif phase == "forward":
            if self.roombaState["forwardCount"] < self.roombaState["targetForward"]:
                self.communication.sendForward()
                self.roombaState["lastCommand"] = "forward"
                self.roombaState["forwardCount"] += 1
                self.roombaState["phase"] = "lookAround"
                self.roombaState["lastPhase"] = "forward"

            else:
                self.roombaState["forwardCount"] = 0
                self.communication.sendRight()
                self.roombaState["lastCommand"] = "right"
                self.roombaState["phase"] = "turn"
                self.roombaState["lastPhase"] = "forward"

        elif phase == "turn":
            if self.roombaState["lastCommand"] == "right":
                self.communication.sendForward()
                self.roombaState["lastCommand"] = "forward"

            elif self.roombaState["lastCommand"] == "forward":
                self.communication.sendLeft()
                self.roombaState["lastCommand"] = "left"
                self.roombaState["phase"] = "lookAround"
                self.roombaState["lastPhase"] = "turn"

    def incantationAction(self) -> None:
        phase = self.incantationState["phase"]

        if phase == "sendComeIncant":
            self.broadcaster.broadcastMessage(f"comeIncant {self.id}")
            self.incantationState["lastCommand"] = "broadcast comeIncant"
            self.incantationState["lastPhase"] = "sendComeIncant"
            self.incantationState["phase"] = "sendConnectNbr"

        elif phase == "sendConnectNbr":
            self.communication.sendGetConnectNbr()
            self.incantationState["lastCommand"] = "Connect_nbr"
            self.incantationState["lastPhase"] = "sendConnectNbr"
            self.incantationState["phase"] = "checkNbPlayers"

        elif phase == "checkNbPlayers":
            if self.incantationState["lastCommand"] == "Connect_nbr":
                self.broadcaster.broadcastMessage(f"isEveryBodyHere {self.id}")
                self.incantationState["lastCommand"] = "broadcast isEveryBodyHere"
                self.incantationState["playerResponses"] = []
                self.incantationState["nbPlayersWait"] = 0

            elif self.incantationState["lastCommand"] == "broadcast isEveryBodyHere":
                receivedResponses = len(self.incantationState["playerResponses"])

                if receivedResponses >= self.nbConnectedPlayers:
                    self.incantationState["phase"] = "dropStones"
                    self.incantationState["lastCommand"] = None
                else:
                    if self.incantationState["nbPlayersWait"] == 1:
                        self.incantationState["status"] = False
                        self.incantationState["nbPlayersWait"] = 0
                        self.incantationState["playerResponses"] = []
                        self.incantationState["phase"] = "sendComeIncant"
                        self.roombaState["phase"] = "forward"
                        return
                    self.incantationState["nbPlayersWait"] += 1

        elif phase == "dropStones":
            self.broadcaster.broadcastMessage(f"dropStones {self.id}")
            self.incantationState["lastCommand"] = "broadcast dropStones"
            self.incantationState["phase"] = "droppingStones"

        elif phase == "droppingStones":
            for stone, quantity in LVL_UPGRADES[self.level]["stones"].items():
                for _ in range(quantity):
                    if self.inventory.get(stone, 0) > 0:
                        self.communication.sendSetObject(stone)
            self.communication.sendInventory()
            self.incantationState["phase"] = "startIncantation"
            self.incantationState["lastCommand"] = "drop stones"

        elif phase == "startIncantation":
            if not self.teamHasEnoughFoodForIncantation():
                self.logger.error("Not enough food for incantation, resetting state")
                self.incantationState["status"] = False
                self.incantationState["phase"] = "sendComeIncant"
                self.incantationState["lastCommand"] = None
                self.incantationState["playerResponses"] = []
                self.incantationState["nbPlayersWait"] = 0
                return

            self.communication.sendIncantation()
            self.incantationState["lastCommand"] = "incantation"
            self.incantationState["phase"] = "waitingForElevation"

        elif phase == "waitingForElevation":
            pass

    def goToIncantationAction(self) -> None:
        if self.goToIncantationState["droppingStones"]:
            for stone, quantity in LVL_UPGRADES[self.level]["stones"].items():
                for _ in range(quantity):
                    if self.inventory.get(stone, 0) > 0:
                        self.communication.sendSetObject(stone)
            self.goToIncantationState["droppingStones"] = False
            self.goToIncantationState["lastCommand"] = "drop stones"
            self.communication.sendInventory()
            return

        if (
            self.goToIncantationState["direction"] == 0 and
            self.goToIncantationState["movementStarted"]
        ):
            self.inIncantation = True
            self.goToIncantationState["arrived"] = True
            return

        if len(self.goToIncantationState["steps"]) == 0:
            sleep(0.1)
            return

        self.goToIncantationState["movementStarted"] = True

        step = self.goToIncantationState["steps"].pop(0)
        step()
        self.goToIncantationState["lastCommand"] = "step"

    def handleResponseInventory(self) -> None:
        newInventory = self.communication.getInventory()

        if not self.needToBroadcastInventory:
            for item in newInventory.keys():
                if (
                    item != "food" and
                    newInventory[item] > self.inventory[item] and
                    not self.incantationState["status"] and
                    not self.goToIncantationState["status"] and
                    self.roombaState["lastPhase"] == "updateInventory" and
                    self.nbTeamSlots != -1 and
                    self.enoughFoodForIncantation(newInventory["food"])
                ):
                    self.roombaState["phase"] = "checkOnTeammates"
                    break

        self.inventory = newInventory or self.inventory

        if self.needToBroadcastInventory:
            self.broadcaster.broadcastMessage(
                "inventory "
                f"{self.inventory["linemate"]},"
                f"{self.inventory["deraumere"]},"
                f"{self.inventory["sibur"]},"
                f"{self.inventory["mendiane"]},"
                f"{self.inventory["phiras"]},"
                f"{self.inventory["thystame"]},"
                f"{self.inventory["food"]} "
                f"{self.id} {self.senderID}"
            )
            self.needToBroadcastInventory = False

    def handleResponseLook(self) -> None:
        self.look = self.communication.getLook() or self.look

    def handleResponseKO(self) -> None:
        lastCommand = self.roombaState["lastCommand"]
        if self.incantationState["status"]:
            lastCommand = self.incantationState["lastCommand"]
        elif self.goToIncantationState["status"]:
            lastCommand = self.goToIncantationState["lastCommand"]

        self.logger.error(f"Command '{lastCommand}' failed")

        if self.roombaState["phase"] == "checkOnTeammates":
            self.roombaState["phase"] = "forward"

        if self.incantationState["status"] and lastCommand == "incantation":
            self.logger.error("Incantation failed, resetting incantation state")
            self.incantationState["status"] = False
            self.incantationState["phase"] = "sendComeIncant"
            self.incantationState["lastCommand"] = None
            self.incantationState["playerResponses"] = []
            self.incantationState["nbPlayersWait"] = 0

    def handleResponseOK(self) -> None:
        if (
            not self.incantationState["status"] and
            not self.goToIncantationState["status"] and
            self.roombaState["lastPhase"] == "vacuum"
        ):
            self.roombaState["phase"] = "updateInventory"

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

                if self.level >= 8:
                    self.logger.success("Player reached maximum level!")
                    self.incantationState["status"] = False
                    self.incantationState["phase"] = "sendComeIncant"
                    self.incantationState["lastCommand"] = None
                    self.incantationState["playerResponses"] = []
                    self.incantationState["nbPlayersWait"] = 0
                    self.goToIncantationState["status"] = False
                    self.goToIncantationState["arrived"] = False
                    self.goToIncantationState["movementStarted"] = False
                    self.goToIncantationState["steps"] = []
                    self.goToIncantationState["droppingStones"] = False
                else:
                    if self.incantationState["status"]:
                        self.incantationState["status"] = False
                        self.incantationState["phase"] = "sendComeIncant"
                        self.incantationState["lastCommand"] = None
                        self.incantationState["playerResponses"] = []
                        self.incantationState["nbPlayersWait"] = 0
                        self.roombaState["phase"] = "updateInventory"

                    self.goToIncantationState["status"] = False
                    self.goToIncantationState["arrived"] = False
                    self.goToIncantationState["movementStarted"] = False
                    self.goToIncantationState["steps"] = []
                    self.goToIncantationState["droppingStones"] = False
            else:
                self.logger.error(
                    f"Unexpected level response: got {new_level}, old level = {self.level}"
                )
        except ValueError:
            self.logger.error(f"Invalid level response: {rest.strip()}")

    def handleResponseConnectNbr(self, response: str) -> None:
        try:
            connectNbr = int(response)
            self.nbConnectedPlayers = self.nbTeamSlots - connectNbr
        except ValueError:
            self.logger.error(f"Invalid connect nbr: {response.strip()}")

    def handleCommandResponse(self, response: str) -> None:
        switcher: list[
            tuple[
                Callable[[str], bool] | Callable[[str, str], bool],
                str,
                Callable[[], None] | Callable[[str], None],
                Callable[[str], None | str]
            ]
        ] = [
            (
                str.startswith,
                "inventory",
                self.handleResponseInventory,
                lambda a: a[len("Current level: "):].strip()
            ),
            (
                str.startswith,
                "look",
                self.handleResponseLook,
                lambda a: a[len("Current level: "):].strip()
            ),
            (
                str.startswith,
                "ko",
                self.handleResponseKO,
                lambda a: None
            ),
            (
                str.startswith,
                "ok",
                self.handleResponseOK,
                lambda a: None
            ),
            (
                str.startswith,
                "Elevation underway",
                self.handleResponseElevationUnderway,
                lambda a: None
            ),
            (
                str.startswith,
                "Current level: ",
                self.handleResponseCurrentLevel,
                lambda a: a[len("Current level: "):].strip()
            ),
            (
                str.isdigit,
                None,
                self.handleResponseConnectNbr,
                lambda a: a.strip()
            ),
        ]

        foundSwitch = False
        for (condition, arg, r, trimer) in switcher:
            if arg:
                foundSwitch = condition(response, arg)
            else:
                foundSwitch = condition(response)
            if foundSwitch:
                param = trimer(response)
                if param:
                    r(param)
                else:
                    r()
                return

        lastCommand = self.roombaState["lastCommand"]
        if self.incantationState["status"]:
            lastCommand = self.incantationState["lastCommand"]
        elif self.goToIncantationState["status"]:
            lastCommand = self.goToIncantationState["lastCommand"]
        self.logger.error(f"Unknown response to '{lastCommand}': {response.strip()}")

    def getStepsFromDirection(self) -> list[Callable[[], None]]:
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

    def handleMessageTeamslots(self, direction: int, rest: str) -> None:
        try:
            nbSlots = int(rest.strip())
            self.nbTeamSlots = nbSlots
        except ValueError:
            self.logger.error(f"Invalid number of team slots: {rest.strip()}")

    def handleMessageSendInventory(self, direction: int, rest: str) -> None:
        id = rest.strip()
        if id != self.id:
            self.senderID = id
            self.needToBroadcastInventory = True
            self.communication.sendInventory()

    def handleMessageInventory(self, direction: int, rest: str) -> None:
        contents = rest.split(" ")
        if len(contents) != 3:
            return
        try:
            data = contents[0]
            responderID = contents[1]
            demanderID = contents[2]

            if responderID == self.id:
                return
            if demanderID != self.id:
                return
            if responderID in [inv["id"] for inv in self.roombaState["teamInventories"]]:
                return

            inventory = data.split(",")
            if len(inventory) != 7:
                return
            inventory = list(map(int, inventory))
            self.roombaState["teamInventories"].append(
                {
                    "linemate": inventory[0],
                    "deraumere": inventory[1],
                    "sibur": inventory[2],
                    "mendiane": inventory[3],
                    "phiras": inventory[4],
                    "thystame": inventory[5],
                    "food": inventory[6],
                    "id": responderID,
                }
            )

        except Exception:
            self.logger.error(f"Invalid contents of inventory message: {rest.strip()}")

    def handleMessageComeIncant(self, direction: int, rest: str):
        id = rest.strip()
        if id == self.id:
            return

        data = id.split("-")
        if not len(data) == 2:
            self.logger.error(f"Invalid contents in comeIncant message: {rest.strip()}")
            return
        _, nb = data
        if not nb.isdigit():
            self.logger.error(f"Invalid contents in comeIncant message: {rest.strip()}")
            return

        sender_pid = int(nb)

        if sender_pid > self.pid:
            if self.incantationState["status"]:
                self.incantationState["status"] = False
                self.incantationState["phase"] = "sendComeIncant"
                self.incantationState["lastCommand"] = None
                self.incantationState["playerResponses"] = []

            self.goToIncantationState["status"] = True
            self.goToIncantationState["direction"] = direction
            self.goToIncantationState["arrived"] = (direction == 0)
            self.goToIncantationState["steps"] = []
            self.goToIncantationState["movementStarted"] = False
            self.goToIncantationState["droppingStones"] = False

    def handleMessageIsEveryBodyHere(self, direction: int, rest: str) -> None:
        id = rest.strip()
        if id == self.id:
            return

        if not self.goToIncantationState["status"]:
            return

        self.goToIncantationState["arrived"] = (direction == 0)
        self.goToIncantationState["direction"] = direction
        self.goToIncantationState["steps"] += self.getStepsFromDirection()
        self.goToIncantationState["droppingStones"] = False

        if (
            self.goToIncantationState["arrived"] and 
            len(self.goToIncantationState["steps"]) == 0 and
            self.goToIncantationState["direction"] == 0
        ):
            self.broadcaster.broadcastMessage(f"iAmHere {self.id} {id}")

    def handleMessageIAmHere(self, direction: int, rest: str) -> None:
        contents = rest.split(" ")
        if len(contents) != 2:
            self.logger.error(f"Invalid contents in iAmHere message: {rest.strip()}")
            return

        responderID = contents[0]
        demanderID = contents[1]

        if responderID == self.id:
            return
        if demanderID != self.id:
            return

        for response in self.incantationState["playerResponses"]:
            if response["id"] == responderID:
                return

        self.incantationState["playerResponses"].append({
            "id": responderID
        })

    def handleMessageDropStones(self, direction: int, rest: str) -> None:
        id = rest.strip()
        if id == self.id:
            return

        if (
            self.goToIncantationState["status"] and
            self.goToIncantationState["arrived"] and
            self.goToIncantationState["direction"] == 0
        ):
            self.goToIncantationState["droppingStones"] = True

    def handleMessages(self, direction: int, message: str) -> None:
        switcher = {
            "teamslots ": self.handleMessageTeamslots,
            "sendInventory ": self.handleMessageSendInventory,
            "inventory ": self.handleMessageInventory,
            "comeIncant ": self.handleMessageComeIncant,
            "isEveryBodyHere ": self.handleMessageIsEveryBodyHere,
            "iAmHere ": self.handleMessageIAmHere,
            "dropStones ": self.handleMessageDropStones,
        }

        for key in switcher.keys():
            if message.startswith(key):
                rest = message[len(key):].strip()
                if rest:
                    switcher[key](direction, rest)
                else:
                    switcher[key]()
                return

        self.logger.error(f"Unknown message: {message.strip()}")

    def loop(self) -> None:
        try:
            if self.nbTeamSlots != -1:
                self.sentNbSlots = False
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

                if not self.sentNbSlots:
                    if self.nbConnectedPlayers >= self.nbTeamSlots:
                        self.broadcaster.broadcastMessage(f"teamslots {self.nbTeamSlots}")
                        self.sentNbSlots = True
                    else:
                        self.communication.sendGetConnectNbr()
                        sleep(0.1)
                        continue

                if (
                    not self.inIncantation and
                    not self.communication.hasRequests() and
                    not self.communication.hasPendingCommands() and
                    not self.communication.hasResponses() and
                    not self.communication.hasMessages()
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
