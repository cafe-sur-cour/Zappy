#
# EPITECH PROJECT, 2025
# zappy
# File description:
# test_player.py
#
# EPITECH PROJECT, 2025
# zappy
# File description:
# test_player.py
#

import pytest
import sys
import os
from unittest.mock import patch, MagicMock, Mock
from time import sleep
from src.Player.Player import Player
from src.Hash.Hash import Hash
from src.Exceptions.Exceptions import PlayerDead

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '../../../../ai'))


class TestPlayer:
    
    @patch('src.Player.Player.Communication')
    def test_player_initialization_default_port(self, mock_communication):
        """Test player initialization with default port"""
        mock_comm_instance = Mock()
        mock_communication.return_value = mock_comm_instance
        
        player = Player("team1", "127.0.0.1")
        
        assert player.teamName == "team1"
        assert player.ip == "127.0.0.1"
        assert player.port == 4242
        assert player.level == 1
        assert not player.communication._is_dead() is True
        assert player.in_incantation is False
        assert player.inventory == {
            "food": 10, "linemate": 0, "deraumere": 0, "sibur": 0,
            "mendiane": 0, "phiras": 0, "thystame": 0
        }
        mock_comm_instance.connectToServer.assert_called_once()

    @patch('src.Player.Player.Communication')
    def test_player_initialization_custom_port(self, mock_communication):
        """Test player initialization with custom port"""
        mock_comm_instance = Mock()
        mock_communication.return_value = mock_comm_instance
        
        player = Player("team2", "192.168.1.1", 8080)
        
        assert player.teamName == "team2"
        assert player.ip == "192.168.1.1"
        assert player.port == 8080
        mock_communication.assert_called_with("team2", "192.168.1.1", 8080)

    @patch('src.Player.Player.Communication')
    def test_player_str_representation(self, mock_communication):
        """Test player string representation"""
        mock_comm_instance = Mock()
        mock_comm_instance.is_dead.return_value = False  # ✅ Ajout clé
        mock_communication.return_value = mock_comm_instance

        player = Player("team1", "127.0.0.1")
        player.level = 3
        player.inventory["food"] = 5

        expected_str = ("Player team: team1, Level: 3, "
                        "Inventory: {'food': 5, 'linemate': 0, 'deraumere': 0, 'sibur': 0, "
                        "'mendiane': 0, 'phiras': 0, 'thystame': 0}, "
                        "Alive: True, In Incantation: False")

        assert str(player) == expected_str

    @patch('src.Player.Player.Communication')
    def test_begin_incantation(self, mock_communication):
        """Test begin incantation method"""
        mock_comm_instance = Mock()
        mock_communication.return_value = mock_comm_instance
        
        player = Player("team1", "127.0.0.1")
        assert player.in_incantation is False
        
        with patch('builtins.print') as mock_print:
            player.begin_incantation()
        
        assert player.in_incantation is True
        mock_print.assert_called_with("Incantation started!")

    @patch('src.Player.Player.Communication')
    def test_lay_an_egg(self, mock_communication):
        """Test lay an egg method"""
        mock_comm_instance = Mock()
        mock_communication.return_value = mock_comm_instance
        
        player = Player("team1", "127.0.0.1")
        
        with patch('builtins.print') as mock_print:
            player.lay_an_egg()
        
        mock_print.assert_called_with("Lay a new egg!")

    @patch('src.Player.Player.Communication')
    def test_loop_normal_execution(self, mock_communication):
        """Test player loop with normal execution"""
        mock_comm_instance = Mock()
        mock_communication.return_value = mock_comm_instance
        mock_comm_instance.is_dead.side_effect = [False, True]
        mock_comm_instance.sendForward.return_value = None
        mock_comm_instance.getInventory.return_value = {
            "food": 8, "linemate": 1, "deraumere": 0,
            "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0
        }
        mock_comm_instance.get_size_message_queue.return_value = 0

        player = Player("team1", "127.0.0.1")

        with patch('builtins.print') as mock_print:
            player.loop()

        mock_comm_instance.sendForward.assert_called_once()
        mock_comm_instance.getInventory.assert_called_once()
        assert mock_print.call_count >= 1

    @patch('src.Player.Player.Communication')
    def test_loop_multiple_iterations(self, mock_communication):
        """Test player loop with multiple iterations before death"""
        mock_comm_instance = Mock()
        mock_communication.return_value = mock_comm_instance

        mock_comm_instance.is_dead.side_effect = [False, False, False, True]

        call_count = 0
        def sendForward_side_effect():
            nonlocal call_count
            call_count += 1
            if call_count >= 3:
                raise PlayerDead()

        mock_comm_instance.sendForward.side_effect = sendForward_side_effect
        mock_comm_instance.getInventory.return_value = {
            "food": 5, "linemate": 0, "deraumere": 0, "sibur": 0,
            "mendiane": 0, "phiras": 0, "thystame": 0
        }
        mock_comm_instance.get_size_message_queue.return_value = 0  # éviter appels indésirés

        player = Player("team1", "127.0.0.1")

        with patch('builtins.print'):
            player.loop()

        assert mock_comm_instance.sendForward.call_count == 3
        assert mock_comm_instance.getInventory.call_count == 2

    @patch('src.Player.Player.Communication')
    def test_inventory_update_in_loop(self, mock_communication):
        """Test that inventory is properly updated during loop execution"""
        mock_comm_instance = Mock()
        mock_communication.return_value = mock_comm_instance
        inventory_states = [
            {"food": 8, "linemate": 1, "deraumere": 0, "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0},
            {"food": 7, "linemate": 2, "deraumere": 1, "sibur": 0, "mendiane": 0, "phiras": 0, "thystame": 0}
        ]
        mock_comm_instance.is_dead.side_effect = [False, False, True]
        mock_comm_instance.sendForward.return_value = None
        mock_comm_instance.get_size_message_queue.return_value = 0
        mock_comm_instance.getInventory.side_effect = inventory_states

        player = Player("team1", "127.0.0.1")
        with patch('builtins.print'):
            player.loop()
        assert player.inventory == inventory_states[1]
        assert mock_comm_instance.sendForward.call_count == 2
        assert mock_comm_instance.getInventory.call_count == 2

    @patch('src.Player.Player.Communication')
    def test_player_attributes_modification(self, mock_communication):
        """Test that player attributes can be modified after initialization"""
        mock_comm_instance = Mock()
        mock_communication.return_value = mock_comm_instance
        
        player = Player("team1", "127.0.0.1")
        
        # Modify attributes
        player.level = 5
        player.communication._is_dead= False
        player.in_incantation = True
        player.inventory["food"] = 20
        
        assert player.level == 5
        assert player.communication._is_dead is False
        assert player.in_incantation is True
        assert player.inventory["food"] == 20

    @patch('src.Player.Player.Communication')
    @patch('src.Player.Player.Hash')
    def test_hash_initialization(self, mock_hash, mock_communication):
        """Test that Hash is properly initialized with team name"""
        mock_comm_instance = Mock()
        mock_communication.return_value = mock_comm_instance
        mock_hash_instance = Mock()
        mock_hash.return_value = mock_hash_instance
        
        player = Player("team1", "127.0.0.1")
        
        mock_hash.assert_called_with("team1")
        assert player.hash == mock_hash_instance


