/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GUI
*/

#ifndef GUI_HPP_
#define GUI_HPP_

#include <memory>
#include "../Game/GameInfos.hpp"
#include "Map.hpp"
#include "HUD/HUD.hpp"
#include "../Audio/IAudio.hpp"
#include "../Utils/Constants.hpp"
#include "Camera/CameraManager.hpp"
#include "../IDisplay.hpp"
#include "../DLLoader/DLLoader.hpp"

class GUI {
    public:
        explicit GUI(std::shared_ptr<GameInfos> gameInfos);
        ~GUI();

        void run();

        int getWindowWidth() const;
        int getWindowHeight() const;
        void setWindowWidth(int width);
        void setWindowHeight(int height);

        void switchCameraMode(zappy::gui::CameraMode mode);
        void switchCameraModeNext();
        void setPlayerToFollow(int playerId);
        int getPlayerToFollow() const;
        bool selectFirstAvailablePlayer();
        void switchToNextPlayer();
        void switchToPreviousPlayer();

    private:
        void updateCamera();
        virtual void update();
        virtual void draw();
        virtual bool isRunning();
        bool playerExists(int playerId) const;

        void initModels();

        bool _isRunning;
        DLLoader<std::shared_ptr<IDisplay>> _dlLoader;
        std::shared_ptr<IDisplay> _display;
        std::shared_ptr<GameInfos> _gameInfos;
        std::unique_ptr<Map> _map;
        std::unique_ptr<HUD> _hud;
        std::shared_ptr<IAudio> _audio;
        std::unique_ptr<CameraManager> _cameraManager;

        int _windowWidth;
        int _windowHeight;

        zappy::gui::CameraMode _cameraMode;
};

#endif /* !GUI_HPP_ */
