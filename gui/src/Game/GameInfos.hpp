/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GameInfos
*/

#ifndef GAMEINFOS_HPP_
#define GAMEINFOS_HPP_

#include <utility>
#include <vector>

#include "../Utils/Constants.hpp"

class GameInfos {
    public:
        GameInfos();
        ~GameInfos();

        void setMapSize(int width, int height);
        std::pair<int, int> getMapSize() const;

        void setTimeUnit(int timeUnit);
        int getTimeUnit() const;

        void updateTile(zappy::structs::Tile tile);
        const std::vector<zappy::structs::Tile> getTiles() const;
        const zappy::structs::Tile getTile(int x, int y) const;

        void updateTeamName(const std::string &teamName);
        const std::vector<std::string> getTeamNames() const;

        void addPlayer(zappy::structs::Player player);
        void updatePlayerPosition(int playerNumber, int x, int y);
        void updatePlayerLevel(int playerNumber, int level);
        void updatePlayerInventory(int playerNumber, zappy::structs::Inventory inventory);
        std::vector<zappy::structs::Player> getPlayers() const;

    private:
        int _mapWidth;
        int _mapHeight;
        int _timeUnit;

        std::vector<zappy::structs::Tile> _tiles;
        std::vector<std::string> _teamNames;
        std::vector<zappy::structs::Player> _players;
};

#endif /* !GAMEINFOS_HPP_ */
