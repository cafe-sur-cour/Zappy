#
# EPITECH PROJECT, 2025
# zappy
# File description:
# test_player.py
#

import pytest
import sys
import os
from unittest.mock import patch, MagicMock
from time import sleep
from src.Player.Player import Player
from src.Hash.Hash import Hash

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '../../../../ai'))


class TestPlayer:
    def setup_method(self):
        self.player = Player("test_team")
    
    def test_player_initialization(self):
        player = Player("team_name")
        assert player.teamName == "team_name"
        assert player.level == 1
        assert isinstance(player.hash, Hash)
        assert player.alive is True
        assert player.in_incantation is False
        
        expected_inventory = {
            "food": 10, "linemate": 0, "deraumere": 0, "sibur": 0,
            "mendiane": 0, "phiras": 0, "thystame": 0
        }
        assert player.inventory == expected_inventory
    
    def test_player_str_representation(self):
        result = str(self.player)
        expected = ("Player team: test_team, "
                   "Level: 1, "
                   "Inventory: {'food': 10, 'linemate': 0, 'deraumere': 0, 'sibur': 0, 'mendiane': 0, 'phiras': 0, 'thystame': 0}, "
                   "Alive: True, "
                   "In Incantation: False")
        assert result == expected
    
    @patch('src.Player.Player.sleep')
    @patch('builtins.print')
    def test_begin_incantation(self, mock_print, mock_sleep):
        initial_level = self.player.level
        self.player.begin_incantation()
        
        assert self.player.level == initial_level + 1
        assert self.player.in_incantation is False
        
        mock_print.assert_any_call("Player team: test_team has started an incantation!")
        mock_print.assert_any_call("Player team: test_team has ended an incantation!")
        mock_sleep.assert_called_once_with(10)
    
    @patch('builtins.print')
    def test_lay_a_egg(self, mock_print):
        self.player.lay_an_egg()
        mock_print.assert_called_once_with("Player team: test_team has laid an egg!")
