##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Communication
##

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
        self._name = name
        self._host = host
        self._port = port
        self._socket = Socket(host, port)

    def receive_data(self) -> str:
        r = self._socket.receive()
        if r == "dead\n":
            self._isDead = True
            return ""
        if r.startswith("message"):
            parts = r.split(" ")
            number = int(parts[1].strip(","))
            data = parts[2]
            self._message_queue.append((number, data))
            return self.receive_data()
        return r

    def get_size_message_queue(self) -> int:
        return len(self._message_queue)

    def get_message_from_queue(self) -> tuple[int, str]:
        if not self._message_queue:
            return 0, ""
        return self._message_queue.pop(0)

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

    def sendForward(self):
        self._socket.send("Forward\n")
        response = self.receive_data()

        if (response[:-1] != "ok"):
            raise CommunicationInvalidResponseException(
                f"Invalid response from Forward: {response[:-1]}"
            )

    def sendRight(self):
        self._socket.send("Right\n")
        response = self.receive_data()

        if (response[:-1] != "ok"):
            raise CommunicationInvalidResponseException(
                f"Invalid response from Right: {response[:-1]}"
            )

    def sendLeft(self):
        self._socket.send("Left\n")
        response = self.receive_data()

        if (response[:-1] != "ok"):
            raise CommunicationInvalidResponseException(
                f"Invalid response from Left: {response[:-1]}"
            )

    def getLook(self) -> list[dict[str, int]]:
        self._socket.send("Look\n")
        response = self.receive_data()

        if (not response[:-1].startswith("[") or not response[:-1].endswith("]")):
            raise CommunicationInvalidResponseException(
                f"Invalid response from Look: {response[:-1]}"
            )

        result = []
        items = response[:-1][1:-1].split(",")
        for item in items:
            result.append({})
            if not item:
                continue
            tile = item.strip().split(" ")
            tile = [d.strip() for d in tile if d and d.strip()]
            for data in tile:
                if not data:
                    continue
                if (data not in result[-1]):
                    result[-1][data] = 0
                result[-1][data] += 1
        return result

    def getInventory(self) -> dict[str, int]:
        self._socket.send("Inventory\n")
        response = self.receive_data()

        if (not response[:-1].startswith("[") or not response[:-1].endswith("]")):
            raise CommunicationInvalidResponseException(
                f"Invalid response from Inventory: {response[:-1]}"
            )

        items = response[:-1][1:-1].split(",")
        inventory = dict()
        for item in items:
            if not item:
                continue
            try:
                data = item.strip().split(" ")
                data = [d.strip() for d in data if d and d.strip()]
                if len(data) != 2:
                    raise ValueError("Invalid item format")
                name, quantity = data
                if not name or not quantity:
                    raise ValueError("Invalid item format")
                inventory[name] = int(quantity)
            except ValueError:
                raise CommunicationInvalidResponseException(
                    f"Invalid item format in Inventory: {item}"
                )

        return inventory

    def sendBroadcast(self, message: str):
        self._socket.send(f"Broadcast {message}\n")
        response = self.receive_data()

        if (response[:-1] != "ok"):
            raise CommunicationInvalidResponseException(
                f"Invalid response from Broadcast: {response[:-1]}"
            )

    def getCetConnectNbr(self) -> int:
        self._socket.send("Connect_nbr\n")
        response = self.receive_data()

        if (not response[:-1].isdecimal()):
            raise CommunicationInvalidResponseException(
                f"Invalid number: Got {response[:-1]} from command Connect_nbr"
            )

        return int(response[:-1])

    def sendFork(self):
        self._socket.send("Fork\n")
        response = self.receive_data()

        if (response[:-1] != "ok"):
            raise CommunicationInvalidResponseException(
                f"Invalid response from Fork: {response[:-1]}"
            )

    def sendEject(self):
        self._socket.send("Eject\n")
        response = self.receive_data()

        if (response[:-1] != "ok"):
            raise CommunicationInvalidResponseException(
                f"Invalid response from Eject: {response[:-1]}"
            )

    def sendTakeObject(self, object_name: str):
        self._socket.send(f"Take {object_name}\n")
        response = self.receive_data()

        if (response[:-1] != "ok"):
            raise CommunicationInvalidResponseException(
                f"Invalid response from Take: {response[:-1]}"
            )

    def sendSetObject(self, object_name: str):
        self._socket.send(f"Set {object_name}\n")
        response = self.receive_data()

        if (response[:-1] != "ok"):
            raise CommunicationInvalidResponseException(
                f"Invalid response from Set: {response[:-1]}"
            )

    def sendIncantation(self):
        self._socket.send("Incantation\n")
        response = self.receive_data()

        if (response[:-1] == "ko"):
            raise CommunicationInvalidResponseException(
                f"Invalid response from Incantation: {response[:-1]}"
            )

        if (response[:-1] == "Elevation underway"):
            return (response[:-1], 0)

        if (response[:-1].startswith("Current level: ")):
            try:
                level = int(response[:-1][len("Current level: "):])
                return (response[:-1], level)
            except ValueError:
                raise CommunicationInvalidResponseException(
                    f"Invalid level in Incantation response: {response[:-1]}"
                )
        raise CommunicationInvalidResponseException(
            f"Unexpected response from Incantation: {response[:-1]}"
        )
