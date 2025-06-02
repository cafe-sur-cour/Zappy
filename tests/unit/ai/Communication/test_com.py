##
## EPITECH PROJECT, 2025
## zappy
## File description:
## test_com
##

import pytest
import sys
import os
from unittest.mock import Mock, patch, MagicMock

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../../ai/')))

from src.Communication.Communication import Communication
from src.Communication.Socket import Socket
from src.Exceptions.Exceptions import (
    CommunicationHandshakeException,
    CommunicationInvalidResponseException
)


class TestCommunication:
    def test_communication_init(self):
        """Test communication initialization"""
        comm = Communication("team1", "localhost", 4242)

        assert comm._name == "team1"
        assert comm._host == "localhost"
        assert comm._port == 4242
        assert isinstance(comm._socket, Socket)

    @patch('src.Communication.Communication.Socket')
    def test_connect_to_server_success(self, mock_socket_class):
        """Test successful server connection and handshake"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.side_effect = [
            "WELCOME\n",
            "3\n",
            "10 15\n"
        ]

        comm = Communication("team1", "localhost", 4242)
        slots, x, y = comm.connectToServer()

        mock_socket.connect.assert_called_once()
        mock_socket.send.assert_called_once_with("team1\n")
        assert slots == 3
        assert x == 10
        assert y == 15

    @patch('src.Communication.Communication.Socket')
    def test_connect_to_server_invalid_welcome(self, mock_socket_class):
        """Test server connection with invalid welcome message"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "Invalid\n"

        comm = Communication("team1", "localhost", 4242)

        with pytest.raises(CommunicationInvalidResponseException) as excinfo:
            comm.connectToServer()

        assert "Invalid response from server handshake: Invalid" in str(excinfo.value)

    @patch('src.Communication.Communication.Socket')
    def test_connect_to_server_welcome_no_newline(self, mock_socket_class):
        """Test server connection with welcome message without newline"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "WELCOME"

        comm = Communication("team1", "localhost", 4242)

        with pytest.raises(CommunicationInvalidResponseException) as excinfo:
            comm.connectToServer()

        assert "Response from server handshake is not terminated with a newline" in str(excinfo.value)

    @patch('src.Communication.Communication.Socket')
    def test_connect_to_server_invalid_slots(self, mock_socket_class):
        """Test server connection with invalid slots number"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.side_effect = [
            "WELCOME\n",
            "invalid\n",
            "10 15\n"
        ]

        comm = Communication("team1", "localhost", 4242)

        with pytest.raises(CommunicationInvalidResponseException) as excinfo:
            comm.connectToServer()

        assert "Invalid number of slots: invalid" in str(excinfo.value)

    @patch('src.Communication.Communication.Socket')
    def test_connect_to_server_invalid_coordinates(self, mock_socket_class):
        """Test server connection with invalid coordinates"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.side_effect = [
            "WELCOME\n",
            "3\n",
            "invalid coordinates\n"
        ]

        comm = Communication("team1", "localhost", 4242)

        with pytest.raises(CommunicationInvalidResponseException) as excinfo:
            comm.connectToServer()

        assert "Invalid coordinates from server: invalid coordinates" in str(excinfo.value)

    @patch('src.Communication.Communication.Socket')
    def test_connect_to_server_invalid_handshake_values(self, mock_socket_class):
        """Test server connection with invalid handshake values (zero or negative)"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.side_effect = [
            "WELCOME\n",
            "0\n",
            "10 15\n"
        ]

        comm = Communication("team1", "localhost", 4242)

        with pytest.raises(CommunicationHandshakeException) as excinfo:
            comm.connectToServer()

        assert "Invalid handshake values: slots=0, x=10, y=15" in str(excinfo.value)

    @patch('src.Communication.Communication.Socket')
    def test_send_forward_success(self, mock_socket_class):
        """Test successful forward command"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "ok\n"

        comm = Communication("team1", "localhost", 4242)
        comm.sendForward()

        mock_socket.send.assert_called_once_with("Forward\n")

    @patch('src.Communication.Communication.Socket')
    def test_send_forward_invalid_response(self, mock_socket_class):
        """Test forward command with invalid response"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "ko\n"

        comm = Communication("team1", "localhost", 4242)

        with pytest.raises(CommunicationInvalidResponseException) as excinfo:
            comm.sendForward()

        assert "Invalid response from Forward: ko" in str(excinfo.value)

    @patch('src.Communication.Communication.Socket')
    def test_send_right_success(self, mock_socket_class):
        """Test successful right command"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "ok\n"

        comm = Communication("team1", "localhost", 4242)
        comm.sendRight()

        mock_socket.send.assert_called_once_with("Right\n")

    @patch('src.Communication.Communication.Socket')
    def test_send_left_success(self, mock_socket_class):
        """Test successful left command"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "ok\n"

        comm = Communication("team1", "localhost", 4242)
        comm.sendLeft()

        mock_socket.send.assert_called_once_with("Left\n")

    @patch('src.Communication.Communication.Socket')
    def test_get_look_success(self, mock_socket_class):
        """Test successful look command"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "[player, food linemate, empty]\n"

        comm = Communication("team1", "localhost", 4242)
        result = comm.getLook()

        mock_socket.send.assert_called_once_with("Look\n")
        assert result == ["player", "food linemate", "empty"]

    @patch('src.Communication.Communication.Socket')
    def test_get_look_invalid_format(self, mock_socket_class):
        """Test look command with invalid response format"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "invalid format\n"

        comm = Communication("team1", "localhost", 4242)

        with pytest.raises(CommunicationInvalidResponseException) as excinfo:
            comm.getLook()

        assert "Invalid response from Look: invalid format" in str(excinfo.value)

    @patch('src.Communication.Communication.Socket')
    def test_get_inventory_success(self, mock_socket_class):
        """Test successful inventory command"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "[food 3, linemate 1, deraumere 2]\n"

        comm = Communication("team1", "localhost", 4242)
        result = comm.getInventory()

        mock_socket.send.assert_called_once_with("Inventory\n")
        assert result == [("food", 3), ("linemate", 1), ("deraumere", 2)]

    @patch('src.Communication.Communication.Socket')
    def test_get_inventory_empty(self, mock_socket_class):
        """Test inventory command with empty inventory"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "[]\n"

        comm = Communication("team1", "localhost", 4242)
        result = comm.getInventory()

        assert result == []

    @patch('src.Communication.Communication.Socket')
    def test_get_inventory_invalid_item_format(self, mock_socket_class):
        """Test inventory command with invalid item format"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "[invalid_item_format]\n"

        comm = Communication("team1", "localhost", 4242)

        with pytest.raises(CommunicationInvalidResponseException) as excinfo:
            comm.getInventory()

        assert "Invalid item format in Inventory: invalid_item_format" in str(excinfo.value)

    @patch('src.Communication.Communication.Socket')
    def test_send_broadcast_success(self, mock_socket_class):
        """Test successful broadcast command"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "ok\n"

        comm = Communication("team1", "localhost", 4242)
        comm.sendBroadcast("Hello world")

        mock_socket.send.assert_called_once_with("Broadcast Hello world\n")

    @patch('src.Communication.Communication.Socket')
    def test_get_connect_nbr_success(self, mock_socket_class):
        """Test successful connect_nbr command"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "5\n"

        comm = Communication("team1", "localhost", 4242)
        result = comm.getCetConnectNbr()

        mock_socket.send.assert_called_once_with("Connect_nbr\n")
        assert result == 5

    @patch('src.Communication.Communication.Socket')
    def test_get_connect_nbr_invalid_number(self, mock_socket_class):
        """Test connect_nbr command with invalid number"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "invalid\n"

        comm = Communication("team1", "localhost", 4242)

        with pytest.raises(CommunicationInvalidResponseException) as excinfo:
            comm.getCetConnectNbr()

        assert "Invalid number: Got invalid from command Connect_nbr" in str(excinfo.value)

    @patch('src.Communication.Communication.Socket')
    def test_send_fork_success(self, mock_socket_class):
        """Test successful fork command"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "ok\n"

        comm = Communication("team1", "localhost", 4242)
        comm.sendFork()

        mock_socket.send.assert_called_once_with("Fork\n")

    @patch('src.Communication.Communication.Socket')
    def test_send_eject_success(self, mock_socket_class):
        """Test successful eject command"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "ok\n"

        comm = Communication("team1", "localhost", 4242)
        comm.sendEject()

        mock_socket.send.assert_called_once_with("Eject\n")

    @patch('src.Communication.Communication.Socket')
    def test_send_take_object_success(self, mock_socket_class):
        """Test successful take object command"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "ok\n"

        comm = Communication("team1", "localhost", 4242)
        comm.sendTakeObject("food")

        mock_socket.send.assert_called_once_with("Take food\n")

    @patch('src.Communication.Communication.Socket')
    def test_send_set_object_success(self, mock_socket_class):
        """Test successful set object command"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "ok\n"

        comm = Communication("team1", "localhost", 4242)
        comm.sendSetObject()

        mock_socket.send.assert_called_once_with("Set object\n")

    @patch('src.Communication.Communication.Socket')
    def test_send_incantation_elevation_underway(self, mock_socket_class):
        """Test incantation command with elevation underway response"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "Elevation underway\n"

        comm = Communication("team1", "localhost", 4242)
        message, level = comm.sendIncantation()

        mock_socket.send.assert_called_once_with("Incantation\n")
        assert message == "Elevation underway"
        assert level == 0

    @patch('src.Communication.Communication.Socket')
    def test_send_incantation_current_level(self, mock_socket_class):
        """Test incantation command with current level response"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "Current level: 3\n"

        comm = Communication("team1", "localhost", 4242)
        message, level = comm.sendIncantation()

        assert message == "Current level: 3"
        assert level == 3

    @patch('src.Communication.Communication.Socket')
    def test_send_incantation_ko_response(self, mock_socket_class):
        """Test incantation command with ko response"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "ko\n"

        comm = Communication("team1", "localhost", 4242)

        with pytest.raises(CommunicationInvalidResponseException) as excinfo:
            comm.sendIncantation()

        assert "Invalid response from Incantation: ko" in str(excinfo.value)

    @patch('src.Communication.Communication.Socket')
    def test_send_incantation_invalid_level_format(self, mock_socket_class):
        """Test incantation command with invalid level format"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "Current level: invalid\n"

        comm = Communication("team1", "localhost", 4242)

        with pytest.raises(CommunicationInvalidResponseException) as excinfo:
            comm.sendIncantation()

        assert "Invalid level in Incantation response: Current level: invalid" in str(excinfo.value)

    @patch('src.Communication.Communication.Socket')
    def test_send_incantation_unexpected_response(self, mock_socket_class):
        """Test incantation command with unexpected response"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket
        mock_socket.receive.return_value = "Unexpected response\n"

        comm = Communication("team1", "localhost", 4242)

        with pytest.raises(CommunicationInvalidResponseException) as excinfo:
            comm.sendIncantation()

        assert "Unexpected response from Incantation: Unexpected response" in str(excinfo.value)

    @patch('src.Communication.Communication.Socket')
    def test_response_without_newline_errors(self, mock_socket_class):
        """Test that responses without newlines raise appropriate errors"""
        mock_socket = Mock()
        mock_socket_class.return_value = mock_socket

        commands_to_test = [
            ("sendForward", lambda comm: comm.sendForward()),
            ("sendRight", lambda comm: comm.sendRight()),
            ("sendLeft", lambda comm: comm.sendLeft()),
            ("getLook", lambda comm: comm.getLook()),
            ("getInventory", lambda comm: comm.getInventory()),
            ("sendBroadcast", lambda comm: comm.sendBroadcast("test")),
            ("getCetConnectNbr", lambda comm: comm.getCetConnectNbr()),
            ("sendFork", lambda comm: comm.sendFork()),
            ("sendEject", lambda comm: comm.sendEject()),
            ("sendTakeObject", lambda comm: comm.sendTakeObject("food")),
            ("sendSetObject", lambda comm: comm.sendSetObject()),
            ("sendIncantation", lambda comm: comm.sendIncantation())
        ]

        for command_name, command_func in commands_to_test:
            mock_socket.receive.return_value = "response_without_newline"
            comm = Communication("team1", "localhost", 4242)

            with pytest.raises(CommunicationInvalidResponseException) as excinfo:
                command_func(comm)

            assert "is not terminated with a newline" in str(excinfo.value)
