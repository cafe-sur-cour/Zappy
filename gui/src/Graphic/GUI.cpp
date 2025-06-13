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
#include <string>
#include "GUI.hpp"
#include "../Exceptions/Exceptions.hpp"
#include "../DLLoader/LoaderType.hpp"
#include "../Audio/Audio.hpp"
#include "../Utils/Constants.hpp"
#include "../Utils/GamepadConstants.hpp"

GUI::GUI(std::shared_ptr<GameInfos> gameInfos, const std::string &lib)
    : _isRunning(false), _gameInfos(gameInfos), _backgroundLoaded(false), _skyboxLoaded(false)
{
    this->_dlLoader = DLLoader<std::shared_ptr<IDisplay>>();
    if (lib.empty())
        throw Exceptions::ModuleError(lib + ": Lib input empty");
    this->_dlLoader.Open(lib.c_str());
    if (!this->_dlLoader.getHandler())
        throw Exceptions::ModuleError(lib + ": Failed to open library: " +
            std::string(dlerror()));
    using CreateFunc = std::shared_ptr<IDisplay>(*)();
    using GetTypeFunc = ModuleType_t(*)();

    GetTypeFunc getTypeFunc = reinterpret_cast<GetTypeFunc>(this->_dlLoader.Symbol("getType"));
    if (!getTypeFunc || getTypeFunc() != ModuleType_t::DISPLAY_MODULE)
        throw Exceptions::ModuleError(lib + ": Not a valid module");

    CreateFunc createFunc = reinterpret_cast<CreateFunc>(this->_dlLoader.Symbol("create"));
    if (!createFunc)
        throw Exceptions::ModuleError(lib + ": No create symbole found in the shared lib");
    this->_display = createFunc();

    std::cout << lib + ": Module GUI Found" << std::endl;
    _cameraMode = zappy::gui::CameraMode::FREE;
    auto monitorSize = this->_display->getMonitorSize();
    this->_windowWidth = monitorSize.x;
    this->_windowHeight = monitorSize.y;

    this->_display->initWindow(this->_windowWidth, this->_windowHeight,
        zappy::gui::WINDOW_TITLE);
    this->_display->initCamera();
    _isRunning = this->_display->isWindowReady();
    this->_display->setTargetFPS(zappy::gui::FPS);
    this->_audio = std::make_shared<Audio>();
    this->_map = std::make_unique<Map>(_gameInfos, this->_display);
    this->_hud = std::make_unique<HUD>(this->_display, _gameInfos, _audio,
        [this]() {
            this->switchCameraMode(zappy::gui::CameraMode::FREE);
        });

    _cameraManager = std::make_unique<CameraManager>(this->_display);
    _cameraManager->setGameInfos(_gameInfos);
    _cameraManager->setMapInstance(std::shared_ptr<Map>(_map.get(), [](Map*){}));
    const auto& mapSize = _gameInfos->getMapSize();

    Vector3f mapCenter = {
        static_cast<float>(mapSize.first - 1) / 2.0f, 0.0f,
        static_cast<float>(mapSize.second - 1) / 2.0f
    };
    _cameraManager->setMapCenter(mapCenter);
    _cameraManager->setMapSize(mapSize.first, mapSize.second);

    initModels();
}

GUI::~GUI()
{
    this->_display->closeWindow();
}

