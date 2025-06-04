/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GUI
*/

#include <cmath>
#include <memory>
#include <iostream>
#include "GUI.hpp"
#include "../Utils/Constants.hpp"

GUI::GUI(std::shared_ptr<GameInfos> gameInfos) : _isRunning(false),
    _gameInfos(gameInfos)
{
    _raylib = std::make_shared<RayLib>();

    _windowWidth = GetMonitorWidth(0);
    _windowHeight = GetMonitorHeight(0);

    _raylib->initWindow(_windowWidth, _windowHeight, WINDOW_TITLE);
    _raylib->initCamera();
    _isRunning = _raylib->isWindowReady();
    _map = std::make_unique<Map>(_gameInfos, _raylib);
    _hud = std::make_unique<HUD>(_raylib);
    _hud->initDefaultLayout(250.0f, 250.0f);
    _hud->initExitButton();
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
    _raylib->updateCameraCustom();
}

void GUI::update()
{
    updateCamera();
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
    if (width <= 0 || width > GetMonitorWidth(0))
        return;

    _windowWidth = width;
    _raylib->initWindow(_windowWidth, _windowHeight, WINDOW_TITLE);
    _hud->handleResize(GetScreenWidth(), GetScreenHeight(), _windowWidth, _windowHeight);
}

void GUI::setWindowHeight(int height)
{
    if (height <= 0 || height > GetMonitorHeight(0))
        return;

    _windowHeight = height;
    _raylib->initWindow(_windowWidth, _windowHeight, WINDOW_TITLE);
    _hud->handleResize(GetScreenWidth(), GetScreenHeight(), _windowWidth, _windowHeight);
}
