##
## EPITECH PROJECT, 2025
## zappy
## File description:
## App
##

import os
import sys
import signal
import time
from src.Config.Constants import (
    FAILURE,
    SUCCESS,
)
from src.Player.Player import Player
from src.Exceptions.Exceptions import (
    CommunicationException,
    SocketException
)
from src.Logger.Logger import Logger


class App:
    def __init__(self, config: dict[str]):
        self.port = config["port"]
        self.name = config["name"]
        self.ip = config["machine"]
        self.childs: list[int] = []
        self.running = True
        self.is_main_process = True
        self.logger = Logger()

        if self.is_main_process:
            signal.signal(signal.SIGINT, self._signal_handler)
            signal.signal(signal.SIGTERM, self._signal_handler)

    def _signal_handler(self, signum, frame):
        if self.is_main_process:
            self.logger.info(f"Shutting down AI team {self.name}...")
            self.running = False

    def _wait_for_children(self):
        if not self.is_main_process:
            return

        if len(self.childs) > 0:
            self.logger.info(f"Waiting for {len(self.childs)} AI child processes to finish...")

        for pid in self.childs:
            try:
                os.waitpid(pid, 0)
            except (ProcessLookupError, ChildProcessError):
                pass
            except Exception:
                pass

        num_children = len(self.childs)
        self.childs.clear()
        if num_children > 0:
            self.logger.success(f"All AI processes finished.")

    def _cleanup_children(self):
        if not self.is_main_process:
            return

        if len(self.childs) > 0:
            self.logger.info(f"Terminating {len(self.childs)} AI child processes...")

        for pid in self.childs:
            try:
                os.kill(pid, signal.SIGTERM)
            except (ProcessLookupError, ChildProcessError):
                pass
            except Exception:
                pass

        time.sleep(0.5)

        force_killed = []
        for pid in self.childs:
            try:
                os.kill(pid, 0)
                os.kill(pid, signal.SIGKILL)
                force_killed.append(pid)
            except (ProcessLookupError, ChildProcessError):
                pass
            except Exception:
                pass

        for pid in self.childs:
            try:
                os.waitpid(pid, 0)
            except (ProcessLookupError, ChildProcessError):
                pass
            except Exception:
                pass

        num_children = len(self.childs)
        self.childs.clear()
        if force_killed:
            self.logger.info(f"Force killed {len(force_killed)} AI child processes")
        if num_children > 0:
            self.logger.success(f"All AI processes terminated.")

    def create_new_player(self) -> int:
        pid: int = os.fork()
        if pid < 0:
            return -1
        if pid == 0:
            self.is_main_process = False
            signal.signal(signal.SIGINT, self._child_signal_handler)
            signal.signal(signal.SIGTERM, self._child_signal_handler)

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

    def _child_signal_handler(self, signum, frame):
        sys.stdout.flush()
        sys.stderr.flush()
        os._exit(SUCCESS)

    def run(self):
        self.logger.success(f"Starting Zappy AI for team: {self.name}...")
        player = Player(self.name, self.ip, self.port)
        slots, x, y = 0, 0, 0

        try:
            slots, x, y = player.communication.connectToServer()
        except (CommunicationException, SocketException) as e:
            self.logger.error(f"Failed to connect to server: {e}")
            return FAILURE

        player.setMapSize(x, y)
        player.setNbSlots(slots + 1)

        for _ in range(slots):
            if not self.running:
                break
            child_pid = self.create_new_player()
            if child_pid > 0:
                self.childs.append(child_pid)

        try:
            player.startComThread()
            while self.running:
                try:
                    player.loop()
                    break
                except KeyboardInterrupt:
                    break
        except (CommunicationException, SocketException):
            self.logger.error(f"Server connection lost for team {self.name}")
            self._cleanup_children()
            return FAILURE
        except KeyboardInterrupt:
            self.logger.info(f"Interrupted - shutting down team {self.name}")
            self._cleanup_children()
            return SUCCESS
        except Exception as e:
            self.logger.error(f"Unexpected error in main player: {e}")
            self._cleanup_children()
            return FAILURE
        finally:
            if self.is_main_process:
                self._wait_for_children()
                self.logger.info(f"AI team {self.name} finished")

        return SUCCESS
