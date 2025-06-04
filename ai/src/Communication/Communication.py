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

from .Socket import Socket
from src.Exceptions.Exceptions import (
    CommunicationHandshakeException,
    CommunicationInvalidResponseException,
)
from ..Exceptions.Exceptions import PlayerDead


class Communication:
    def __init__(self, name: str, host: str, port: int):
        self._isDead: bool = False
        self._message_queue: list[tuple[int, str]] = []
        self._request_queue: list[str] = []
        self._pending_queue: list[str] = []
        self._response_buffer: str = ""
        self._name = name
        self._host = host
        self._port = port
        self._socket = Socket(host, port)
        self.mutex = threading.Lock()

    def loop(self) -> None:
        while not self._isDead:
            with self.mutex:
                if len(self._request_queue) > 0 and len(self._pending_queue) < 10:
                    request = self._request_queue.pop(0)
                    self._socket.send(request)
                    self._pending_queue.append(request)
            self.receive()

    def receive_data(self) -> str:
        r = self._socket.receive()
        if r == "dead\n":
            with self.mutex:
                self._isDead = True
            return ""
        if r.startswith("message"):
            parts = r.split(" ")
            number = int(parts[1].strip(","))
            data = parts[2]
            with self.mutex:
                self._message_queue.append((number, data))
            self.receive_data()
            return ""
        with self.mutex:
            self._response_buffer += r
            for line in r.split('\n'):
                self._request_queue.append(line + "\n")
        return r

    def receive(self) -> None:
        poller_object = select.poll()
        poller_object.register(self._socket.get_fd(), select.POLLIN)
        fd_vs_event = poller_object.poll(200)

        for fd, event in fd_vs_event:
            print(f"event received: {event} on descriptor {fd}")
            self.receive_data()


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
        return self._isDead

    def connectToServer(self):
        self._socket.connect()
        response = self.receive_data()

        if (response[:-1] != "WELCOME"):
            raise CommunicationInvalidResponseException(
                f"Invalid response from server handshake: {response[:-1]}"
            )

        self._socket.send(f"{self._name}\n")
        response = self.receive_data()

        if response[:-1] == "ko":
            raise CommunicationHandshakeException(
                f"Handshake failed: server return KO after sending: '{self._name}'"
            )
        slots = 0
        try:
            slots = int(response[:-1])
        except ValueError:
            raise CommunicationInvalidResponseException(
                f"Invalid number of slots: {response[:-1]}"
            )

        response = self.receive_data()
        if (response[-1] != '\n'):
            raise CommunicationInvalidResponseException(
                f"Response from server after slots is not terminated with a newline"
            )

        x = 0
        y = 0
        try:
            x, y = map(int, response[:-1].split(" "))
        except ValueError:
            raise CommunicationInvalidResponseException(
                f"Invalid coordinates from server: {response[:-1]}"
            )

        if slots < 0 or x < 0 or y < 0:
            raise CommunicationHandshakeException(
                f"Invalid handshake values: slots={slots}, x={x}, y={y}"
            )
        return (slots, x, y)

    def sendCommand(self, message: str) -> None:
        with self.mutex:
            self._request_queue.append(f"{message}\n")

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
