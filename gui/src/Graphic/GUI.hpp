/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GUI
*/

#ifndef GUI_HPP_
#define GUI_HPP_

#include "RayLib/RayLib.hpp"
#include "../Game/GameInfos.hpp"
#include "Map.hpp"
#include "HUD/HUD.hpp"

class GUI {
    public:
        GUI(std::shared_ptr<GameInfos> gameInfos);
        ~GUI();

        void run();

    private:
        void updateCamera();

        bool _isRunning;
        std::shared_ptr<RayLib> _raylib;
        std::shared_ptr<GameInfos> _gameInfos;
        std::unique_ptr<Map> _map;
        std::unique_ptr<HUD> _hud;
};

#endif /* !GUI_HPP_ */
