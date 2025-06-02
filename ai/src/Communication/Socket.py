##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Socket
##

import socket

BUFFER_SIZE = 4096

class Socket:
    def __init__(self, host: str, port: int):
        self._host = host
        self._port = port
        self._address = (host, port)
        self._socket = None

    def connect(self):
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._socket.connect(self._address)

    def send(self, content: str):
        self._socket.sendall(content.encode("utf-8"))

    def receive(self) -> str:
        return self._socket.recv(BUFFER_SIZE).decode("utf-8")

    def close(self):
        self._socket.close()
