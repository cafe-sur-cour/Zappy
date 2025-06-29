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
        self.hasher = Hash(team)
        self.messageIndex: int = 0
        self.playerId: str = ""
        self.messages: list[str] = []

    def revealMessage(self, message: str) -> str:
        try:
            message = self.hasher.unHashMessage(message.strip())
        except Exception:
            message = ""

        if not message:
            return ""

        return message.strip()

    def broadcastMessage(self, message: str, format: bool = True) -> None:
        if not message:
            return

        message = message.strip()
        if not message:
            return

        if format:
            message = self._formatMessage(message)

        hashedMessage = ""
        try:
            hashedMessage = self.hasher.hashMessage(message)
        except Exception as e:
            print(f"Error hashing message: {e}")
            return

        if not hashedMessage:
            return

        self.messages.append(hashedMessage)
        self.com.sendBroadcast(hashedMessage)

    def setPlayerId(self, playerId: str) -> None:
        self.playerId = playerId

    def _formatMessage(self, message: str) -> str:
        self.messageIndex += 1
        return f"{message} {self.playerId} {self.messageIndex}"

    def parseReceivedMessage(self, message: str) -> tuple[str, str, int]:
        parts = message.strip().split()
        if len(parts) >= 3:
            try:
                message_index = int(parts[-1])
                sender_id = parts[-2]
                original_message = " ".join(parts[:-2])
                return original_message, sender_id, message_index
            except (ValueError, IndexError):
                return message.strip(), "", 0
        else:
            return message.strip(), "", 0

    def alreadyReceivedMessage(self, message: str) -> bool:
        if message not in self.messages:
            self.messages.append(message)
            return False
        return True
