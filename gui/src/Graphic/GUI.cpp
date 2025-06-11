/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GUI
*/

#include <cmath>
#include <memory>
#include <iostream>
#include <algorithm>
#include "GUI.hpp"
#include "../Audio/Audio.hpp"
#include "../Utils/Constants.hpp"
#include "../Utils/GamepadConstants.hpp"

GUI::GUI(std::shared_ptr<GameInfos> gameInfos) : _isRunning(false),
    _gameInfos(gameInfos)
{
    _raylib = std::make_shared<RayLib>();

    _cameraMode = zappy::gui::CameraMode::FREE;
    _windowWidth = _raylib->getMonitorWidth(0);
    _windowHeight = _raylib->getMonitorHeight(0);

    _raylib->initWindow(_windowWidth, _windowHeight, zappy::gui::WINDOW_TITLE);
    _raylib->initCamera();
    _isRunning = _raylib->isWindowReady();
    _raylib->setTargetFPS(zappy::gui::FPS);
    _audio = std::make_shared<Audio>();
    _map = std::make_unique<Map>(_gameInfos, _raylib);
    _hud = std::make_unique<HUD>(_raylib, _gameInfos, _audio,
        [this]() {
            this->switchCameraMode(zappy::gui::CameraMode::FREE);
        });

    _cameraManager = std::make_unique<CameraManager>(_raylib);
    _cameraManager->setGameInfos(_gameInfos);
    _cameraManager->setMapInstance(std::shared_ptr<Map>(_map.get(), [](Map*){}));
    const auto& mapSize = _gameInfos->getMapSize();

    Vector3 mapCenter = {
        static_cast<float>(mapSize.first - 1) / 2.0f, 0.0f,
        static_cast<float>(mapSize.second - 1) / 2.0f
    };
    _cameraManager->setMapCenter(mapCenter);
    _cameraManager->setMapSize(mapSize.first, mapSize.second);

    initModels();
}

GUI::~GUI()
{
    _raylib->closeWindow();
}

void GUI::run()
{
    if (!_isRunning)
        return;

    while (!_raylib->windowShouldClose()) {
        update();
        draw();
    }
}

void GUI::updateCamera()
{
    _cameraManager->updateCamera(_cameraMode);
}

void GUI::update()
{
    if (_raylib->isKeyReleased(KEY_TAB) ||
        _raylib->isGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_SHOULDER))
        switchCameraModeNext();

    if (_cameraMode == zappy::gui::CameraMode::PLAYER) {
        if (_raylib->isKeyReleased(KEY_UP) ||
            _raylib->isGamepadButtonReleased(0, GAMEPAD_BUTTON_UP))
            switchToNextPlayer();
        if (_raylib->isKeyReleased(KEY_DOWN) ||
            _raylib->isGamepadButtonReleased(0, GAMEPAD_BUTTON_DOWN))
            switchToPreviousPlayer();
    }

    updateCamera();
    _hud->updateTeamPlayersDisplay(_gameInfos);
    _hud->updatePlayerInventoryDisplay(_cameraManager->getPlayerId(), _cameraMode);
    _hud->update();
}

void GUI::draw()
{
    if (!_isRunning || _raylib->windowShouldClose())
        return;

    _raylib->beginDrawing();
    _raylib->clearBackground(RAYWHITE);

    _raylib->begin3DMode();
    _map->draw();
    _raylib->end3DMode();

    _hud->draw();

    _raylib->endDrawing();
}

int GUI::getWindowWidth() const
{
    return _windowWidth;
}
int GUI::getWindowHeight() const
{
    return _windowHeight;
}
void GUI::setWindowWidth(int width)
{
    if (width <= 0 || width > _raylib->getMonitorWidth(0))
        return;

    _windowWidth = width;
    _raylib->initWindow(_windowWidth, _windowHeight, zappy::gui::WINDOW_TITLE);
    _hud->handleResize(_raylib->getScreenWidth(), _raylib->getScreenHeight(),
        _windowWidth, _windowHeight);
}

void GUI::setWindowHeight(int height)
{
    if (height <= 0 || height > _raylib->getMonitorHeight(0))
        return;

    _windowHeight = height;
    _raylib->initWindow(_windowWidth, _windowHeight, zappy::gui::WINDOW_TITLE);
    _hud->handleResize(_raylib->getScreenWidth(), _raylib->getScreenHeight(),
        _windowWidth, _windowHeight);
}

