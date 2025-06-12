##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Broadcaster
##

from src.Communication.Communication import Communication
from src.Hash.Hash import Hash


class Broadcaster:
    def __init__(self, com: Communication, team: str) -> None:
        self.com = com
        self.hash = Hash(team)
        self.lastIndex = 0

    def revealMessage(self, message: str) -> str:
        data = ""
        try:
            data = self.hash.unHashMessage(message.strip())
        except Exception as e:
            print(f"Error unhashing message: {e}")
            data = ""

        if not data:
            return ""

        if "/" not in data:
            return ""
        contents = data.split("/")
        if len(contents) != 2:
            return ""

        message, index = contents

        index = int(index.strip())
        if index <= self.lastIndex:
            return ""
        self.lastIndex = index

        return message.strip()

    def broadcastMessage(self, message: str) -> None:
        if not message:
            return

        message = message.strip()
        if not message:
            return

        self.lastIndex += 1
        message += f"/{self.lastIndex}"

        hashedMessage = ""
        try:
            hashedMessage = self.hash.hashMessage(message)
        except Exception as e:
            print(f"Error hashing message: {e}")
            self.lastIndex -= 1
            return

        if not hashedMessage:
            self.lastIndex -= 1
            return

        self.com.sendBroadcast(hashedMessage)
