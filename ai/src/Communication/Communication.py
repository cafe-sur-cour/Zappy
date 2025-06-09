##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Communication
##

import select
import threading
from time import sleep

from .Socket import Socket
from src.Exceptions.Exceptions import (
    CommunicationHandshakeException,
    CommunicationInvalidResponseException,
)


class Communication:
    def __init__(self, name: str, host: str, port: int):
        self.requestQueue: list[str] = []
        self.pendingQueue: list[str] = []
        self.responseQueue: list[str] = []

        self.responseBuffer: str = ""

        self.messageQueue: list[tuple[int, str]] = []
        self.playerDead: bool = False
        self.lastInventory: dict[str, int] = {}
        self.lastLook: list[dict[str, int]] = []

        self.name = name
        self.host = host
        self.port = port

        self.socket = Socket(host, port)

        self.mutex = threading.Lock()

    def loop(self) -> None:
        while not self.playerDead:
            with self.mutex:
                if len(self.requestQueue) > 0 and len(self.pendingQueue) < 10:
                    request = self.requestQueue.pop(0)
                    self.socket.send(request)
                    self.pendingQueue.append(request)
                else:
                    sleep(0.1)
            self.receive()

    def tryGetInventory(self, response: str) -> dict[str, int] | None:
        if not response.startswith("[") or not response.endswith("]"):
            return None

        items = response[1:-1].split(",")
        inventory = dict()
        for item in items:
            if not item:
                continue
            try:
                data = [d.strip() for d in item.strip().split(" ") if d and d.strip()]
                if len(data) != 2:
                    return None
                name, quantity = data
                if not name or not quantity:
                    return None
                inventory[name] = int(quantity)
            except ValueError:
                return None

        return inventory

    def tryGetLook(self, response: str) -> list[dict[str, int]] | None:
        if not response.startswith("[") or not response.endswith("]"):
            return None

        look = []
        items = response[1:-1].split(",")
        if not items or len(items) == 0:
            return None
        for item in items:
            look.append({})
            if not item:
                continue
            tile = item.strip().split(" ")
            tile = [d.strip() for d in tile if d and d.strip()]
            for data in tile:
                if not data:
                    continue
                if data not in look[-1]:
                    look[-1][data] = 0
                look[-1][data] += 1

        return look

    def handleResponse(self, response: str) -> str:
        if response.startswith("dead"):
            self.playerDead = True
            return "dead"
        if response.startswith("message "):
            parts = response.split(" ")
            number = int(parts[1].strip(","))
            data = parts[2]
            self.messageQueue.append((number, data))
            return ""
        inventory = self.tryGetInventory(response)
        if inventory is not None:
            self.lastInventory = inventory
            return "inventory"
        look = self.tryGetLook(response)
        if look is not None:
            self.lastLook = look
            return "look"
        return response

    def receiveData(self) -> str:
        with self.mutex:
            if '\n' not in self.responseBuffer:
                r = self.socket.receive()
                self.responseBuffer += r
            firstNewlineIndex = self.responseBuffer.index('\n')
            data = self.responseBuffer[:firstNewlineIndex + 1]
            self.responseBuffer = self.responseBuffer[firstNewlineIndex + 1:]
            return self.handleResponse(data.strip())
        return ""

    def receive(self) -> None:
        poller_object = select.poll()
        poller_object.register(self.socket.get_fd(), select.POLLIN)
        fd_vs_event = poller_object.poll(200)

        for fd, event in fd_vs_event:
            data = self.receiveData()
            if data != "":
                self.addResponse(data)
                self.pendingQueue.pop()

    def addResponse(self, response: str) -> None:
        with self.mutex:
            self.responseQueue.append(response)

    def hasReponses(self) -> bool:
        with self.mutex:
            return len(self.responseQueue) > 0

    def getInventory(self) -> dict[str, int]:
        with self.mutex:
            return self.lastInventory

    def getLook(self) -> list[dict[str, int]]:
        with self.mutex:
            return self.lastLook

    def lenMessageQueue(self) -> int:
        with self.mutex:
            return len(self.messageQueue)

    def hasMessages(self) -> bool:
        with self.mutex:
            return len(self.messageQueue) > 0

    def getLastMessage(self) -> tuple[int, str]:
        with self.mutex:
            if not self.messageQueue:
                return 0, ""
            return self.messageQueue.pop(0)

    def lenResponseQueue(self) -> int:
        with self.mutex:
            return len(self.responseBuffer.split('\n'))

    def getLastResponse(self) -> str:
        with self.mutex:
            return self.responseQueue.pop(0) if len(self.responseQueue) else ""

    def playerIsDead(self) -> bool:
        with self.mutex:
            return self.playerDead

    def hasPendingCommands(self) -> bool:
        with self.mutex:
            return len(self.pendingQueue) > 0

    def connectToServer(self):
        self.socket.connect()
        response = self.receiveData()

        if response != "WELCOME":
            raise CommunicationInvalidResponseException(
                f"Invalid response from server handshake: {response}"
            )

        self.socket.send(f"{self.name}\n")
        response = self.receiveData()

        if response == "ko":
            raise CommunicationHandshakeException(
                f"Handshake failed: server return KO after sending: '{self.name}'"
            )

        slots = 0
        try:
            slots = int(response)
        except ValueError:
            raise CommunicationInvalidResponseException(
                f"Invalid number of slots: {response}"
            )

        response = self.receiveData()

        x = 0
        y = 0
        try:
            x, y = map(int, response.split(" "))
        except ValueError:
            raise CommunicationInvalidResponseException(
                f"Invalid coordinates from server: {response}"
            )

        if slots < 0 or x < 0 or y < 0:
            raise CommunicationHandshakeException(
                f"Invalid handshake values: slots={slots}, x={x}, y={y}"
            )
        return slots, x, y

    def sendCommand(self, message: str) -> None:
        with self.mutex:
            self.requestQueue.append(message)

    def sendForward(self):
        self.sendCommand("Forward\n")

    def sendRight(self):
        self.sendCommand("Right\n")

    def sendLeft(self):
        self.sendCommand("Left\n")

    def sendLook(self) -> None:
        self.sendCommand("Look\n")

    def sendInventory(self) -> None:
        self.sendCommand("Inventory\n")

    def sendBroadcast(self, message: str):
        self.sendCommand(f"Broadcast {message}\n")

    def sendGetConnectNbr(self) -> None:
        self.sendCommand("Connect_nbr\n")

    def sendFork(self):
        self.sendCommand("Fork\n")

    def sendEject(self):
        self.sendCommand("Eject\n")

    def sendTakeObject(self, object_name: str):
        self.sendCommand(f"Take {object_name}\n")

    def sendSetObject(self, object_name: str):
        self.sendCommand(f"Set {object_name}\n")

    def sendIncantation(self):
        self.sendCommand("Incantation\n")
