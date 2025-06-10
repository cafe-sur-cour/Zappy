##
## EPITECH PROJECT, 2025
## zappy
## File description:
## App
##

import os
import signal
from src.Utils.Utils import (
    FAILURE,
    SUCCESS,
    Colors
)
from src.Player.Player import Player
from src.Exceptions.Exceptions import (
    CommunicationException,
    SocketException
)


class App:
    def __init__(self, config: dict[str]):
        self.port = config["port"]
        self.name = config["name"]
        self.ip = config["machine"]
        self.childs: list[int] = []
        self.running = True
        self.is_main_process = True

        if self.is_main_process:
            signal.signal(signal.SIGINT, self._signal_handler)
            signal.signal(signal.SIGTERM, self._signal_handler)

    def _signal_handler(self, signum, frame):
        if self.is_main_process:
            print(f"\n{Colors.CYAN}Shutting down AI team {self.name}...{Colors.RESET}")
            self.running = False
            self._cleanup_children()

    def _cleanup_children(self):
        if not self.is_main_process:
            return

        for pid in self.childs:
            try:
                os.kill(pid, signal.SIGTERM)
                os.waitpid(pid, os.WNOHANG)
            except (ProcessLookupError, ChildProcessError):
                pass
            except Exception:
                pass

    def __del__(self):
        if self.is_main_process:
            self._cleanup_children()

    def create_new_player(self) -> int:
        pid: int = os.fork()
        if pid < 0:
            return -1
        if pid == 0:
            self.is_main_process = False
            signal.signal(signal.SIGINT, signal.SIG_DFL)
            signal.signal(signal.SIGTERM, signal.SIG_DFL)

            try:
                p = Player(self.name, self.ip, self.port)
                p.is_child_process = True
                _, x, y = p.communication.connectToServer()
                p.setMapSize(x, y)
                p.startComThread()
                p.loop()
            except (CommunicationException, SocketException):
                exit(FAILURE)
            except KeyboardInterrupt:
                exit(SUCCESS)
            except Exception:
                exit(FAILURE)
            exit(SUCCESS)
        return pid

    def run(self):
        print(f"{Colors.GREEN}Starting Zappy AI for team: {self.name}...{Colors.RESET}")
        player = Player(self.name, self.ip, self.port)
        slots, x, y = 0, 0, 0

        try:
            slots, x, y = player.communication.connectToServer()
        except (CommunicationException, SocketException) as e:
            print(f"{Colors.RED}Failed to connect to server: {e}{Colors.RESET}")
            return FAILURE

        player.setMapSize(x, y)

        for _ in range(slots):
            if not self.running:
                break
            child_pid = self.create_new_player()
            if child_pid > 0:
                self.childs.append(child_pid)

        try:
            player.startComThread()
            player.loop()
        except (CommunicationException, SocketException):
            print(f"{Colors.RED}Server connection lost for team {self.name}{Colors.RESET}")
            return FAILURE
        except KeyboardInterrupt:
            print(f"{Colors.YELLOW}Interrupted - shutting down team {self.name}{Colors.RESET}")
            return SUCCESS
        except Exception as e:
            print(f"{Colors.RED}Unexpected error in main player: {e}{Colors.RESET}")
            return FAILURE
        finally:
            if self.is_main_process:
                print(f"{Colors.YELLOW}AI team {self.name} finished{Colors.RESET}")
                self._cleanup_children()

        return SUCCESS
