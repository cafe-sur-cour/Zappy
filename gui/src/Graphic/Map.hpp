/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Map
*/

#ifndef MAP_HPP_
#define MAP_HPP_

#include <memory>
#include "../Game/GameInfos.hpp"
#include "RayLib/RayLib.hpp"

class Map {
    public:
        Map(std::shared_ptr<GameInfos> gameInfos, std::shared_ptr<RayLib> raylib);
        ~Map();

        void draw();
        void drawTile(int x, int y, const zappy::structs::Tile &tile);

    private:
        std::shared_ptr<GameInfos> _gameInfos;
        std::shared_ptr<RayLib> _raylib;
};

#endif /* !MAP_HPP_ */
