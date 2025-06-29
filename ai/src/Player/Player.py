#
# EPITECH PROJECT, 2025
# zappy
# File description:
# player.py
#

import os
import sys
import signal
from threading import Thread
from typing import Callable
import time
from time import sleep

from src.Broadcaster.Broadcaster import Broadcaster
from src.Exceptions.Exceptions import (
    CommunicationException,
    SocketException
)
from src.Communication.Communication import Communication
from src.Config.Constants import (
    SUCCESS,
    FAILURE
)
from src.Config.GameConfig import (
    LVL_UPGRADES,
    MAX_LEVEL,
    TOTAL_NEEDED_STONES,
    ELEVATION_COST,
    FOOD_VALUE,
    MIN_FOOD_FOR_INCANTATION
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

        self.teamMatesStatus: list = []

        self.roombaState: dict = {
            "forwardCount": 0,
            "targetForward": 10,
            "phase": "lookAround",
            "lastCommand": None,
            "lastPhase": None,
            "commandSentTime": 0,
            "highestPidSeen": 0,
        }

        self.incantationState: dict = {
            "status": False,
            "phase": "sendComeIncant",
            "lastCommand": None,
            "timeout": 10,
            "playerResponses": [],
            "startTime": None,
        }

        self.goToIncantationState: dict = {
            "status": False,
            "steps": [],
            "lastCommand": None,
            "direction": 0,
            "arrived": False,
            "movementStarted": False,
            "droppingStones": False,
            "needToWait": False,
        }
        self.teamHasEnoughStones = False

        self.commandsToSend: list[tuple[Callable, str]] = []
        self.lastCommandSent: str = None

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

    def _child_signal_handler(self, signum, frame):
        try:
            if hasattr(self, 'communication') and self.communication:
                self.communication.stopLoop()
        except Exception:
            pass
        sys.stdout.flush()
        sys.stderr.flush()
        os._exit(SUCCESS)

    def startComThread(self) -> None:
        self.commThread.start()

    def setMapSize(self, x: int, y: int) -> None:
        self.x = x
        self.y = y
        self.roombaState["targetForward"] = max(x, y)

    def setNbSlots(self, slots: int) -> None:
        self.nbTeamSlots = slots

    def start(self):
        try:
            if self.is_child_process:
                signal.signal(signal.SIGINT, self._child_signal_handler)
                signal.signal(signal.SIGTERM, self._child_signal_handler)

            slots, x, y = self.communication.connectToServer()
            self.setMapSize(x, y)

            if not self.is_child_process:
                self.setNbSlots(slots + 1)

            self.startComThread()

            self.loop()

            return SUCCESS

        except (CommunicationException, SocketException):
            return FAILURE
        except KeyboardInterrupt:
            return SUCCESS
        except Exception:
            return FAILURE

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
        for inventory in self.teamMatesStatus:
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
        self.logger.debug(f"Inventory {inventory} is sufficient for level {self.level}")
        self.teamHasEnoughStones = True
        return True

    def hasEnoughStonesLocally(self) -> bool:
        """Check if current player has enough stones for their level"""
        if self.level >= MAX_LEVEL:
            return True
        required_stones = LVL_UPGRADES[self.level]["stones"]
        for stone, required_quantity in required_stones.items():
            if self.inventory.get(stone, 0) < required_quantity:
                return False
        return True

    def howManyTimeUnitsForIncantation(self, level: int) -> int:
        return ELEVATION_COST

    def enoughFoodForIncantation(self, level: int, nbFood: int) -> bool:
        if level >= MAX_LEVEL:
            return True

        # Ensure minimum food requirement is met
        if nbFood < MIN_FOOD_FOR_INCANTATION:
            return False

        offset = FOOD_VALUE

        return nbFood * FOOD_VALUE > self.howManyTimeUnitsForIncantation(level) + offset

    def enoughFoodForGoToIncantation(self, level: int, nbFood: int) -> bool:
        if level >= MAX_LEVEL:
            return True

        # Ensure minimum food requirement is met
        self.logger.error(f"Checking food for go to incantation: level {level}, nbFood {nbFood}")
        if nbFood < 20:
            return False

        stonesToDrop = sum(LVL_UPGRADES[level]["stones"].values())
        droppingCost = stonesToDrop * 7

        maxLength = (max(self.x, self.y) / 2) * 20
        movementCost = 7 * maxLength

        offset = FOOD_VALUE * 2

        totalCost = droppingCost + movementCost + offset
        self.logger.error(f"nbfood {nbFood} * {FOOD_VALUE} > {totalCost} + {self.howManyTimeUnitsForIncantation(level)}")
        return nbFood * FOOD_VALUE > (totalCost + self.howManyTimeUnitsForIncantation(level))

    def teamHasEnoughFoodForGoToIncantation(self) -> bool:
        # Check self first with minimum food requirement
        if self.inventory["food"] < 20:
            return False
            
        if not self.enoughFoodForGoToIncantation(self.level, self.inventory["food"]):
            return False

        for teamMate in self.teamMatesStatus:
            # Check minimum food requirement for each teammate
            if teamMate["food"] < 20:
                return False
            if not self.enoughFoodForGoToIncantation(teamMate["level"], teamMate["food"]):
                return False
        self.logger.debug("Team has enough food for go to incantation")
        return True

    def teamHasEnoughFoodForIncantation(self) -> bool:
        # Check self first with minimum food requirement
        if self.inventory["food"] < 20:
            return False
            
        if not self.enoughFoodForIncantation(self.level, self.inventory["food"]):
            return False

        for teamMate in self.teamMatesStatus:
            # Check minimum food requirement for each teammate
            if teamMate["food"] < 20:
                return False
            if not self.enoughFoodForIncantation(teamMate["level"], teamMate["food"]):
                return False

        return True

    def roombaAction(self) -> None:
        phase = self.roombaState["phase"]

        # self.logger.debug(f"Roomba phase: {self.roombaState['phase']}")
        if phase == "lookAround":
            self.commandsToSend.append(
                (lambda: self.communication.sendLook(), "look")
            )
            self.roombaState["lastCommand"] = "look"
            self.roombaState["phase"] = "vacuum"
            self.roombaState["lastPhase"] = "lookAround"

        elif phase == "vacuum":
            self.look = self.communication.getLook() or self.look
            self.look = self.communication.getLook() or self.look
            if self.look and len(self.look) > 0:
                for item, quantity in self.look[0].items():
                    if item == "player" or item == "egg":
                        continue
                    if self.teamHasEnoughStones and item != "food":
                        continue
                    
                    # Check if we already have enough of this stone for all incantations
                    if item in TOTAL_NEEDED_STONES:
                        current_amount = self.inventory.get(item, 0)
                        total_needed = TOTAL_NEEDED_STONES[item]
                        if current_amount >= total_needed:
                            continue  # Skip picking up this stone type
                        
                        # Only pick up what we need
                        remaining_needed = total_needed - current_amount
                        quantity_to_take = min(quantity, remaining_needed)
                    else:
                        # For food or other items, take all
                        quantity_to_take = quantity
                    
                    for _ in range(quantity_to_take):
                        self.commandsToSend.append(
                            (
                                lambda obj=item: self.communication.sendTakeObject(obj),
                                f"take {item}"
                            )
                        )
            self.roombaState["lastCommand"] = "take"
            self.roombaState["phase"] = "forward"
            self.roombaState["lastPhase"] = "vacuum"

        elif phase == "updateInventory":
            self.commandsToSend.append(
                (lambda: self.communication.sendInventory(), "inventory")
            )
            self.roombaState["lastCommand"] = "inventory"
            self.roombaState["phase"] = "forward"
            self.roombaState["lastPhase"] = "updateInventory"

        elif phase == "checkOnTeammates":
            if self.roombaState["lastCommand"] == "Connect_nbr":
                # Only broadcast inventory request if we have enough stones locally and enough food
                if (self.hasEnoughStonesLocally() and 
                    self.inventory["food"] >= MIN_FOOD_FOR_INCANTATION and
                    self.enoughFoodForGoToIncantation(self.level, self.inventory["food"])):
                    self.commandsToSend.append(
                        (
                            lambda: self.broadcaster.broadcastMessage(f"sendInventory {self.id}"),
                            "broadcast sendInventory"
                        )
                    )
                    self.roombaState["lastCommand"] = "broadcast sendInventory"
                else:
                    # Not ready for incantation, go back to collecting
                    self.roombaState["phase"] = "forward"
                    self.roombaState["lastPhase"] = "checkOnTeammates"

            elif self.roombaState["lastCommand"] == "broadcast sendInventory":
                expectedTeamMates = self.nbConnectedPlayers - 1
                receivedTeamMatesStatus = len(self.teamMatesStatus)
                print(f"Received team mates in send inv status: {receivedTeamMatesStatus} / {expectedTeamMates}")
                if receivedTeamMatesStatus >= expectedTeamMates:
                    self.logger.error("Enough incenter response")
                    if (
                        self.doesTeamHaveEnoughStones() and
                        self.teamHasEnoughFoodForGoToIncantation()
                    ):
                        self.logger.info(
                            "Team has enough stones and food for incantation, proceeding")
                        highest_pid = self.pid
                        highest_pid_id = self.id

                        for teamMate in self.teamMatesStatus:
                            teammate_id = teamMate["id"]
                            if "-" in teammate_id:
                                teammate_pid = int(teammate_id.split("-")[1])
                                if teammate_pid > highest_pid:
                                    highest_pid = teammate_pid
                                    highest_pid_id = teammate_id

                        if highest_pid == self.pid:
                            self.logger.info(f"I am the leader (PID: {self.pid}), starting incantation")
                            self.incantationState["status"] = True
                            self.incantationState["phase"] = "sendComeIncant"
                            return  # Exit roomba action to start incantation
                        else:
                            self.logger.info(f"Leader is {highest_pid_id} (PID: {highest_pid}), broadcasting leadIncantation")
                            self.commandsToSend.append(
                                (
                                    lambda: self.broadcaster.broadcastMessage(
                                        f"leadIncantation {highest_pid_id}"
                                    ),
                                    "broadcast leadIncantation"
                                )
                            )
                            self.roombaState["lastCommand"] = "broadcast leadIncantation"
                    self.roombaState["phase"] = "forward"
                    self.roombaState["lastPhase"] = "checkOnTeammates"
                else:
                    self.commandsToSend.append(
                        (lambda: self.communication.sendGetConnectNbr(), "Connect_nbr")
                    )
                    self.roombaState["lastCommand"] = "Connect_nbr"

            else:
                self.commandsToSend.append(
                    (lambda: self.communication.sendGetConnectNbr(), "Connect_nbr")
                )
                self.roombaState["lastCommand"] = "Connect_nbr"

        elif phase == "forward":
            if self.roombaState["forwardCount"] < self.roombaState["targetForward"]:
                self.commandsToSend.append(
                    (lambda: self.communication.sendForward(), "forward")
                )
                self.roombaState["lastCommand"] = "forward"
                self.roombaState["forwardCount"] += 1
                self.roombaState["phase"] = "lookAround"
                self.roombaState["lastPhase"] = "forward"

            else:
                self.roombaState["forwardCount"] = 0
                self.commandsToSend.append(
                    (lambda: self.communication.sendRight(), "right")
                )
                self.roombaState["lastCommand"] = "right"
                self.roombaState["phase"] = "turn"
                self.roombaState["lastPhase"] = "forward"

        elif phase == "turn":
            if self.roombaState["lastCommand"] == "right":
                self.commandsToSend.append(
                    (lambda: self.communication.sendForward(), "forward")
                )
                self.roombaState["lastCommand"] = "forward"

            elif self.roombaState["lastCommand"] == "forward":
                self.commandsToSend.append(
                    (lambda: self.communication.sendLeft(), "left")
                )
                self.roombaState["lastCommand"] = "left"
                self.roombaState["phase"] = "lookAround"
                self.roombaState["lastPhase"] = "turn"

            else:
                self.logger.error(
                    f"Unexpected lastCommand '{self.roombaState['lastCommand']}'"
                    f" in turn phase, resetting to lookAround"
                )
                self.roombaState["phase"] = "lookAround"
                self.roombaState["lastPhase"] = "turn"

        else:
            self.logger.error(f"Unknown roomba phase '{phase}', resetting to lookAround")
            self.roombaState["phase"] = "lookAround"
            self.roombaState["lastPhase"] = "unknown"

    def incantationAction(self) -> None:
        if self.level >= MAX_LEVEL:
            self.incantationState["status"] = False
            self.roombaState["phase"] = "forward"
            return

        phase = self.incantationState["phase"]

        if phase == "sendComeIncant":
            self.commandsToSend.append(
                (
                    lambda: self.broadcaster.broadcastMessage(f"comeIncant {self.id}"),
                    "broadcast comeIncant"
                )
            )
            self.incantationState["lastCommand"] = "broadcast comeIncant"
            self.incantationState["lastPhase"] = "sendComeIncant"
            self.incantationState["phase"] = "sendConnectNbr"

        elif phase == "sendConnectNbr":
            self.commandsToSend.append(
                (lambda: self.communication.sendGetConnectNbr(), "Connect_nbr")
            )
            self.incantationState["lastCommand"] = "Connect_nbr"
            self.incantationState["lastPhase"] = "sendConnectNbr"
            self.incantationState["phase"] = "waitForWhereAreYou"
            self.incantationState["playerResponses"] = []

        elif phase == "waitForWhereAreYou":
            # Initialize timeout on first entry to this phase
            if self.incantationState["startTime"] is None:
                self.incantationState["startTime"] = time.time()
            
            # Check for timeout
            elapsed_time = time.time() - self.incantationState["startTime"]
            if elapsed_time > self.incantationState["timeout"]:
                self.logger.error("Incantation regroup timed out, broadcasting goRoombas and resetting")
                self.commandsToSend.append(
                    (
                        lambda: self.broadcaster.broadcastMessage("goRoombas"),
                        "broadcast goRoombas"
                    )
                )
                self.incantationState["status"] = False
                self.incantationState["phase"] = "sendComeIncant"
                self.incantationState["lastCommand"] = None
                self.incantationState["playerResponses"] = []
                self.incantationState["startTime"] = None
                self.teamMatesStatus = []
                self.roombaState["phase"] = "forward"
                return
            
            if self.incantationState["lastCommand"] != "look":
                self.commandsToSend.append(
                    (lambda: self.communication.sendLook(), "look")
                )
                self.incantationState["lastCommand"] = "look"
            else:
                # Check if we have a look response
                self.look = self.communication.getLook() or self.look
                if self.look and len(self.look) > 0:
                    # Count players in the first tile (current position)
                    playersOnCurrentTile = self.look[0].get("player", 0)
                    self.logger.debug(f"Nb players on current tile: {playersOnCurrentTile} / {self.nbConnectedPlayers}")
                    if playersOnCurrentTile >= self.nbConnectedPlayers:
                        self.logger.debug("All players are on the same tile, proceeding with incantation")
                        self.incantationState["phase"] = "dropStones"
                        self.incantationState["startTime"] = None  # Reset timeout
                    else:
                        # Wait a bit and look again
                        self.incantationState["lastCommand"] = None

        elif phase == "dropStones":
            self.commandsToSend.append(
                (
                    lambda: self.broadcaster.broadcastMessage(f"dropStones {self.id}"),
                    f"broadcast dropStones {self.id}"
                )
            )
            self.incantationState["lastCommand"] = "broadcast dropStones"
            self.logger.debug("Broadcasting dropStones command")
            self.incantationState["phase"] = "droppingStones"
            self.logger.debug("Moving to droppingStones phase")

        elif phase == "droppingStones":
            # Drop all required stones for current level incantation
            self.logger.debug("Dropping stones for incantation")
            required_stones = LVL_UPGRADES[self.level]["stones"]
            
            for stone, required_quantity in required_stones.items():
                available_quantity = self.inventory.get(stone, 0)
                # Drop all available stones of this type (up to required amount)
                quantity_to_drop = min(available_quantity, required_quantity)
                self.logger.debug(f"Dropping {quantity_to_drop} of {stone} (available: {available_quantity}, required: {required_quantity})")
                for _ in range(quantity_to_drop):
                    self.logger.debug(f"Dropping stone: {stone}")
                    self.commandsToSend.append(
                        (
                            lambda obj=stone: self.communication.sendSetObject(obj),
                            f"set {stone}"
                        )
                    )

            # Always move to next phase after dropping stones
            self.incantationState["lastCommand"] = "drop stones"
            self.incantationState["phase"] = "updateInventory"


        elif phase == "updateInventory":
            self.commandsToSend.append(
                (
                    lambda: self.communication.sendInventory(),
                    "inventory"
                )
            )
            self.incantationState["phase"] = "inventoryCheck"
            self.incantationState["lastCommand"] = "inventory"

        elif phase == "inventoryCheck":
            if self.incantationState["lastCommand"] == "Connect_nbr":
                self.commandsToSend.append(
                    (
                        lambda: self.broadcaster.broadcastMessage(f"sendInventory {self.id}"),
                        f"broadcast sendInventory {self.id}"
                    )
                )
                self.incantationState["lastCommand"] = "broadcast sendInventory"

            elif self.incantationState["lastCommand"] == "broadcast sendInventory":
                expectedTeamMates = self.nbConnectedPlayers - 1
                receivedTeamMatesStatus = len(self.teamMatesStatus)

                print(f"Received team mates status: {receivedTeamMatesStatus} / {expectedTeamMates}")
                if receivedTeamMatesStatus >= expectedTeamMates:
                    if not self.teamHasEnoughFoodForIncantation():
                        self.logger.error("Not enough food for incantation, resetting state")
                        self.commandsToSend.append(
                            (
                                lambda: self.broadcaster.broadcastMessage("goRoombas"),
                                "broadcast goRoombas"
                            )
                        )
                        self.incantationState["status"] = False
                        self.incantationState["phase"] = "sendComeIncant"
                        self.incantationState["lastCommand"] = None
                        self.incantationState["playerResponses"] = []
                        self.roombaState["phase"] = "forward"

                    else:
                        self.incantationState["phase"] = "startIncantation"

                else:
                    print("in connect nbr phase, not enough team mates responses")
                    self.commandsToSend.append(
                        (lambda: self.communication.sendGetConnectNbr(), "Connect_nbr")
                    )
                    self.incantationState["lastCommand"] = "Connect_nbr"

            else:
                print("in connect nbr phase, waiting for responses")
                self.commandsToSend.append(
                    (lambda: self.communication.sendGetConnectNbr(), "Connect_nbr")
                )
                self.incantationState["lastCommand"] = "Connect_nbr"

        elif phase == "startIncantation":
            self.commandsToSend.append(
                (lambda: self.communication.sendIncantation(), "incantation")
            )
            self.incantationState["lastCommand"] = "incantation"
            self.incantationState["phase"] = "waitingForElevation"

        elif phase == "waitingForElevation":
            pass

    def goToIncantationAction(self) -> None:
        if self.level >= MAX_LEVEL:
            self.goToIncantationState["status"] = False
            self.roombaState["phase"] = "forward"
            return

        if self.goToIncantationState["droppingStones"]:
            # Drop all required stones for current level incantation
            required_stones = LVL_UPGRADES[self.level]["stones"]
            stones_dropped = False
            
            for stone, required_quantity in required_stones.items():
                available_quantity = self.inventory.get(stone, 0)
                # Drop all available stones of this type (up to required amount)
                quantity_to_drop = min(available_quantity, required_quantity)
                
                for _ in range(quantity_to_drop):
                    self.commandsToSend.append(
                        (
                            lambda obj=stone: self.communication.sendSetObject(obj),
                            f"set {stone}"
                        )
                    )
                    stones_dropped = True
            if stones_dropped:
                self.incantationState["lastCommand"] = "drop stones"
                self.incantationState["phase"] = "startIncantation"
            else:
                self.incantationState["phase"] = "updateInventory"
                self.incantationState["lastCommand"] = "inventory"
            # self.goToIncantationState["droppingStones"] = False
            # self.goToIncantationState["lastCommand"] = "drop stones"
            
            # if stones_dropped:
            #     self.commandsToSend.append(
            #         (lambda: self.communication.sendInventory(), "inventory")
            #     )
            return

        if (
            self.goToIncantationState["direction"] == 0 and
            self.goToIncantationState["movementStarted"]
        ):
            self.goToIncantationState["arrived"] = True
            # Use look instead of broadcasting whereAreYou
            if self.goToIncantationState["lastCommand"] != "look":
                self.commandsToSend.append(
                    (lambda: self.communication.sendLook(), "look")
                )
                self.goToIncantationState["lastCommand"] = "look"
            else:
                # Check if we have a look response
                self.look = self.communication.getLook() or self.look
                if self.look and len(self.look) > 0:
                    # Count players in the first tile (current position)
                    playersOnCurrentTile = self.look[0].get("player", 0)
                    self.logger.debug(f"GoToIncant - Nb players on current tile: {playersOnCurrentTile} / {self.nbConnectedPlayers}")
                    if playersOnCurrentTile >= self.nbConnectedPlayers:
                        self.logger.debug("All players are on the same tile, proceeding with stone dropping")
                        self.goToIncantationState["droppingStones"] = True
                    else:
                        # Wait a bit and look again
                        self.goToIncantationState["lastCommand"] = None
            return

        if len(self.goToIncantationState["steps"]) == 0:
            # Use look instead of broadcasting whereAreYou when no steps remaining
            if self.goToIncantationState["lastCommand"] != "look":
                self.commandsToSend.append(
                    (lambda: self.communication.sendLook(), "look")
                )
                self.goToIncantationState["lastCommand"] = "look"
            else:
                # Check if we have a look response
                self.look = self.communication.getLook() or self.look
                if self.look and len(self.look) > 0:
                    # Count players in the first tile (current position)
                    playersOnCurrentTile = self.look[0].get("player", 0)
                    self.logger.debug(f"GoToIncant (no steps) - Nb players on current tile: {playersOnCurrentTile} / {self.nbConnectedPlayers}")
                    if playersOnCurrentTile >= self.nbConnectedPlayers:
                        self.logger.debug("All players are on the same tile, proceeding with stone dropping")
                        self.goToIncantationState["droppingStones"] = True
                    else:
                        # Wait a bit and look again
                        self.goToIncantationState["lastCommand"] = None
            return

        self.goToIncantationState["movementStarted"] = True

        step = self.goToIncantationState["steps"].pop(0)
        self.commandsToSend.append(
            (
                lambda: step(),
                "step"
            )
        )
        self.goToIncantationState["lastCommand"] = "step"

    def handleResponseInventory(self) -> None:
        newInventory = self.communication.getInventory()

        if not self.needToBroadcastInventory:
            for item in newInventory.keys():
                if (
                    newInventory[item] <= self.inventory[item] or
                    self.incantationState["status"] or
                    self.goToIncantationState["status"] or
                    self.roombaState["lastPhase"] != "updateInventory" or
                    self.nbTeamSlots == -1
                ):
                    continue
                # Only trigger checkOnTeammates if we have enough stones locally and food to justify the exchange
                if (
                    self.hasEnoughStonesLocally() and
                    self.inventory["food"] >= MIN_FOOD_FOR_INCANTATION and
                    self.enoughFoodForGoToIncantation(self.level, newInventory["food"])
                ):
                    self.roombaState["phase"] = "checkOnTeammates"
                    break

        self.inventory = newInventory or self.inventory

        if self.needToBroadcastInventory:
            inventoryToSend = (
                "inventory "
                f"{self.inventory['linemate']},"
                f"{self.inventory['deraumere']},"
                f"{self.inventory['sibur']},"
                f"{self.inventory['mendiane']},"
                f"{self.inventory['phiras']},"
                f"{self.inventory['thystame']},"
                f"{self.inventory['food']},"
                f"{self.level} "
                f"{self.id} {self.senderID}"
            )
            print(f"Invetory that will be broadcasted: {inventoryToSend}")
            self.commandsToSend.append(
                (
                    lambda: self.broadcaster.broadcastMessage(inventoryToSend),
                    f"broadcast {inventoryToSend}"
                )
            )
            self.needToBroadcastInventory = False

    def handleResponseLook(self) -> None:
        self.look = self.communication.getLook() or self.look

    def handleResponseKO(self) -> None:
        # self.logger.error(f"Command '{self.lastCommandSent}' failed")

        if self.roombaState["phase"] == "checkOnTeammates":
            self.roombaState["phase"] = "forward"

        # Handle incantation failure for leader
        if self.incantationState["status"] and self.lastCommandSent == "incantation":
            self.logger.error("Incantation failed, broadcasting goRoombas and resetting")
            self.commandsToSend.append(
                (
                    lambda: self.broadcaster.broadcastMessage("goRoombas"),
                    "broadcast goRoombas"
                )
            )
            self.incantationState["status"] = False
            self.incantationState["phase"] = "vacuum"
            self.incantationState["lastCommand"] = None
            self.incantationState["playerResponses"] = []
            self.teamMatesStatus = []  # Clear to force fresh check
            self.roombaState["phase"] = "forward"
            self.inIncantation = False

        # Handle general incantation failure
        elif self.incantationState["status"] and self.inIncantation:
            self.logger.error("Incantation failed, going roomba mode")
            self.incantationState["status"] = False
            self.incantationState["phase"] = "vacuum"
            self.incantationState["lastCommand"] = None
            self.incantationState["playerResponses"] = []
            self.teamMatesStatus = []  # Clear to force fresh check
            self.roombaState["phase"] = "forward"
            self.inIncantation = False

        # Handle goToIncantation failure
        if self.goToIncantationState["status"] and self.inIncantation:
            self.logger.error("Go to incantation failed, going roomba mode")
            self.goToIncantationState["status"] = False
            self.goToIncantationState["arrived"] = False
            self.goToIncantationState["movementStarted"] = False
            self.goToIncantationState["steps"] = []
            self.goToIncantationState["droppingStones"] = False
            self.goToIncantationState["needToWait"] = False
            self.roombaState["phase"] = "forward"
            self.inIncantation = False

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

                # Reset all incantation states completely
                self.incantationState = {
                    "status": False,
                    "phase": "sendComeIncant",
                    "lastCommand": None,
                    "lastPhase": None,
                    "playerResponses": [],
                    "startTime": None,
                    "timeout": 10,
                }
                
                self.goToIncantationState = {
                    "status": False,
                    "direction": 0,
                    "steps": [],
                    "arrived": False,
                    "movementStarted": False,
                    "droppingStones": False,
                    "needToWait": False
                }
                
                # Clear teammate status to force fresh inventory check for new level
                self.teamMatesStatus = []
                self.teamHasEnoughStones = False
                if self.level >= 8:
                    self.logger.success("Player reached maximum level!")
                    self.incantationState["status"] = False
                    self.incantationState["phase"] = "sendComeIncant"
                    self.incantationState["lastCommand"] = None
                    self.incantationState["playerResponses"] = []
                    self.goToIncantationState["status"] = False
                    self.goToIncantationState["arrived"] = False
                    self.goToIncantationState["movementStarted"] = False
                    self.goToIncantationState["steps"] = []
                    self.goToIncantationState["droppingStones"] = False
                    self.roombaState["phase"] = "forward"
                else:
                    # Reset both incantation states and go back to roomba mode
                    self.incantationState["status"] = False
                    self.incantationState["phase"] = "sendComeIncant"
                    self.incantationState["lastCommand"] = None
                    self.incantationState["playerResponses"] = []
                    
                    self.goToIncantationState["status"] = False
                    self.goToIncantationState["arrived"] = False
                    self.goToIncantationState["movementStarted"] = False
                    self.goToIncantationState["steps"] = []
                    self.goToIncantationState["droppingStones"] = False
                    
                    # Clear teammate status to force fresh check for next level
                    self.teamMatesStatus = []
                    
                    # Go back to roomba mode
                    self.roombaState["phase"] = "forward"

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
        # self.logger.error(f"Unknown response to '{lastCommand}': {response.strip()}")

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
            # Only respond if we have enough stones locally and food to justify the exchange
            if (self.hasEnoughStonesLocally() and 
                self.inventory["food"] >= MIN_FOOD_FOR_INCANTATION and
                self.enoughFoodForGoToIncantation(self.level, self.inventory["food"])):
                self.teamMatesStatus = [mate for mate in self.teamMatesStatus if mate["id"] != id]
                self.senderID = id
                self.needToBroadcastInventory = True
                self.commandsToSend.append(
                    (
                        lambda: self.communication.sendInventory(),
                        "inventory"
                    )
                )

    def handleMessageInventory(self, direction: int, rest: str) -> None:
        self.logger.debug(f"Raw inventory message: '{rest}'")
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
            if responderID in [mate["id"] for mate in self.teamMatesStatus]:
                return

            teamMate = data.split(",")
            if len(teamMate) != 8:
                return
            teamMate = list(map(int, teamMate))
            self.logger.debug(f"Parsed inventory data: {teamMate}")
            self.teamMatesStatus.append(
                {
                    "linemate": teamMate[0],
                    "deraumere": teamMate[1],
                    "sibur": teamMate[2],
                    "mendiane": teamMate[3],
                    "phiras": teamMate[4],
                    "thystame": teamMate[5],
                    "food": teamMate[6],
                    "level": teamMate[7],
                    "id": responderID,
                }
            )

        except Exception:
            self.logger.error(f"Invalid contents of inventory message: {rest.strip()}")

    def handleMessageComeIncant(self, direction: int, rest: str):
        self.logger.debug(f"Received comeIncant message from {rest.strip()} in direction {direction}")
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
            self.goToIncantationState["steps"] = self.getStepsFromDirection()
            self.goToIncantationState["movementStarted"] = False
            self.goToIncantationState["droppingStones"] = False

            self.commandsToSend.append(
                (
                    lambda: self.broadcaster.broadcastMessage(f"whereAreYou {self.id}"),
                    f"broadcast whereAreYou {self.id}"
                )
            )

    def handleMessageDropStones(self, direction: int, rest: str) -> None:
        id = rest.strip()
        if id == self.id:
            return
        self.logger.debug(f"Received dropStones message from {id} in direction {direction}")
        
        # All players should drop stones when they receive this message
        if (
            self.goToIncantationState["status"] and
            self.goToIncantationState["arrived"]
        ):
            self.goToIncantationState["droppingStones"] = True
        
        # Also drop stones if we're in incantation state but not the leader
        elif not self.incantationState["status"]:
            # Drop required stones for current level incantation
            required_stones = LVL_UPGRADES[self.level]["stones"]
            
            for stone, required_quantity in required_stones.items():
                available_quantity = self.inventory.get(stone, 0)
                # Drop all available stones of this type (up to required amount)
                quantity_to_drop = min(available_quantity, required_quantity)
                
                for _ in range(quantity_to_drop):
                    self.commandsToSend.append(
                        (
                            lambda obj=stone: self.communication.sendSetObject(obj),
                            f"set {stone}"
                        )
                    )

    def handleMessageWhereAreYou(self, direction: int, rest: str) -> None:
        id = rest.strip()
        if id == self.id:
            return

        if not self.incantationState["status"]:
            return

        self.commandsToSend.append(
            (
                lambda: self.broadcaster.broadcastMessage(f"here {self.id} {id}"),
                f"broadcast here {self.id} {id}"
            )
        )

        for response in self.incantationState["playerResponses"]:
            if response["id"] == id:
                response["direction"] = direction
                return

        self.incantationState["playerResponses"].append({
            "id": id,
            "direction": direction
        })

    def handleMessageHere(self, direction: int, rest: str) -> None:
        contents = rest.split(" ")
        if len(contents) != 2:
            self.logger.error(f"Invalid contents in here message: {rest.strip()}")
            return

        responderID = contents[0]
        demanderID = contents[1]

        if responderID == self.id:
            return
        if demanderID != self.id:
            return

        if self.goToIncantationState["status"]:
            self.goToIncantationState["direction"] = direction
            self.goToIncantationState["steps"] = self.getStepsFromDirection()
            self.goToIncantationState["arrived"] = (direction == 0)
            self.goToIncantationState["movementStarted"] = False
            self.goToIncantationState["needToWait"] = False

    def handleMessageLeadIncantation(self, direction: int, rest: str) -> None:
        target_id = rest.strip()
        if target_id != self.id:
            return

        if (
            not self.goToIncantationState["status"] and
            not self.incantationState["status"]
        ):
            self.incantationState["status"] = True

    def handleMessageGoRoombas(self, direction: int) -> None:
        self.logger.debug("Received goRoombas message, resetting all states")

        if self.incantationState["status"]:
            self.incantationState["status"] = False
            self.incantationState["phase"] = "sendComeIncant"
            self.incantationState["lastCommand"] = None
            self.incantationState["playerResponses"] = []
            
        if self.goToIncantationState["status"]:
            self.goToIncantationState["status"] = False
            self.goToIncantationState["arrived"] = False
            self.goToIncantationState["movementStarted"] = False
            self.goToIncantationState["steps"] = []
            self.goToIncantationState["droppingStones"] = False
            self.goToIncantationState["needToWait"] = False
            
        # Clear teammate status to force fresh inventory check
        self.teamMatesStatus = []
        self.inIncantation = False
        
        # Reset to roomba mode
        self.roombaState["phase"] = "forward"
        self.roombaState["forwardCount"] = 0

    def handleMessages(self, direction: int, message: str) -> None:
    #     self.logger.debug(f"Received message: {message.strip()} in direction {direction}")
        switcher = {
            "teamslots ": self.handleMessageTeamslots,
            "sendInventory ": self.handleMessageSendInventory,
            "inventory ": self.handleMessageInventory,
            "comeIncant ": self.handleMessageComeIncant,
            "whereAreYou ": self.handleMessageWhereAreYou,
            "here ": self.handleMessageHere,
            "dropStones ": self.handleMessageDropStones,
            "leadIncantation ": self.handleMessageLeadIncantation,
            "goRoombas": self.handleMessageGoRoombas,
        }

        for key in switcher.keys():
            if message.startswith(key):
                rest = message[len(key):].strip()
                if rest:
                    switcher[key](direction, rest)
                else:
                    switcher[key](direction)
                return

        # self.logger.error(f"Unknown message: {message.strip()}")

    def sendCommands(self) -> None:
        if len(self.commandsToSend) == 0:
            return
        command, name = self.commandsToSend.pop(0)
        self.lastCommandSent = name
        command()

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
                    self.nbTeamSlots != -1 and
                    not self.inIncantation and
                    not self.communication.hasRequests() and
                    not self.communication.hasPendingCommands() and
                    not self.communication.hasResponses() and
                    not self.communication.hasMessages()
                ):
                    if len(self.commandsToSend) > 0:
                        self.sendCommands()
                    else:
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