void GUI::run()
{
    if (!_isRunning) {
        return;
    }

    while (_isRunning) {
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
    this->_isRunning = this->_display->isOpen();
    if (this->_display->isKeyReleased(this->_display->getKeyId(TAB)) ||
        this->_display->isGamepadButtonReleased(this->_display->getKeyId(GM_PD_LEFT_SHOULDER)))
        switchCameraModeNext();

    if (_cameraMode == zappy::gui::CameraMode::PLAYER) {
        if (this->_display->isKeyReleased(this->_display->getKeyId(UP)) ||
            this->_display->isGamepadButtonReleased(this->_display->getKeyId(GM_PD_UP)))
            switchToNextPlayer();
        if (this->_display->isKeyReleased(this->_display->getKeyId(DOWN)) ||
            this->_display->isGamepadButtonReleased(this->_display->getKeyId(GM_PD_DOWN)))
            switchToPreviousPlayer();
    }

    updateCamera();
    this->_hud->updateTeamPlayersDisplay(this->_gameInfos);
    this->_hud->updatePlayerInventoryDisplay(this->_cameraManager->getPlayerId(),
        this->_cameraMode);
    this->_hud->updateHelpInformationHUD(this->_cameraMode);
    this->_hud->update();
}

void GUI::refresh()
{
    update();
}

bool GUI::isRunning()
{
    return this->_isRunning;
}

void GUI::draw()
{
    if (!this->_display->isOpen())
        return;

    this->_display->beginDrawing();

    this->_display->clearBackground(Color32{255, 255, 255, 255});

    this->_display->drawSimpleSkybox();

    this->_display->begin3DMode();

    if (_skyboxLoaded) {
        this->_display->drawSkybox("skybox");
    }

    _map->draw();

    this->_display->end3DMode();

    _hud->draw();

    this->_display->endDrawing();
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
    if (width <= 0 || width > this->_display->getMonitorSize().x)
        return;

    _windowWidth = width;
    auto screenSize = this->_display->getScreenSize();
    this->_display->initWindow(_windowWidth, _windowHeight, zappy::gui::WINDOW_TITLE);
    _hud->handleResize(screenSize.x, screenSize.y,
        _windowWidth, _windowHeight);
}

void GUI::setWindowHeight(int height)
{
    if (height <= 0 || height > this->_display->getMonitorSize().y)
        return;

    _windowHeight = height;
    auto screenSize = this->_display->getScreenSize();
    this->_display->initWindow(_windowWidth, _windowHeight, zappy::gui::WINDOW_TITLE);
    _hud->handleResize(screenSize.x, screenSize.y,
        _windowWidth, _windowHeight);
}

void GUI::switchCameraMode(zappy::gui::CameraMode mode)
{
    if (mode == zappy::gui::CameraMode::TARGETED &&
        _cameraMode != zappy::gui::CameraMode::TARGETED) {
        const auto& mapSize = _gameInfos->getMapSize();

        Vector3f mapCenter = {
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
    if (this->_display->loadSkybox("skybox", "gui/assets/sprite/skybox/skybox.png")) {
        std::cout << colors::T_GREEN << "[INFO] Successfully loaded skybox texture."
                << colors::RESET << std::endl;
        _skyboxLoaded = true;
    } else {
        std::cout << colors::T_RED << "[ERROR] Failed to load skybox texture."
                << colors::RESET << std::endl;
        if (!_backgroundLoaded) {
            if (!this->_display->loadTexture("background", "gui/assets/sprite/background.png"))
                std::cout << colors::T_RED << "[ERROR] Failed to load background texture."
                        << colors::RESET << std::endl;
            else
                _backgroundLoaded = true;
        }
    }

    if (!this->_display->loadModel("player", "gui/assets/models/fall_guy.glb",
        {0.0f, -75.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load player model."
                  << colors::RESET << std::endl;

    if (!this->_display->loadModel("platform", "gui/assets/models/tile.glb",
        {0.0f, 0.25f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load platform model."
                  << colors::RESET << std::endl;

    if (!this->_display->loadModel("food", "gui/assets/models/apple.glb", {0.0f, 0.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load food model."
                  << colors::RESET << std::endl;

    if (!this->_display->loadModel("linemate", "gui/assets/models/soccerball.glb",
        {0.0f, 0.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load linemate model."
                  << colors::RESET << std::endl;

    if (!this->_display->loadModel("deraumere", "gui/assets/models/beachball.glb",
        {0.0f, 0.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load deraumere model."
                  << colors::RESET << std::endl;

    if (!this->_display->loadModel("sibur", "gui/assets/models/basketball.glb",
        {0.0f, 0.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load sibur model."
                  << colors::RESET << std::endl;

    if (!this->_display->loadModel("mendiane", "gui/assets/models/bowlingball.glb",
        {0.0f, 0.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load mendiane model."
                  << colors::RESET << std::endl;

    if (!this->_display->loadModel("phiras", "gui/assets/models/eightball.glb",
        {0.0f, 0.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load phiras model."
                  << colors::RESET << std::endl;

    if (!this->_display->loadModel("thystame", "gui/assets/models/tennisball.glb",
        {0.0f, 0.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load thystame model."
                  << colors::RESET << std::endl;

    if (!this->_display->loadModel("egg", "gui/assets/models/egg.glb", {0.0f, 0.0f, 0.0f}))
        std::cout << colors::T_RED << "[ERROR] Failed to load egg model."
                  << colors::RESET << std::endl;
}
