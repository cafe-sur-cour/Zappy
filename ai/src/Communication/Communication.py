##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Communication
##

import select
import socket
import os
import threading
from time import sleep

from .Socket import Socket
from src.Exceptions.Exceptions import (
    CommunicationHandshakeException,
    CommunicationInvalidResponseException,
)


class Communication:
    def __init__(self, name: str, host: str, port: int):
        self._request_queue: list[str] = []
        self._pending_queue: list[str] = []

        self._response_buffer: str = ""

        self._message_queue: list[tuple[int, str]] = []
        self._isDead: bool = False
        self._lastInventory: dict[str, int] = {}
        self._lastLook: list[dict[str, int]] = []

        self._name = name
        self._host = host
        self._port = port

        self._socket = Socket(host, port)

        self.mutex = threading.Lock()

    def loop(self) -> None:
        while not self._isDead:
            with self.mutex:
                # Attention au surchargement
                # de la quest trop de donnée peuvent mettre du temp
                # s à traiter une donnée importante queue
                if len(self._request_queue) > 0 and len(self._pending_queue) < 10:
                    request = self._request_queue.pop(0)
                    self._socket.send(request)
                    self._pending_queue.append(request)
                else:
                    sleep(0.1)
            self.receive()

    def tryGetInventory(self, response: str) -> dict[str, int] | None:
        if not response[:-1].startswith("[") or not response[:-1].endswith("]"):
            return None

        items = response[:-1][1:-1].split(",")
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
        if not response[:-1].startswith("[") or not response[:-1].endswith("]"):
            return None

        look = []
        items = response[:-1][1:-1].split(",")
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
            self._isDead = True
            return ""
        elif response.startswith("message "):
            parts = response.split(" ")
            number = int(parts[1].strip(","))
            data = parts[2]
            self._message_queue.append((number, data))
            return ""
        inventory = self.tryGetInventory(response)
        if inventory is not None:
            self._lastInventory = inventory
            return "inventory\n"
        look = self.tryGetLook(response)
        if look is not None:
            self._lastLook = look
            return "look\n"
        return response

    def receive_data(self) -> str:
        with self.mutex:
            if '\n' in self._response_buffer:
                firstNewlineIndex = self._response_buffer.index('\n')
                data = self._response_buffer[:firstNewlineIndex + 1]
                self._response_buffer = self._response_buffer[firstNewlineIndex + 1:]
                return self.handleResponse(data.strip())
            r = self._socket.receive()
            self._response_buffer += r
            if '\n' in self._response_buffer:
                firstNewlineIndex = self._response_buffer.index('\n')
                data = self._response_buffer[:firstNewlineIndex + 1]
                self._response_buffer = self._response_buffer[firstNewlineIndex + 1:]
                return self.handleResponse(data.strip())
        return ""

    def receive(self) -> None:
        poller_object = select.poll()
        poller_object.register(self._socket.get_fd(), select.POLLIN)
        fd_vs_event = poller_object.poll(200)

        for fd, event in fd_vs_event:
            if self.receive_data() != "":  # TODO: Move this ?
                self._pending_queue.pop()

    def getInventory(self) -> dict[str, int]:
        with self.mutex:
            return self._lastInventory

    def getLook(self) -> list[str]:
        with self.mutex:
            return self._lastLook

    def get_size_message_queue(self) -> int:
        with self.mutex:
            return len(self._message_queue)

    def get_message_from_queue(self) -> tuple[int, str]:
        with self.mutex:
            if not self._message_queue:
                return 0, ""
            return self._message_queue.pop(0)

    def get_size_response_queue(self) -> int:
        with self.mutex:
            return len(self._response_buffer.split('\n'))

    def get_response_list(self) -> list[str]:
        with self.mutex:
            return self._response_buffer.split('\n')

    def is_dead(self) -> bool:
        with self.mutex:
            return self._isDead

    def connectToServer(self):
        self._socket.connect()
        response = self.receive_data()

        if response != "WELCOME":
            raise CommunicationInvalidResponseException(
                f"Invalid response from server handshake: {response}"
            )

        self._socket.send(f"{self._name}\n")
        response = self.receive_data()

        if response == "ko":
            raise CommunicationHandshakeException(
                f"Handshake failed: server return KO after sending: '{self._name}'"
            )
        slots = 0
        try:
            slots = int(response)
        except ValueError:
            raise CommunicationInvalidResponseException(
                f"Invalid number of slots: {response}"
            )

        response = self.receive_data()

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
            self._request_queue.append(message)

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
