#
# EPITECH PROJECT, 2025
# zappy
# File description:
# player.py
#

import os
from threading import Thread
from time import sleep

from src.Hash.Hash import Hash
from src.Exceptions.Exceptions import (
    CommunicationException
)
from src.Communication.Communication import Communication
from src.Utils.Utils import SUCCESS, FAILURE

MIN_FOOD = 10

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

        self.level: int = 1
        self.hash: Hash = Hash(name)
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

        self.helpRequestCount: int = 0
        self.isInSurvivalMode: bool = False
        self.lastFoodCheck: int = 10

        self.isHelpingTeammate: bool = False
        self.helpTargetDirection: int = 0

    def __del__(self):
        self.communication.stopLoop()
        if self._commThread.is_alive():
            self._commThread.join()
        if self.communication:
            del self.communication

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

    def checkSurvivalStatus(self) -> None:
        currentFood = self.inventory.get("food", 0)

        if currentFood > self.lastFoodCheck and self.isInSurvivalMode:
            print(f"Got food! ({currentFood}) - End of survival mode")
            self.isInSurvivalMode = False
            self.helpRequestCount = 0
            self.lastFoodCheck = currentFood
            return

        if currentFood < 5 and not self.isInSurvivalMode:
            print(f"Survival mode current food : {currentFood}")
            self.isInSurvivalMode = True
            self.sendHelpRequest()
        elif currentFood < 5 and self.isInSurvivalMode:
            if self.helpRequestCount < 5:
                if self.roombaState["forwardCount"] % 10 == 0:
                    self.sendHelpRequest()
            else:
                self.dropStonesForSurvival()
        self.lastFoodCheck = currentFood

    def sendHelpRequest(self) -> None:
        self.helpRequestCount += 1
        helpMessage = f"help:{self.helpRequestCount}"

        self.communication.sendBroadcast(self.hash.hashMessage(helpMessage))

    def dropStonesForSurvival(self) -> None:
        dropPriority = ["thystame", "phiras", "mendiane", "sibur", "deraumere", "linemate"]

        for stone in dropPriority:
            if self.inventory.get(stone, 0) > 0:
                print(f"Survival mod critical: drop {stone}")
                self.communication.sendSetObject(stone)
                return

    def canHelpTeammate(self) -> bool:
        currentFood = self.inventory.get("food", 0)
        return currentFood > 6 and not self.isInSurvivalMode

    def getDirectionFromSound(self, direction: int) -> str:
        if direction == 0:
            return "self"

        # Zappy directions (from documentation):
        direction_map = {
            1: "forward",        # Forward
            2: "forward-right",  # Forward-Left
            3: "right",          # Left
            4: "back-right",     # Behind-Left
            5: "back",           # Behind
            6: "back-left",      # Behind-Right
            7: "left",           # Right
            8: "forward-left"    # Forward-Right
        }
        return direction_map.get(direction, "unknown")

    def startHelpingTeammate(self, direction: int) -> None:
        if not self.canHelpTeammate():
            return

        print(f"Starting help to this direction {direction}")
        self.isHelpingTeammate = True
        self.helpTargetDirection = direction

        self.communication.sendBroadcast(self.hash.hashMessage("coming_to_help"))

    def helpTeammateAction(self) -> bool:
        if not self.isHelpingTeammate:
            return True

        if self.helpTargetDirection == 0:
            if self.inventory.get("food", 0) > 6:
                print("Drop food for teammate")
                self.communication.sendSetObject("food")
                self.communication.sendBroadcast(self.hash.hashMessage("food_dropped"))
            else:
                print("Not engough food to help teammate")

            self.isHelpingTeammate = False
            self.helpTargetDirection = 0
            return True

        direction_str = self.getDirectionFromSound(self.helpTargetDirection)

        if direction_str in ["forward", "forward-right", "forward-left"]:
            self.communication.sendForward()
        elif direction_str == "right":
            self.communication.sendRight()
            self.communication.sendForward()
        elif direction_str == "left":
            self.communication.sendLeft()
            self.communication.sendForward()
        elif direction_str in ["back", "back-right", "back-left"]:
            self.communication.sendRight()
            self.communication.sendRight()
            self.communication.sendForward()
        elif direction_str == "self":
          return True
        else:
            self.communication.sendForward()

        return False

    def roombaAction(self) -> None:
        self.checkSurvivalStatus()

        if self.isHelpingTeammate:
            if self.helpTeammateAction():
                return

        if self.roombaState["phase"] == "forward":
            if self.roombaState["lastCommand"] in ("left", "forward", None):
                self.communication.sendLook()
                self.roombaState["lastCommand"] = "look"

            elif self.roombaState["lastCommand"] == "look":
                self.look = self.communication.getLook() or self.look
                if self.look:
                    if "food" in self.look[0].keys():
                        self.communication.sendTakeObject("food")
                    if not self.isInSurvivalMode or self.helpRequestCount < 3:
                        neededStones = self.getNeededStonesByPriority()
                        for stone in neededStones:
                            if stone in self.look[0].keys():
                                self.communication.sendTakeObject(stone)
                self.roombaState["lastCommand"] = "getObjects"

            elif self.roombaState["lastCommand"] == "getObjects":
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

    def handleCommandResponse(self, response: str) -> None:
        if response.strip() == "inventory":
            oldFood = self.inventory.get("food", 0)
            self.inventory = self.communication.getInventory()
            newFood = self.inventory.get("food", 0)

            if newFood != oldFood:
                print(f"Food: {oldFood} -> {newFood}")
                self.checkSurvivalStatus()

        if response.strip() == "look":
            self.look = self.communication.getLook()

        elif response.strip() == "ko":
            print(f"Command '{self.roombaState['lastCommand']}' failed")

        elif response.strip() == "ok":
            if self.roombaState['lastCommand'] in ["getObjects"]:
                self.communication.sendInventory()

    def loop(self) -> None:
        self.communication.sendInventory()
        stepCount = 0

        while not self.communication.playerIsDead():
            stepCount += 1

            if self.communication.hasMessages():
                message = self.communication.getLastMessage()
                direction = message[0]
                raw_message = str(message[1])
                raw_message = raw_message.strip()

                try:
                    response = self.hash.unHashMessage(raw_message)
                    print(f"Message received from the server (decrypted): {response}")
                except (ValueError, TypeError, Exception):
                    response = raw_message
                    if response.startswith("help:"):
                        try:
                            help_parts = response.split(":")
                            if len(help_parts) >= 2 and help_parts[1]:
                                help_number = help_parts[1]
                                print(
                                    f"Help receive from team: #{help_number}"
                                    f"from direction {direction}")
                                if self.canHelpTeammate():
                                    print(f"Start helping to {direction}")
                                    self.startHelpingTeammate(direction)
                                else:
                                    print(
                                        f"I can't help: "
                                        f"{self.inventory.get('food', 0)} food,"
                                        f" survival mode: {self.isInSurvivalMode}")
                            else:
                                print(f"Helping message error: {response}")
                        except Exception as e:
                            print(f"Error during message formating: {e}")
                    elif response == "coming_to_help":
                        if direction != 0:
                            print(
                                f"A teammate is coming to help in the direction "
                                f"{direction}")
                    elif response == "food_dropped":
                        if direction != 0:
                            print(
                                f"Food dropped by a teammate in the direction "
                                f"{direction}")
                    else:
                        print(f"Message error: {response}")

            if self.communication.hasResponses():
                response = self.communication.getLastResponse()
                if response.strip() == "dead":
                    break
                self.handleCommandResponse(response)

            if not self.communication.hasPendingCommands():
                self.roombaAction()
            if stepCount % 20 == 0:
                self.communication.sendInventory()
            sleep(0.1)
