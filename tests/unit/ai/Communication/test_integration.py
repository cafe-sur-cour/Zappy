##
## EPITECH PROJECT, 2025
## zappy
## File description:
## test_integration
##

import pytest
import sys
import os
import subprocess
import time
import signal
import threading
from unittest.mock import patch

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../../ai/')))

from src.Communication.Socket import Socket
from src.Communication.Communication import Communication
from src.Exceptions.Exceptions import (
    SocketException,
    CommunicationHandshakeException,
    CommunicationInvalidResponseException
)


class TestIntegration:
    """Integration tests with the actual zappy server"""

    @pytest.fixture
    def server_process(self):
        """Start the zappy server for integration testing"""
        server_binary = os.path.join(
            os.path.dirname(__file__),
            "../../../../zappy_ref-v3.0.0/zappy_server"
        )

        if not os.path.exists(server_binary):
            pytest.skip(f"Server binary not found at {server_binary}")

        process = subprocess.Popen([
            server_binary,
            "-p", "4242",
            "-x", "10",
            "-y", "10",
            "-n", "team1", "team2",
            "-c", "3",
            "-f", "1000"
        ], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        time.sleep(0.5)

        if process.poll() is not None:
            stdout, stderr = process.communicate()
            pytest.skip(f"Server failed to start: {stderr.decode()}")

        yield process

        try:
            process.terminate()
            process.wait(timeout=5)
        except subprocess.TimeoutExpired:
            process.kill()
            process.wait()

    def test_socket_real_connection(self, server_process):
        """Test Socket class with real server connection"""
        sock = Socket("localhost", 4242)

        try:
            sock.connect()

            response = sock.receive()
            assert response == "WELCOME\n"

            sock.send("team1\n")

            response = sock.receive()
            assert response.strip().isdigit()
            slots = int(response.strip())
            assert slots > 0

            response = sock.receive()
            dimensions = response.strip().split()
            assert len(dimensions) == 2
            x, y = map(int, dimensions)
            assert x == 10 and y == 10

        finally:
            sock.close()

    def test_communication_real_handshake(self, server_process):
        """Test Communication class handshake with real server"""
        comm = Communication("team1", "localhost", 4242)

        slots, x, y = comm.connectToServer()

        assert isinstance(slots, int)
        assert slots > 0
        assert x == 10
        assert y == 10

    def test_communication_real_commands(self, server_process):
        """Test Communication class commands with real server"""
        comm = Communication("team1", "localhost", 4242)
        comm.connectToServer()

        comm.sendForward()
        comm.sendRight()
        comm.sendLeft()

        look_result = comm.getLook()
        assert isinstance(look_result, list)

        inventory_result = comm.getInventory()
        assert isinstance(inventory_result, list)

        connect_nbr = comm.getCetConnectNbr()
        assert isinstance(connect_nbr, int)
        assert connect_nbr >= 0

    def test_communication_real_object_manipulation(self, server_process):
        """Test object manipulation commands with real server"""
        comm = Communication("team1", "localhost", 4242)
        comm.connectToServer()

        try:
            comm.sendTakeObject("food")
        except CommunicationInvalidResponseException as e:
            assert "Invalid response from Take" in str(e)

        try:
            comm.sendSetObject("food")
        except CommunicationInvalidResponseException as e:
            assert "Invalid response from Set" in str(e)

    def test_communication_real_incantation(self, server_process):
        """Test incantation command with real server"""
        comm = Communication("team1", "localhost", 4242)
        comm.connectToServer()

        try:
            message, level = comm.sendIncantation()
            assert message in ["Elevation underway"] or message.startswith("Current level:")
            assert isinstance(level, int)
            assert level >= 0
        except CommunicationInvalidResponseException as e:
            assert "Invalid response from Incantation" in str(e)

    def test_multiple_clients(self, server_process):
        """Test multiple clients connecting to the same server"""
        comm1 = Communication("team1", "localhost", 4242)
        comm2 = Communication("team1", "localhost", 4242)

        slots1, x1, y1 = comm1.connectToServer()
        slots2, x2, y2 = comm2.connectToServer()

        assert x1 == x2 == 10
        assert y1 == y2 == 10

        assert slots1 > 0
        assert slots2 > 0

    def test_socket_connection_refused(self):
        """Test Socket behavior when server is not running"""
        sock = Socket("localhost", 9999)

        with pytest.raises(ConnectionRefusedError):
            sock.connect()

    def test_communication_connection_refused(self):
        """Test Communication behavior when server is not running"""
        comm = Communication("team1", "localhost", 9999)

        with pytest.raises(ConnectionRefusedError):
            comm.connectToServer()

    def test_socket_buffer_with_real_server(self, server_process):
        """Test Socket buffer handling with real server responses"""
        sock = Socket("localhost", 4242)
        sock.connect()

        response1 = sock.receive()
        assert response1 == "WELCOME\n"
        assert sock._buffer == ""

        sock.send("team1\n")

        response2 = sock.receive()
        assert response2.endswith("\n")

        response3 = sock.receive()
        assert response3.endswith("\n")

        sock.close()

    def test_invalid_team_name(self, server_process):
        """Test behavior with invalid team name"""
        sock = Socket("localhost", 4242)
        sock.connect()

        response = sock.receive()
        assert response == "WELCOME\n"

        sock.send("invalid_team\n")

        try:
            response = sock.receive()
        except SocketException:
            pass

        sock.close()
