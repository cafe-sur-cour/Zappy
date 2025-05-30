/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GameInfos
*/

#ifndef GAMEINFOS_HPP_
#define GAMEINFOS_HPP_

#include <utility>

class GameInfos {
    public:
        GameInfos();
        ~GameInfos();

        void setMapSize(int width, int height);
        std::pair<int, int> getMapSize() const;

    private:
        int _mapWidth;
        int _mapHeight;
};

#endif /* !GAMEINFOS_HPP_ */
