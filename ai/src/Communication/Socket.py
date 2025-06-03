##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Socket
##

import socket
from src.Exceptions.Exceptions import (
    SocketException, PlayerDead
)

BUFFER_SIZE = 4096


class Socket:
    def __init__(self, host: str, port: int):
        self._host = host
        self._port = port
        self._address = (host, port)
        self._socket = None
        self._buffer = ""

    def connect(self):
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._socket.connect(self._address)

    def send(self, content: str):
        self._socket.sendall(content.encode("utf-8"))

    def receive(self) -> str:
        while '\n' not in self._buffer:
            data = self._socket.recv(BUFFER_SIZE)
            if not data:
                raise SocketException("Socket connection closed by the server")
            self._buffer += data.decode("utf-8")
        message, self._buffer = self._buffer.split('\n', 1)
        return message + '\n'

    def close(self):
        self._socket.close()
