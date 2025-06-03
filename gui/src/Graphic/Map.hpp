/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Map
*/

#ifndef MAP_HPP_
#define MAP_HPP_

#include <memory>
#include <unordered_map>
#include <string>
#include "../Game/GameInfos.hpp"
#include "RayLib/RayLib.hpp"

class Map {
    public:
        Map(std::shared_ptr<GameInfos> gameInfos, std::shared_ptr<RayLib> raylib);
        ~Map();

        void draw();
        void drawTile(int x, int y, const zappy::structs::Tile &tile);
        void drawPlayers(int x, int y);
        Color getTeamColor(const std::string &teamName);

    private:
        std::shared_ptr<GameInfos> _gameInfos;
        std::shared_ptr<RayLib> _raylib;
        std::unordered_map<std::string, Color> _teamColors;

        void drawOrientationArrow(const Vector3 &position, int orientation, float playerHeight);
};

#endif /* !MAP_HPP_ */