void GUI::switchCameraMode(zappy::gui::CameraMode mode)
{
    if (mode == zappy::gui::CameraMode::TARGETED &&
        _cameraMode != zappy::gui::CameraMode::TARGETED) {
        const auto& mapSize = _gameInfos->getMapSize();

        Vector3 mapCenter = {
            static_cast<float>(mapSize.first - 1) / 2.0f, 0.0f,
            static_cast<float>(mapSize.second - 1) / 2.0f
        };

        _cameraManager->setMapCenter(mapCenter);
        _cameraManager->initTargetPositionFromCurrentCamera();
    }

    if (mode == zappy::gui::CameraMode::PLAYER) {
        if (_cameraManager->getPlayerId() < 0 ||
            !playerExists(_cameraManager->getPlayerId())) {
            if (!selectFirstAvailablePlayer()) {
                mode = static_cast<zappy::gui::CameraMode>(
                    (static_cast<int>(mode) + 1) %
                        static_cast<int>(zappy::gui::CameraMode::NB_MODES));
            }
        }
    }

    if (mode == zappy::gui::CameraMode::PLAYER && _cameraMode !=
        zappy::gui::CameraMode::PLAYER) {
        _hud->initPlayerInventoryDisplay(_cameraManager->getPlayerId());
    } else if (mode != zappy::gui::CameraMode::PLAYER && _cameraMode ==
        zappy::gui::CameraMode::PLAYER) {
        _hud->clearPlayerInventoryElements();
    }

    _cameraMode = mode;
}

void GUI::switchCameraModeNext()
{
    zappy::gui::CameraMode newMode = static_cast<zappy::gui::CameraMode>(
        (static_cast<int>(_cameraMode) + 1) %
            static_cast<int>(zappy::gui::CameraMode::NB_MODES));

    switchCameraMode(newMode);
}

void GUI::setPlayerToFollow(int playerId)
{
    _cameraManager->setPlayerId(playerId);
    if (_cameraMode == zappy::gui::CameraMode::PLAYER) {
        _hud->initPlayerInventoryDisplay(playerId);
    }
}

int GUI::getPlayerToFollow() const
{
    return _cameraManager->getPlayerId();
}

bool GUI::selectFirstAvailablePlayer()
{
    const auto& players = _gameInfos->getPlayers();

    if (players.empty())
        return false;

    int firstPlayerId = players.front().number;
    _cameraManager->setPlayerId(firstPlayerId);
    return true;
}

bool GUI::playerExists(int playerId) const
{
    const auto& players = _gameInfos->getPlayers();

    return std::any_of(players.begin(), players.end(),
        [playerId](const zappy::structs::Player& player) {
            return player.number == playerId;
        });
}

void GUI::switchToNextPlayer()
{
    const auto& players = _gameInfos->getPlayers();

    if (players.empty())
        return;

    int currentPlayerId = _cameraManager->getPlayerId();
    bool foundCurrent = false;
    int nextPlayerId = -1;

    for (const auto& player : players) {
        if (foundCurrent) {
            nextPlayerId = player.number;
            break;
        }
        if (player.number == currentPlayerId) {
            foundCurrent = true;
        }
    }

    if (nextPlayerId == -1)
        nextPlayerId = players.front().number;
    _cameraManager->setPlayerId(nextPlayerId);
}

void GUI::switchToPreviousPlayer()
{
    const auto& players = _gameInfos->getPlayers();

    if (players.empty())
        return;

    int currentPlayerId = _cameraManager->getPlayerId();
    int prevPlayerId = -1;

    for (const auto& player : players) {
        if (player.number == currentPlayerId) {
            break;
        }
        prevPlayerId = player.number;
    }

    if (prevPlayerId == -1)
        prevPlayerId = players.back().number;
    _cameraManager->setPlayerId(prevPlayerId);
}

void GUI::initModels()
{
    if (!_raylib->loadModel("player", "gui/assets/models/fallguys.glb", {0.0f, 0.0f, 475.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load player model."
                  << colors::RESET << std::endl;
}
