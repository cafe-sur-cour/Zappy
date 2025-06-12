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
#include <memory>
#include <mutex>
#include <string>
#include <chrono>

#include "../Utils/Constants.hpp"
#include "../Communication/ICommunication.hpp"

class GameInfos {
    public:
        explicit GameInfos(std::shared_ptr<ICommunication> communication);
        ~GameInfos();

        void setMapSize(int width, int height);
        std::pair<int, int> getMapSize() const;

        void setTimeUnit(int timeUnit, bool sendToServer = false);
        int getTimeUnit() const;

        void updateTile(const zappy::structs::Tile tile);
        const std::vector<zappy::structs::Tile> getTiles() const;
        const zappy::structs::Tile getTile(int x, int y) const;

        void updateTeamName(const std::string &teamName);
        const std::vector<std::string> getTeamNames() const;

        void addPlayer(const zappy::structs::Player player);
        void updatePlayerPosition(int playerNumber, int x, int y);
        void updatePlayerOrientation(int playerNumber, int orientation);
        void updatePlayerLevel(int playerNumber, int level);
        void updatePlayerInventory(int playerNumber,
            const zappy::structs::Inventory inventory);
        void updatePlayerExpulsion(int playerNumber);
        void updatePlayerDeath(int playerNumber);
        void updatePlayerResourceAction(int playerNumber, int resourceId, bool isCollecting);
        void updatePlayerFork(int playerNumber);
        const std::vector<zappy::structs::Player> getPlayers() const;
        const zappy::structs::Player getPlayer(int playerNumber) const;

        void addPlayerBroadcast(int playerNumber, const std::string &message);
        const std::vector<std::pair<int, std::string>> getPlayersBroadcasting();

        void addIncantation(const zappy::structs::Incantation incantation);
        void removeIncantation(int x, int y, int result);
        const std::vector<zappy::structs::Incantation> getIncantations();

        void addEgg(const zappy::structs::Egg egg);
        void updateEggHatched(int eggNumber);
        void updateEggDeath(int eggNumber);
        const std::vector<zappy::structs::Egg> getEggs() const;

        void setGameOver(const std::string &winningTeam);
        std::pair<bool, std::string> isGameOver() const;

    private:
        int _mapWidth;
        int _mapHeight;
        int _timeUnit;

        std::vector<zappy::structs::Tile> _tiles;
        std::vector<std::string> _teamNames;
        std::vector<zappy::structs::Player> _players;
        std::vector<std::pair<int, bool>> _playersExpulsing;
        std::vector<std::tuple<int, std::string, std::chrono::steady_clock::time_point>>
            _playersBroadcasting;
        std::vector<zappy::structs::Incantation> _incantations;
        std::vector<zappy::structs::Egg> _eggs;

        bool _gameOver;
        std::string _winningTeam;

        mutable std::mutex _dataMutex;

        std::shared_ptr<ICommunication> _communication;
};

#endif /* !GAMEINFOS_HPP_ */
