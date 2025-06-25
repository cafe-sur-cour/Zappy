#
# EPITECH PROJECT, 2025
# zappy
# File description:
# test_player.py

import pytest
import sys
import os
from unittest.mock import patch, MagicMock, Mock, call
from time import sleep
from threading import Thread

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '../../../../ai'))

from src.Player.Player import Player
from src.Communication.Communication import Communication  
from src.Broadcaster.Broadcaster import Broadcaster
from src.Logger.Logger import Logger
from src.Exceptions.Exceptions import (
    CommunicationException,
    SocketException
)
from src.Config.Constants import SUCCESS, FAILURE
from src.Config.GameConfig import LVL_UPGRADES, TOTAL_NEEDED_STONES


class TestPlayer:
    """Test suite for Player class"""

    @pytest.fixture
    def mock_communication(self):
        """Create a mock Communication object"""
        mock_comm = Mock(spec=Communication)
        mock_comm.playerIsDead.return_value = False
        mock_comm.hasMessages.return_value = False
        mock_comm.hasResponses.return_value = False
        mock_comm.hasRequests.return_value = False
        mock_comm.hasPendingCommands.return_value = False
        mock_comm.getInventory.return_value = {
            "food": 10, "linemate": 0, "deraumere": 0, "sibur": 0,
            "mendiane": 0, "phiras": 0, "thystame": 0
        }
        mock_comm.getLook.return_value = []
        return mock_comm

    @pytest.fixture
    def mock_broadcaster(self):
        """Create a mock Broadcaster object"""
        mock_bc = Mock(spec=Broadcaster)
        mock_bc.revealMessage.return_value = ""
        return mock_bc

    @pytest.fixture
    def mock_logger(self):
        """Create a mock Logger object"""
        return Mock(spec=Logger)

    @pytest.fixture
    def player_instance(self, mock_communication, mock_broadcaster, mock_logger):
        """Create a Player instance with mocked dependencies"""
        with patch('src.Player.Player.Communication', return_value=mock_communication), \
             patch('src.Player.Player.Broadcaster', return_value=mock_broadcaster), \
             patch('src.Player.Player.Logger', return_value=mock_logger), \
             patch('src.Player.Player.Thread') as mock_thread, \
             patch('os.getpid', return_value=12345):
            
            player = Player("TestTeam", "127.0.0.1", 4242)
            player.communication = mock_communication
            player.broadcaster = mock_broadcaster
            player.logger = mock_logger
            return player

    def test_player_initialization(self, player_instance):
        """Test Player initialization with correct default values"""
        assert player_instance.teamName == "TestTeam"
        assert player_instance.ip == "127.0.0.1"
        assert player_instance.port == 4242
        assert player_instance.level == 1
        assert player_instance.x == 0
        assert player_instance.y == 0
        assert player_instance.nbTeamSlots == -1
        assert player_instance.nbConnectedPlayers == 1
        assert player_instance.inIncantation == False
        assert player_instance.id == "TestTeam-12345"
        
        # Test initial inventory
        expected_inventory = {
            "food": 10, "linemate": 0, "deraumere": 0, "sibur": 0,
            "mendiane": 0, "phiras": 0, "thystame": 0
        }
        assert player_instance.inventory == expected_inventory

    def test_str_representation(self, player_instance):
        """Test string representation of Player"""
        str_repr = str(player_instance)
        assert "TestTeam" in str_repr
        assert "Level: 1" in str_repr
        assert "Alive: True" in str_repr

    def test_set_map_size(self, player_instance):
        """Test setMapSize method"""
        player_instance.setMapSize(20, 15)
        assert player_instance.x == 20
        assert player_instance.y == 15
        assert player_instance.roombaState["targetForward"] == 20  # max(20, 15)

    def test_set_nb_slots(self, player_instance):
        """Test setNbSlots method"""
        player_instance.setNbSlots(5)
        assert player_instance.nbTeamSlots == 5

    def test_get_needed_stones_by_priority_level_1(self, player_instance):
        """Test getNeededStonesByPriority for level 1"""
        player_instance.level = 1
        player_instance.inventory = {
            "food": 10, "linemate": 0, "deraumere": 0, "sibur": 0,
            "mendiane": 0, "phiras": 0, "thystame": 0
        }
        
        needed_stones = player_instance.getNeededStonesByPriority()
        assert len(needed_stones) == 1
        assert ("linemate", 1) in needed_stones

    def test_get_needed_stones_by_priority_max_level(self, player_instance):
        """Test getNeededStonesByPriority for max level (8)"""
        player_instance.level = 8
        needed_stones = player_instance.getNeededStonesByPriority()
        assert len(needed_stones) == 0

    def test_get_needed_stones_by_priority_with_inventory(self, player_instance):
        """Test getNeededStonesByPriority with partial inventory"""
        player_instance.level = 2
        player_instance.inventory = {
            "food": 10, "linemate": 1, "deraumere": 0, "sibur": 0,
            "mendiane": 0, "phiras": 0, "thystame": 0
        }
        
        needed_stones = player_instance.getNeededStonesByPriority()
        # Should need deraumere: 1, sibur: 1 (linemate already satisfied)
        expected = [("deraumere", 1), ("sibur", 1)]
        assert sorted(needed_stones) == sorted(expected)

    def test_does_team_have_enough_stones_false(self, player_instance):
        """Test doesTeamHaveEnoughStones when not enough stones"""
        result = player_instance.doesTeamHaveEnoughStones()
        assert result == False
        assert player_instance.teamHasEnoughStones == False

    def test_does_team_have_enough_stones_true(self, player_instance):
        """Test doesTeamHaveEnoughStones when enough stones"""
        # Set up inventory with enough stones
        player_instance.inventory = {
            "food": 10, "linemate": 10, "deraumere": 10, "sibur": 10,
            "mendiane": 10, "phiras": 10, "thystame": 10
        }
        
        result = player_instance.doesTeamHaveEnoughStones()
        assert result == True
        assert player_instance.teamHasEnoughStones == True

    def test_enough_food_for_incantation_true(self, player_instance):
        """Test enoughFoodForIncantation with sufficient food"""
        player_instance.x = 10
        player_instance.y = 10
        result = player_instance.enoughFoodForIncantation(100)
        assert result == True

    def test_enough_food_for_incantation_false(self, player_instance):
        """Test enoughFoodForIncantation with insufficient food"""
        player_instance.x = 100
        player_instance.y = 100
        result = player_instance.enoughFoodForIncantation(1)
        assert result == False

    def test_team_has_enough_food_for_incantation_true(self, player_instance):
        """Test teamHasEnoughFoodForIncantation with sufficient food"""
        player_instance.inventory["food"] = 100
        player_instance.x = 10
        player_instance.y = 10
        
        result = player_instance.teamHasEnoughFoodForIncantation()
        assert result == True

    def test_team_has_enough_food_for_incantation_false(self, player_instance):
        """Test teamHasEnoughFoodForIncantation with insufficient food"""
        player_instance.inventory["food"] = 1
        player_instance.x = 100
        player_instance.y = 100
        
        result = player_instance.teamHasEnoughFoodForIncantation()
        assert result == False

    def test_roomba_action_look_around(self, player_instance):
        """Test roombaAction in lookAround phase"""
        player_instance.roombaState["phase"] = "lookAround"
        player_instance.roombaAction()
        
        player_instance.communication.sendLook.assert_called_once()
        assert player_instance.roombaState["lastCommand"] == "look"
        assert player_instance.roombaState["phase"] == "vacuum"
        assert player_instance.roombaState["lastPhase"] == "lookAround"

    def test_roomba_action_vacuum(self, player_instance):
        """Test roombaAction in vacuum phase"""
        player_instance.roombaState["phase"] = "vacuum"
        player_instance.look = [{"food": 2, "linemate": 1}]
        
        player_instance.roombaAction()
        
        assert player_instance.roombaState["lastCommand"] == "take"
        assert player_instance.roombaState["phase"] == "forward"
        assert player_instance.roombaState["lastPhase"] == "vacuum"

    def test_roomba_action_update_inventory(self, player_instance):
        """Test roombaAction in updateInventory phase"""
        player_instance.roombaState["phase"] = "updateInventory"
        player_instance.roombaAction()
        
        player_instance.communication.sendInventory.assert_called_once()
        assert player_instance.roombaState["lastCommand"] == "inventory"
        assert player_instance.roombaState["phase"] == "forward"

    def test_roomba_action_forward(self, player_instance):
        """Test roombaAction in forward phase"""
        player_instance.roombaState["phase"] = "forward"
        player_instance.roombaState["forwardCount"] = 5
        player_instance.roombaState["targetForward"] = 10
        
        player_instance.roombaAction()
        
        player_instance.communication.sendForward.assert_called_once()
        assert player_instance.roombaState["forwardCount"] == 6
        assert player_instance.roombaState["phase"] == "lookAround"

    def test_roomba_action_forward_max_reached(self, player_instance):
        """Test roombaAction when forward count reaches target"""
        player_instance.roombaState["phase"] = "forward"
        player_instance.roombaState["forwardCount"] = 10
        player_instance.roombaState["targetForward"] = 10
        
        player_instance.roombaAction()
        
        player_instance.communication.sendRight.assert_called_once()
        assert player_instance.roombaState["forwardCount"] == 0
        assert player_instance.roombaState["phase"] == "turn"

    def test_handle_response_inventory(self, player_instance):
        """Test handleResponseInventory method"""
        new_inventory = {
            "food": 15, "linemate": 2, "deraumere": 1, "sibur": 0,
            "mendiane": 0, "phiras": 0, "thystame": 0
        }
        player_instance.communication.getInventory.return_value = new_inventory
        
        player_instance.handleResponseInventory()
        
        assert player_instance.inventory == new_inventory

    def test_handle_response_look(self, player_instance):
        """Test handleResponseLook method"""
        new_look = [{"food": 1}, {"linemate": 2}]
        player_instance.communication.getLook.return_value = new_look
        
        player_instance.handleResponseLook()
        
        assert player_instance.look == new_look

    def test_handle_response_ko(self, player_instance):
        """Test handleResponseKO method"""
        player_instance.roombaState["lastCommand"] = "take"
        player_instance.handleResponseKO()
        
        player_instance.logger.error.assert_called_with("Command 'take' failed")

    def test_handle_response_ok_vacuum(self, player_instance):
        """Test handleResponseOK method in vacuum context"""
        player_instance.incantationState["status"] = False
        player_instance.goToIncantationState["status"] = False
        player_instance.roombaState["lastPhase"] = "vacuum"
        
        player_instance.handleResponseOK()
        
        assert player_instance.roombaState["phase"] == "updateInventory"

    def test_handle_response_elevation_underway(self, player_instance):
        """Test handleResponseElevationUnderway method"""
        player_instance.handleResponseElevationUnderway()
        
        assert player_instance.inIncantation == True
        player_instance.logger.display.assert_called_with("Elevation underway, waiting for result...")

    def test_handle_response_current_level_success(self, player_instance):
        """Test handleResponseCurrentLevel with successful level up"""
        player_instance.level = 2
        player_instance.handleResponseCurrentLevel("3")
        
        assert player_instance.level == 3
        assert player_instance.inIncantation == False
        player_instance.logger.success.assert_called_with("Player level increased to 3")

    def test_handle_response_current_level_max_level(self, player_instance):
        """Test handleResponseCurrentLevel reaching max level"""
        player_instance.level = 7
        player_instance.handleResponseCurrentLevel("8")
        
        assert player_instance.level == 8
        player_instance.logger.success.assert_called_with("Player reached maximum level!")

    def test_handle_response_current_level_invalid(self, player_instance):
        """Test handleResponseCurrentLevel with invalid level"""
        player_instance.level = 2
        player_instance.handleResponseCurrentLevel("invalid")
        
        player_instance.logger.error.assert_called_with("Invalid level response: invalid")

    def test_handle_response_connect_nbr(self, player_instance):
        """Test handleResponseConnectNbr method"""
        player_instance.nbTeamSlots = 10
        player_instance.handleResponseConnectNbr("7")
        
        assert player_instance.nbConnectedPlayers == 3  # 10 - 7

    def test_handle_response_connect_nbr_invalid(self, player_instance):
        """Test handleResponseConnectNbr with invalid input"""
        player_instance.handleResponseConnectNbr("invalid")
        
        player_instance.logger.error.assert_called_with("Invalid connect nbr: invalid")

    def test_get_steps_from_direction(self, player_instance):
        """Test getStepsFromDirection method"""
        player_instance.goToIncantationState["direction"] = 1
        steps = player_instance.getStepsFromDirection()
        
        assert len(steps) == 1
        assert steps[0] == player_instance.communication.sendForward

    def test_get_steps_from_direction_complex(self, player_instance):
        """Test getStepsFromDirection with complex direction"""
        player_instance.goToIncantationState["direction"] = 2
        steps = player_instance.getStepsFromDirection()
        
        assert len(steps) == 3
        assert steps[0] == player_instance.communication.sendForward
        assert steps[1] == player_instance.communication.sendLeft
        assert steps[2] == player_instance.communication.sendForward

    def test_handle_message_teamslots(self, player_instance):
        """Test handleMessageTeamslots method"""
        player_instance.handleMessageTeamslots(0, "5")
        assert player_instance.nbTeamSlots == 5

    def test_handle_message_teamslots_invalid(self, player_instance):
        """Test handleMessageTeamslots with invalid input"""
        player_instance.handleMessageTeamslots(0, "invalid")
        player_instance.logger.error.assert_called_with("Invalid number of team slots: invalid")

    def test_handle_message_send_inventory(self, player_instance):
        """Test handleMessageSendInventory method"""
        player_instance.id = "TestTeam-12345"
        player_instance.handleMessageSendInventory(0, "OtherTeam-67890")
        
        assert player_instance.senderID == "OtherTeam-67890"
        assert player_instance.needToBroadcastInventory == True
        player_instance.communication.sendInventory.assert_called_once()

    def test_handle_message_send_inventory_own_id(self, player_instance):
        """Test handleMessageSendInventory with own ID (should ignore)"""
        player_instance.id = "TestTeam-12345"
        player_instance.handleMessageSendInventory(0, "TestTeam-12345")
        
        player_instance.communication.sendInventory.assert_not_called()

    def test_handle_message_inventory(self, player_instance):
        """Test handleMessageInventory method"""
        player_instance.id = "TestTeam-12345"
        message = "1,2,3,4,5,6,10 OtherTeam-67890 TestTeam-12345"
        
        player_instance.handleMessageInventory(0, message)
        
        assert len(player_instance.roombaState["teamInventories"]) == 1
        inventory = player_instance.roombaState["teamInventories"][0]
        assert inventory["linemate"] == 1
        assert inventory["deraumere"] == 2
        assert inventory["food"] == 10
        assert inventory["id"] == "OtherTeam-67890"

    def test_handle_message_inventory_invalid_format(self, player_instance):
        """Test handleMessageInventory with invalid format"""
        player_instance.handleMessageInventory(0, "invalid format")
        assert len(player_instance.roombaState["teamInventories"]) == 0

    def test_handle_message_come_incant(self, player_instance):
        """Test handleMessageComeIncant method"""
        with patch('os.getpid', return_value=12345):
            player_instance.pid = 12345
            player_instance.id = "TestTeam-12345"
            
            # Higher PID should trigger go to incantation
            player_instance.handleMessageComeIncant(1, "TestTeam-54321")
            
            assert player_instance.goToIncantationState["status"] == True
            assert player_instance.goToIncantationState["direction"] == 1
            player_instance.broadcaster.broadcastMessage.assert_called_with(f"whereAreYou {player_instance.id}")

    def test_handle_message_come_incant_lower_pid(self, player_instance):
        """Test handleMessageComeIncant with lower PID (should ignore)"""
        with patch('os.getpid', return_value=54321):
            player_instance.pid = 54321
            player_instance.id = "TestTeam-54321"
            
            # Lower PID should be ignored
            player_instance.handleMessageComeIncant(1, "TestTeam-12345")
            
            assert player_instance.goToIncantationState["status"] == False

    def test_handle_message_where_are_you(self, player_instance):
        """Test handleMessageWhereAreYou method"""
        player_instance.incantationState["status"] = True
        player_instance.id = "TestTeam-12345"
        
        player_instance.handleMessageWhereAreYou(2, "OtherTeam-67890")
        
        player_instance.broadcaster.broadcastMessage.assert_called_with("here TestTeam-12345 OtherTeam-67890")
        assert len(player_instance.incantationState["playerResponses"]) == 1
        assert player_instance.incantationState["playerResponses"][0]["id"] == "OtherTeam-67890"
        assert player_instance.incantationState["playerResponses"][0]["direction"] == 2

    def test_handle_message_here(self, player_instance):
        """Test handleMessageHere method"""
        player_instance.goToIncantationState["status"] = True
        player_instance.id = "TestTeam-12345"
        
        player_instance.handleMessageHere(3, "OtherTeam-67890 TestTeam-12345")
        
        assert player_instance.goToIncantationState["direction"] == 3
        assert player_instance.goToIncantationState["arrived"] == False

    def test_handle_message_here_arrived(self, player_instance):
        """Test handleMessageHere when already arrived (direction 0)"""
        player_instance.goToIncantationState["status"] = True
        player_instance.id = "TestTeam-12345"
        
        player_instance.handleMessageHere(0, "OtherTeam-67890 TestTeam-12345")
        
        assert player_instance.goToIncantationState["direction"] == 0
        assert player_instance.goToIncantationState["arrived"] == True

    def test_handle_message_drop_stones(self, player_instance):
        """Test handleMessageDropStones method"""
        player_instance.goToIncantationState["status"] = True
        player_instance.goToIncantationState["arrived"] = True
        player_instance.id = "TestTeam-12345"
        
        player_instance.handleMessageDropStones(0, "OtherTeam-67890")
        
        assert player_instance.goToIncantationState["droppingStones"] == True

    def test_incantation_action_send_come_incant(self, player_instance):
        """Test incantationAction in sendComeIncant phase"""
        player_instance.incantationState["phase"] = "sendComeIncant"
        player_instance.id = "TestTeam-12345"
        
        player_instance.incantationAction()
        
        player_instance.broadcaster.broadcastMessage.assert_called_with("comeIncant TestTeam-12345")
        assert player_instance.incantationState["phase"] == "sendConnectNbr"

    def test_incantation_action_send_connect_nbr(self, player_instance):
        """Test incantationAction in sendConnectNbr phase"""
        player_instance.incantationState["phase"] = "sendConnectNbr"
        
        player_instance.incantationAction()
        
        player_instance.communication.sendGetConnectNbr.assert_called_once()
        assert player_instance.incantationState["phase"] == "waitForWhereAreYou"

    def test_incantation_action_wait_for_where_are_you(self, player_instance):
        """Test incantationAction in waitForWhereAreYou phase"""
        player_instance.incantationState["phase"] = "waitForWhereAreYou"
        player_instance.nbConnectedPlayers = 3
        player_instance.incantationState["playerResponses"] = [
            {"id": "player1", "direction": 0},
            {"id": "player2", "direction": 0}
        ]
        
        player_instance.incantationAction()
        
        assert player_instance.incantationState["phase"] == "dropStones"

    def test_go_to_incantation_action_dropping_stones(self, player_instance):
        """Test goToIncantationAction while dropping stones"""
        player_instance.goToIncantationState["droppingStones"] = True
        player_instance.level = 2
        
        player_instance.goToIncantationAction()
        
        assert player_instance.goToIncantationState["droppingStones"] == False
        player_instance.communication.sendInventory.assert_called_once()

    def test_go_to_incantation_action_arrived(self, player_instance):
        """Test goToIncantationAction when arrived"""
        player_instance.goToIncantationState["direction"] = 0
        player_instance.goToIncantationState["movementStarted"] = True
        player_instance.id = "TestTeam-12345"
        
        player_instance.goToIncantationAction()
        
        assert player_instance.goToIncantationState["arrived"] == True
        player_instance.broadcaster.broadcastMessage.assert_called_with("whereAreYou TestTeam-12345")

    def test_handle_command_response_inventory(self, player_instance):
        """Test handleCommandResponse with inventory response"""
        with patch.object(player_instance, 'handleResponseInventory') as mock_handle:
            player_instance.handleCommandResponse("inventory linemate 1 deraumere 0")
            mock_handle.assert_called_once()

    def test_handle_command_response_look(self, player_instance):
        """Test handleCommandResponse with look response"""
        with patch.object(player_instance, 'handleResponseLook') as mock_handle:
            player_instance.handleCommandResponse("look [food]")
            mock_handle.assert_called_once()

    def test_handle_command_response_ko(self, player_instance):
        """Test handleCommandResponse with ko response"""
        with patch.object(player_instance, 'handleResponseKO') as mock_handle:
            player_instance.handleCommandResponse("ko")
            mock_handle.assert_called_once()

    def test_handle_command_response_ok(self, player_instance):
        """Test handleCommandResponse with ok response"""
        with patch.object(player_instance, 'handleResponseOK') as mock_handle:
            player_instance.handleCommandResponse("ok")
            mock_handle.assert_called_once()

    def test_handle_command_response_elevation(self, player_instance):
        """Test handleCommandResponse with elevation response"""
        with patch.object(player_instance, 'handleResponseElevationUnderway') as mock_handle:
            player_instance.handleCommandResponse("Elevation underway")
            mock_handle.assert_called_once()

    def test_handle_command_response_current_level(self, player_instance):
        """Test handleCommandResponse with current level response"""
        with patch.object(player_instance, 'handleResponseCurrentLevel') as mock_handle:
            player_instance.handleCommandResponse("Current level: 3")
            mock_handle.assert_called_with("3")

    def test_handle_command_response_connect_nbr(self, player_instance):
        """Test handleCommandResponse with connect number response"""
        with patch.object(player_instance, 'handleResponseConnectNbr') as mock_handle:
            player_instance.handleCommandResponse("5")
            mock_handle.assert_called_with("5")

    def test_handle_command_response_unknown(self, player_instance):
        """Test handleCommandResponse with unknown response"""
        player_instance.roombaState["lastCommand"] = "test_command"
        player_instance.handleCommandResponse("unknown response")
        
        player_instance.logger.error.assert_called_with("Unknown response to 'test_command': unknown response")

    def test_handle_messages_teamslots(self, player_instance):
        """Test handleMessages with teamslots message"""
        with patch.object(player_instance, 'handleMessageTeamslots') as mock_handle:
            player_instance.handleMessages(0, "teamslots 5")
            mock_handle.assert_called_with(0, "5")

    def test_handle_messages_send_inventory(self, player_instance):
        """Test handleMessages with sendInventory message"""
        with patch.object(player_instance, 'handleMessageSendInventory') as mock_handle:
            player_instance.handleMessages(0, "sendInventory TestTeam-67890")
            mock_handle.assert_called_with(0, "TestTeam-67890")

    def test_handle_messages_unknown(self, player_instance):
        """Test handleMessages with unknown message"""
        player_instance.handleMessages(0, "unknown message")
        player_instance.logger.error.assert_called_with("Unknown message: unknown message")

    @patch('os.fork')
    def test_create_child_success(self, mock_fork, player_instance):
        """Test successful child creation"""
        mock_fork.return_value = 12345
        
        result = player_instance.create_child()
        
        assert result == 12345
        mock_fork.assert_called_once()

    @patch('os.fork')
    def test_create_child_failure(self, mock_fork, player_instance):
        """Test failed child creation"""
        mock_fork.return_value = -1
        
        result = player_instance.create_child()
        
        assert result == -1

    @patch('os.fork')
    @patch('sys.exit')
    def test_create_child_in_child_process(self, mock_exit, mock_fork, player_instance):
        """Test child process execution"""
        mock_fork.return_value = 0
        
        # Mock sys.exit to prevent actual exit
        mock_exit.side_effect = SystemExit(0)
        
        with patch('src.Player.Player.Player') as mock_player_class:
            mock_child_player = Mock()
            mock_player_class.return_value = mock_child_player
            
            # This should raise SystemExit due to the child process exit
            with pytest.raises(SystemExit):
                player_instance.create_child()

    def test_start_com_thread(self, player_instance):
        """Test startComThread method"""
        mock_thread = Mock()
        player_instance.commThread = mock_thread
        
        player_instance.startComThread()
        
        mock_thread.start.assert_called_once()

    def test_player_destructor(self, player_instance):
        """Test Player destructor behavior"""
        mock_thread = Mock()
        mock_thread.is_alive.return_value = True
        player_instance.commThread = mock_thread
        
        # Call destructor
        player_instance.__del__()
        
        player_instance.communication.stopLoop.assert_called_once()
        mock_thread.join.assert_called_with(timeout=1.0)

    def test_player_destructor_exception_handling(self, player_instance):
        """Test Player destructor handles exceptions gracefully"""
        mock_thread = Mock()
        mock_thread.is_alive.side_effect = Exception("Thread error")
        player_instance.commThread = mock_thread
        
        # Should not raise exception
        player_instance.__del__()

    def test_team_has_enough_stones_with_team_inventories(self, player_instance):
        """Test doesTeamHaveEnoughStones with team inventories"""
        player_instance.inventory = {
            "food": 10, "linemate": 5, "deraumere": 4, "sibur": 5,
            "mendiane": 2, "phiras": 3, "thystame": 0
        }
        player_instance.roombaState["teamInventories"] = [
            {
                "linemate": 4, "deraumere": 4, "sibur": 5,
                "mendiane": 3, "phiras": 3, "thystame": 1, "food": 10, "id": "teammate1"
            }
        ]
        
        result = player_instance.doesTeamHaveEnoughStones()
        assert result == True

    def test_team_has_enough_food_with_team_inventories(self, player_instance):
        """Test teamHasEnoughFoodForIncantation with team inventories"""
        player_instance.inventory["food"] = 100
        player_instance.x = 10
        player_instance.y = 10
        player_instance.roombaState["teamInventories"] = [
            {"food": 100, "linemate": 0, "deraumere": 0, "sibur": 0,
             "mendiane": 0, "phiras": 0, "thystame": 0, "id": "teammate1"}
        ]
        
        result = player_instance.teamHasEnoughFoodForIncantation()
        assert result == True

    def test_roomba_action_check_on_teammates_broadcast_send_inventory(self, player_instance):
        """Test roombaAction in checkOnTeammates phase with Connect_nbr command"""
        player_instance.roombaState["phase"] = "checkOnTeammates"
        player_instance.roombaState["lastCommand"] = "Connect_nbr"
        player_instance.id = "TestTeam-12345"
        
        player_instance.roombaAction()
        
        player_instance.broadcaster.broadcastMessage.assert_called_with("sendInventory TestTeam-12345")
        assert player_instance.roombaState["lastCommand"] == "broadcast sendInventory"

    def test_roomba_action_check_on_teammates_enough_teammates(self, player_instance):
        """Test roombaAction when enough teammates responded"""
        player_instance.roombaState["phase"] = "checkOnTeammates"
        player_instance.roombaState["lastCommand"] = "broadcast sendInventory"
        player_instance.nbConnectedPlayers = 3
        player_instance.roombaState["teamInventories"] = [
            {"id": "teammate1", "food": 100, "linemate": 10, "deraumere": 10, "sibur": 10,
             "mendiane": 10, "phiras": 10, "thystame": 10},
            {"id": "teammate2", "food": 100, "linemate": 10, "deraumere": 10, "sibur": 10,
             "mendiane": 10, "phiras": 10, "thystame": 10}
        ]
        
        # Mock enough stones and food
        with patch.object(player_instance, 'doesTeamHaveEnoughStones', return_value=True), \
             patch.object(player_instance, 'teamHasEnoughFoodForIncantation', return_value=True):
            
            player_instance.roombaAction()
            
            assert player_instance.roombaState["phase"] == "forward"

    def test_roomba_action_turn_right_then_forward(self, player_instance):
        """Test roombaAction turn phase - right then forward"""
        player_instance.roombaState["phase"] = "turn"
        player_instance.roombaState["lastCommand"] = "right"
        
        player_instance.roombaAction()
        
        player_instance.communication.sendForward.assert_called_once()
        assert player_instance.roombaState["lastCommand"] == "forward"

    def test_roomba_action_turn_forward_then_left(self, player_instance):
        """Test roombaAction turn phase - forward then left"""
        player_instance.roombaState["phase"] = "turn"
        player_instance.roombaState["lastCommand"] = "forward"
        
        player_instance.roombaAction()
        
        player_instance.communication.sendLeft.assert_called_once()
        assert player_instance.roombaState["lastCommand"] == "left"
        assert player_instance.roombaState["phase"] == "lookAround"

    def test_incantation_action_drop_stones(self, player_instance):
        """Test incantationAction in dropStones phase"""
        player_instance.incantationState["phase"] = "dropStones"
        player_instance.id = "TestTeam-12345"
        
        player_instance.incantationAction()
        
        player_instance.broadcaster.broadcastMessage.assert_called_with("dropStones TestTeam-12345")
        assert player_instance.incantationState["phase"] == "droppingStones"

    def test_incantation_action_start_incantation_insufficient_food(self, player_instance):
        """Test incantationAction startIncantation phase with insufficient food"""
        player_instance.incantationState["phase"] = "startIncantation"
        player_instance.x = 100
        player_instance.y = 100
        player_instance.inventory["food"] = 1
        
        player_instance.incantationAction()
        
        assert player_instance.incantationState["status"] == False
        assert player_instance.incantationState["phase"] == "sendComeIncant"
        player_instance.logger.error.assert_called_with("Not enough food for incantation, resetting state")

    def test_incantation_action_start_incantation_sufficient_food(self, player_instance):
        """Test incantationAction startIncantation phase with sufficient food"""
        player_instance.incantationState["phase"] = "startIncantation"
        player_instance.x = 10
        player_instance.y = 10
        player_instance.inventory["food"] = 100
        
        # Mock sufficient food
        with patch.object(player_instance, 'teamHasEnoughFoodForIncantation', return_value=True):
            player_instance.incantationAction()
        
        player_instance.communication.sendIncantation.assert_called_once()
        assert player_instance.incantationState["phase"] == "waitingForElevation"

    def test_incantation_action_waiting_for_elevation(self, player_instance):
        """Test incantationAction in waitingForElevation phase (should do nothing)"""
        player_instance.incantationState["phase"] = "waitingForElevation"
        
        # Should not call any methods
        player_instance.incantationAction()
        
        # Verify no communication methods were called
        player_instance.communication.sendIncantation.assert_not_called()

    def test_go_to_incantation_action_need_to_wait(self, player_instance):
        """Test goToIncantationAction when needing to wait"""
        player_instance.goToIncantationState["needToWait"] = True
        
        with patch('src.Player.Player.sleep') as mock_sleep:
            player_instance.goToIncantationAction()
            mock_sleep.assert_called_with(0.5)

    def test_go_to_incantation_action_no_steps(self, player_instance):
        """Test goToIncantationAction with no steps"""
        player_instance.goToIncantationState["steps"] = []
        player_instance.goToIncantationState["needToWait"] = False
        player_instance.id = "TestTeam-12345"
        
        player_instance.goToIncantationAction()
        
        player_instance.broadcaster.broadcastMessage.assert_called_with("whereAreYou TestTeam-12345")
        assert player_instance.goToIncantationState["needToWait"] == True

    def test_go_to_incantation_action_execute_step(self, player_instance):
        """Test goToIncantationAction executing a step"""
        mock_step = Mock()
        player_instance.goToIncantationState["steps"] = [mock_step]
        player_instance.goToIncantationState["needToWait"] = False
        
        player_instance.goToIncantationAction()
        
        mock_step.assert_called_once()
        assert player_instance.goToIncantationState["movementStarted"] == True
        assert len(player_instance.goToIncantationState["steps"]) == 0

    def test_handle_response_inventory_with_broadcast_needed(self, player_instance):
        """Test handleResponseInventory when broadcast is needed"""
        new_inventory = {
            "food": 15, "linemate": 2, "deraumere": 1, "sibur": 0,
            "mendiane": 0, "phiras": 0, "thystame": 0
        }
        player_instance.communication.getInventory.return_value = new_inventory
        player_instance.needToBroadcastInventory = True
        player_instance.senderID = "OtherTeam-67890"
        player_instance.id = "TestTeam-12345"
        
        player_instance.handleResponseInventory()
        
        expected_message = "inventory 2,1,0,0,0,0,15 TestTeam-12345 OtherTeam-67890"
        player_instance.broadcaster.broadcastMessage.assert_called_with(expected_message)
        assert player_instance.needToBroadcastInventory == False

    def test_handle_response_inventory_trigger_check_teammates(self, player_instance):
        """Test handleResponseInventory triggering check teammates phase"""
        current_inventory = {
            "food": 10, "linemate": 1, "deraumere": 0, "sibur": 0,
            "mendiane": 0, "phiras": 0, "thystame": 0
        }
        new_inventory = {
            "food": 100, "linemate": 2, "deraumere": 1, "sibur": 0,
            "mendiane": 0, "phiras": 0, "thystame": 0
        }
        
        player_instance.inventory = current_inventory
        player_instance.communication.getInventory.return_value = new_inventory
        player_instance.needToBroadcastInventory = False
        player_instance.incantationState["status"] = False
        player_instance.goToIncantationState["status"] = False
        player_instance.roombaState["lastPhase"] = "updateInventory"
        player_instance.nbTeamSlots = 5
        player_instance.x = 10
        player_instance.y = 10
        
        player_instance.handleResponseInventory()
        
        assert player_instance.roombaState["phase"] == "checkOnTeammates"

    def test_handle_response_ko_incantation_failed(self, player_instance):
        """Test handleResponseKO when incantation fails"""
        player_instance.incantationState["status"] = True
        player_instance.incantationState["lastCommand"] = "incantation"
        
        player_instance.handleResponseKO()
        
        assert player_instance.incantationState["status"] == False
        assert player_instance.incantationState["phase"] == "sendComeIncant"
        player_instance.logger.error.assert_called_with("Incantation failed, resetting incantation state")

    def test_handle_response_ko_check_teammates_phase(self, player_instance):
        """Test handleResponseKO in checkOnTeammates phase"""
        player_instance.roombaState["phase"] = "checkOnTeammates"
        player_instance.roombaState["lastCommand"] = "Connect_nbr"
        
        player_instance.handleResponseKO()
        
        assert player_instance.roombaState["phase"] == "forward"

    def test_handle_response_current_level_reset_states(self, player_instance):
        """Test handleResponseCurrentLevel resetting incantation states at max level"""
        player_instance.level = 7
        player_instance.incantationState["status"] = True
        player_instance.goToIncantationState["status"] = True
        
        player_instance.handleResponseCurrentLevel("8")
        
        assert player_instance.level == 8
        assert player_instance.incantationState["status"] == False
        assert player_instance.goToIncantationState["status"] == False

    def test_handle_response_current_level_unexpected(self, player_instance):
        """Test handleResponseCurrentLevel with unexpected level"""
        player_instance.level = 5
        player_instance.handleResponseCurrentLevel("3")
        
        player_instance.logger.error.assert_called_with("Unexpected level response: got 3, old level = 5")

    def test_handle_message_inventory_wrong_demander(self, player_instance):
        """Test handleMessageInventory with wrong demander ID"""
        player_instance.id = "TestTeam-12345"
        message = "1,2,3,4,5,6,10 OtherTeam-67890 WrongTeam-11111"
        
        player_instance.handleMessageInventory(0, message)
        
        # Should not add to team inventories
        assert len(player_instance.roombaState["teamInventories"]) == 0

    def test_handle_message_inventory_duplicate_responder(self, player_instance):
        """Test handleMessageInventory with duplicate responder"""
        player_instance.id = "TestTeam-12345"
        player_instance.roombaState["teamInventories"] = [
            {"id": "OtherTeam-67890", "food": 5}
        ]
        message = "1,2,3,4,5,6,10 OtherTeam-67890 TestTeam-12345"
        
        player_instance.handleMessageInventory(0, message)
        
        # Should still have only one inventory (no duplicate)
        assert len(player_instance.roombaState["teamInventories"]) == 1

    def test_handle_message_come_incant_invalid_format(self, player_instance):
        """Test handleMessageComeIncant with invalid message format"""
        player_instance.handleMessageComeIncant(1, "InvalidFormat")
        
        player_instance.logger.error.assert_called_with("Invalid contents in comeIncant message: InvalidFormat")

    def test_handle_message_come_incant_non_digit_pid(self, player_instance):
        """Test handleMessageComeIncant with non-digit PID"""
        player_instance.handleMessageComeIncant(1, "TestTeam-abc")
        
        player_instance.logger.error.assert_called_with("Invalid contents in comeIncant message: TestTeam-abc")

    def test_handle_message_come_incant_reset_incantation(self, player_instance):
        """Test handleMessageComeIncant resetting current incantation"""
        with patch('os.getpid', return_value=12345):
            player_instance.pid = 12345
            player_instance.id = "TestTeam-12345"
            player_instance.incantationState["status"] = True
            
            # Higher PID should reset current incantation
            player_instance.handleMessageComeIncant(1, "TestTeam-54321")
            
            assert player_instance.incantationState["status"] == False

    def test_handle_message_where_are_you_not_in_incantation(self, player_instance):
        """Test handleMessageWhereAreYou when not in incantation"""
        player_instance.incantationState["status"] = False
        player_instance.id = "TestTeam-12345"
        
        player_instance.handleMessageWhereAreYou(2, "OtherTeam-67890")
        
        # Should not broadcast or add to responses
        player_instance.broadcaster.broadcastMessage.assert_not_called()
        assert len(player_instance.incantationState["playerResponses"]) == 0

    def test_handle_message_where_are_you_update_existing_response(self, player_instance):
        """Test handleMessageWhereAreYou updating existing player response"""
        player_instance.incantationState["status"] = True
        player_instance.id = "TestTeam-12345"
        player_instance.incantationState["playerResponses"] = [
            {"id": "OtherTeam-67890", "direction": 1}
        ]
        
        player_instance.handleMessageWhereAreYou(3, "OtherTeam-67890")
        
        # Should update existing response
        assert len(player_instance.incantationState["playerResponses"]) == 1
        assert player_instance.incantationState["playerResponses"][0]["direction"] == 3

    def test_handle_message_here_invalid_format(self, player_instance):
        """Test handleMessageHere with invalid message format"""
        player_instance.handleMessageHere(0, "InvalidFormat")
        
        player_instance.logger.error.assert_called_with("Invalid contents in here message: InvalidFormat")

    def test_handle_message_here_wrong_demander(self, player_instance):
        """Test handleMessageHere with wrong demander ID"""
        player_instance.id = "TestTeam-12345"
        player_instance.goToIncantationState["status"] = True
        
        player_instance.handleMessageHere(3, "OtherTeam-67890 WrongTeam-11111")
        
        # Should not update go to incantation state
        assert player_instance.goToIncantationState["direction"] != 3

    def test_handle_message_drop_stones_not_arrived(self, player_instance):
        """Test handleMessageDropStones when not arrived"""
        player_instance.goToIncantationState["status"] = True
        player_instance.goToIncantationState["arrived"] = False
        player_instance.id = "TestTeam-12345"
        
        player_instance.handleMessageDropStones(0, "OtherTeam-67890")
        
        # Should not set dropping stones
        assert player_instance.goToIncantationState["droppingStones"] == False

    def test_loop_dead_player(self, player_instance):
        """Test loop method when player is dead"""
        player_instance.communication.playerIsDead.return_value = True
        
        # Should exit loop immediately
        player_instance.loop()
        
        # Verify stopLoop was called in finally block
        player_instance.communication.stopLoop.assert_called_once()

    def test_loop_with_messages(self, player_instance):
        """Test loop method processing messages"""
        # Setup to run one iteration then exit
        call_count = [0]
        def mock_player_is_dead():
            call_count[0] += 1
            return call_count[0] > 1
        
        player_instance.communication.playerIsDead.side_effect = mock_player_is_dead
        player_instance.communication.hasMessages.return_value = True
        player_instance.communication.getLastMessage.return_value = (1, "hashed_message")
        player_instance.broadcaster.revealMessage.return_value = "teamslots 5"
        
        with patch.object(player_instance, 'handleMessages') as mock_handle:
            player_instance.loop()
            mock_handle.assert_called_with(1, "teamslots 5")

    def test_loop_with_responses(self, player_instance):
        """Test loop method processing responses"""
        call_count = [0]
        def mock_player_is_dead():
            call_count[0] += 1
            return call_count[0] > 1
        
        player_instance.communication.playerIsDead.side_effect = mock_player_is_dead
        player_instance.communication.hasMessages.return_value = False
        player_instance.communication.hasResponses.return_value = True
        player_instance.communication.getLastResponse.return_value = "ok"
        
        with patch.object(player_instance, 'handleCommandResponse') as mock_handle:
            player_instance.loop()
            mock_handle.assert_called_with("ok")

    def test_loop_dead_response(self, player_instance):
        """Test loop method with dead response"""
        call_count = [0]
        def mock_player_is_dead():
            call_count[0] += 1
            return call_count[0] > 1
        
        player_instance.communication.playerIsDead.side_effect = mock_player_is_dead
        player_instance.communication.hasMessages.return_value = False
        player_instance.communication.hasResponses.return_value = True
        player_instance.communication.getLastResponse.return_value = "dead"
        
        # Mock logger.display to prevent actual logging during test
        player_instance.logger.display = Mock()
        
        player_instance.loop()
        
        player_instance.logger.display.assert_called_with("Player died")

    def test_loop_broadcast_team_slots(self, player_instance):
        """Test loop method broadcasting team slots"""
        call_count = [0]
        def mock_player_is_dead():
            call_count[0] += 1
            return call_count[0] > 1
        
        player_instance.communication.playerIsDead.side_effect = mock_player_is_dead
        player_instance.communication.hasMessages.return_value = False
        player_instance.communication.hasResponses.return_value = False
        player_instance.sentNbSlots = False
        player_instance.nbConnectedPlayers = 5
        player_instance.nbTeamSlots = 5
        
        player_instance.loop()
        
        player_instance.broadcaster.broadcastMessage.assert_called_with("teamslots 5")
        assert player_instance.sentNbSlots == True

    def test_loop_get_connect_nbr(self, player_instance):
        """Test loop method getting connect number"""
        call_count = [0]
        def mock_player_is_dead():
            call_count[0] += 1
            return call_count[0] > 1
        
        player_instance.communication.playerIsDead.side_effect = mock_player_is_dead
        player_instance.communication.hasMessages.return_value = False
        player_instance.communication.hasResponses.return_value = False
        player_instance.sentNbSlots = False
        player_instance.nbConnectedPlayers = 3
        player_instance.nbTeamSlots = 5
        
        with patch('time.sleep'):  # Mock sleep to speed up test
            player_instance.loop()
        
        player_instance.communication.sendGetConnectNbr.assert_called()

    def test_loop_action_execution(self, player_instance):
        """Test loop method executing actions"""
        call_count = [0]
        def mock_player_is_dead():
            call_count[0] += 1
            return call_count[0] > 1
        
        player_instance.communication.playerIsDead.side_effect = mock_player_is_dead
        player_instance.communication.hasMessages.return_value = False
        player_instance.communication.hasResponses.return_value = False
        player_instance.communication.hasRequests.return_value = False
        player_instance.communication.hasPendingCommands.return_value = False
        player_instance.inIncantation = False
        player_instance.sentNbSlots = True
        player_instance.incantationState["status"] = False
        player_instance.goToIncantationState["status"] = False
        
        with patch.object(player_instance, 'roombaAction') as mock_roomba:
            player_instance.loop()
            mock_roomba.assert_called_once()

    def test_loop_incantation_action(self, player_instance):
        """Test loop method executing incantation action"""
        call_count = [0]
        def mock_player_is_dead():
            call_count[0] += 1
            return call_count[0] > 1
        
        player_instance.communication.playerIsDead.side_effect = mock_player_is_dead
        player_instance.communication.hasMessages.return_value = False
        player_instance.communication.hasResponses.return_value = False
        player_instance.communication.hasRequests.return_value = False
        player_instance.communication.hasPendingCommands.return_value = False
        player_instance.inIncantation = False
        player_instance.sentNbSlots = True
        player_instance.incantationState["status"] = True
        
        with patch.object(player_instance, 'incantationAction') as mock_incantation:
            player_instance.loop()
            mock_incantation.assert_called_once()

    def test_loop_go_to_incantation_action(self, player_instance):
        """Test loop method executing go to incantation action"""
        call_count = [0]
        def mock_player_is_dead():
            call_count[0] += 1
            return call_count[0] > 1
        
        player_instance.communication.playerIsDead.side_effect = mock_player_is_dead
        player_instance.communication.hasMessages.return_value = False
        player_instance.communication.hasResponses.return_value = False
        player_instance.communication.hasRequests.return_value = False
        player_instance.communication.hasPendingCommands.return_value = False
        player_instance.inIncantation = False
        player_instance.sentNbSlots = True
        player_instance.incantationState["status"] = False
        player_instance.goToIncantationState["status"] = True
        
        with patch.object(player_instance, 'goToIncantationAction') as mock_go_to:
            player_instance.loop()
            mock_go_to.assert_called_once()

    def test_loop_exception_handling(self, player_instance):
        """Test loop method exception handling"""
        player_instance.communication.playerIsDead.side_effect = CommunicationException("Test error")
        
        player_instance.loop()
        
        player_instance.logger.error.assert_called_with("Communication exception: Test error")
        player_instance.communication.stopLoop.assert_called_once()

    def test_get_needed_stones_priority_sorting(self, player_instance):
        """Test getNeededStonesByPriority sorting by quantity needed"""
        player_instance.level = 2  # Needs: linemate: 1, deraumere: 1, sibur: 1
        player_instance.inventory = {
            "food": 10, "linemate": 1, "deraumere": 0, "sibur": 0,
            "mendiane": 0, "phiras": 0, "thystame": 0
        }
        
        needed_stones = player_instance.getNeededStonesByPriority()
        
        # Should need deraumere: 1, sibur: 1 (both equally needed)
        assert len(needed_stones) == 2
        # Both stones are needed equally, so order may vary but both should be present
        stone_names = [stone for stone, _ in needed_stones]
        assert "deraumere" in stone_names
        assert "sibur" in stone_names

    def test_initialization_with_custom_port(self):
        """Test Player initialization with custom port"""
        with patch('src.Player.Player.Communication'), \
             patch('src.Player.Player.Broadcaster'), \
             patch('src.Player.Player.Logger'), \
             patch('src.Player.Player.Thread'), \
             patch('os.getpid', return_value=12345):
            
            player = Player("CustomTeam", "192.168.1.1", 8080)
            
            assert player.teamName == "CustomTeam"
            assert player.ip == "192.168.1.1"
            assert player.port == 8080

    def test_handle_response_current_level_reset_phase_not_status(self, player_instance):
        """Test handleResponseCurrentLevel resetting incantation phase but not status for level < 8"""
        player_instance.level = 6
        player_instance.incantationState["status"] = True
        player_instance.incantationState["phase"] = "waitingForElevation"
        player_instance.goToIncantationState["status"] = True
        
        player_instance.handleResponseCurrentLevel("7")
        
        assert player_instance.level == 7
        assert player_instance.incantationState["status"] == True  # Status remains True
        assert player_instance.incantationState["phase"] == "sendComeIncant"  # Phase resets
        assert player_instance.goToIncantationState["status"] == True  # Status remains True
        assert player_instance.goToIncantationState["arrived"] == True  # But arrived is set

