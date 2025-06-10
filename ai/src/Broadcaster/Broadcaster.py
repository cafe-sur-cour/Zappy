##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Broadcaster
##

from Communication.Communication import Communication
from Hash.Hash import Hash


class Broadcaster:
    def __init__(self, com: Communication, team: str) -> None:
        self.com = com
        self.hash = Hash(team)
        self.lastIndex = 0

    def unHashMessage(self, message: str) -> str:
        data = self.hash.unHashMessage(message.strip())
        contents = data.split("/")
        if len(contents) != 2:
            return ""
        message, index = contents
        index = int(index.strip())
        if index <= self.lastIndex:
            return ""
        self.lastIndex = index
        return message.strip()

    def broadcast(self, message: str) -> None:
        if not message:
            return
        hashedMessage = self.hash.hashMessage(message)
        if not hashedMessage:
            return
        self.lastIndex += 1
        hashedMessage += f"/{self.lastIndex}"
        self.com.sendBroadcast(hashedMessage)
