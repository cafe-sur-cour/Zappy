from time import sleep
from src.Hash.Hash import Hash

class Player:
    def __init__(self, name):
        self.teamName : str = name
        self.level : int = 1
        self.hash : Hash = Hash(name)
        self.inventory : dict[str, int] = {"food": 10, "linemate": 0, "deraumere": 0, "sibur": 0,
                          "mendiane": 0, "phiras": 0, "thystame": 0}
        self.alive : bool = True
        self.in_incantation : bool = False

    def __str__(self):
        return f"Player team: {self.teamName} - Level: {self.level}, Inventory: {self.inventory}, Alive: {self.alive}, In Incantation: {self.in_incantation}"

    def beginIncantation(self) -> None:
        print(f"Player team: {self.teamName} has started an incentive!")
        self.in_incantation = True
        sleep(10)
        print(f"Player team: {self.teamName} has ended an incentive!")
        self.in_incantation = False
        self.level = self.level + 1

    def layAEgg(self) -> None:
        print(f"Player team: {self.teamName} has laid an egg!")

    def loop(self) -> None:
        while self.alive:
            self.hash.hashMessage("Hello, World!")
            print(self)
            sleep(1)
