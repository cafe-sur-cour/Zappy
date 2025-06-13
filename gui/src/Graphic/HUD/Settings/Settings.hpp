/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Settings
*/

#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_
#include <memory>
#include "../Containers/Containers.hpp"
#include "../../../IDisplay.hpp"
#include  "../../../Audio/IAudio.hpp"
#include "Graphic/Camera/CameraManager.hpp"

class Settings {
    private:
        std::shared_ptr<IDisplay> _display;
        std::shared_ptr<IAudio> _audio;
        std::shared_ptr<CameraManager> _camera;
        float _sfxLevel = 25.f;
        float _musicLevel = 25.f;
        float _cameraMovingSpeed;
        float _cameraRotaSpeed;
        float _cameraZoomSpeed;
        std::shared_ptr<Containers> _settingsContainer;
        bool _visible;

    public:
        bool isVisible() const;

        void show();

        void hide();

        void update();

        void draw();

        void handleResize(int oldWidth, int oldHeight, int newWidth, int newHeight);

        Settings(
            std::shared_ptr<IDisplay> display,
            std::shared_ptr<IAudio> audio,
            std::shared_ptr<CameraManager> camera
        );
        ~Settings();
};

#endif /* !SETTINGS_HPP_ */
