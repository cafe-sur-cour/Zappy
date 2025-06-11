/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Raylib
*/

#include <iostream>
#include "Raylib.hpp"
#include "../Utils/GamepadConstants.hpp"


Vector2i Raylib::getScreenSize()
{
    return {this->_raylib->getScreenWidth(), this->_raylib->getScreenHeight()};
}

Vector2i Raylib::getMonitorSize()
{
    return {this->_raylib->getMonitorWidth(0), this->_raylib->getMonitorHeight(0)};
}

void Raylib::initWindow(int width, int height, std::string title)
{
    this->_raylib->initWindow(width, height, title);
}

void Raylib::initCamera()
{
    return this->_raylib->initCamera();
}

bool Raylib::isWindowReady()
{
    return this->_raylib->isWindowReady();
}

void Raylib::setTargetFPS(unsigned int FPS)
{
    return this->_raylib->setTargetFPS(FPS);
}

bool Raylib::isOpen()
{
    return !this->_raylib->windowShouldClose();
}

void Raylib::closeWindow()
{
    return this->_raylib->closeWindow();
}

int Raylib::getKeyId(Key key)
{
    switch (key)
    {
    case TAB:
        return KEY_TAB;
    case UP:
        return KEY_UP;
    case DOWN:
        return KEY_DOWN;
    case RIGHT:
        return KEY_RIGHT;
    case LEFT:
        return KEY_LEFT;
    case GM_PD_LEFT_SHOULDER:
        return GAMEPAD_BUTTON_LEFT_SHOULDER;
    case GM_PD_RIGHT_SHOULDER:
        return GAMEPAD_BUTTON_RIGHT_SHOULDER;
    case GM_PD_LEFT_TRIGGER:
        return GAMEPAD_BUTTON_LEFT_TRIGGER;
    case GM_PD_RIGHT_TRIGGER:
        return GAMEPAD_BUTTON_RIGHT_TRIGGER;
    case GM_PD_UP:
        return GAMEPAD_BUTTON_UP;
    case GM_PD_DOWN:
        return GAMEPAD_BUTTON_DOWN;
    case GM_PD_AXIS_RIGHT_X:
        return GAMEPAD_AXIS_RIGHT_X;
    case GM_PD_AXIS_RIGHT_Y:
        return GAMEPAD_AXIS_RIGHT_Y;
    case MOUSE_RIGHT:
        return MOUSE_RIGHT_BUTTON;
    case MOUSE_LEFT:
        return MOUSE_LEFT_BUTTON;
    default : {
        std::cerr << "NO KEY FOUND" << std::endl;
        return -1;
    }
    }
}

bool Raylib::isKeyReleased(int key)
{
    return this->_raylib->isKeyReleased(key);
}

bool Raylib::isKeyPressed(int key)
{
    return this->_raylib->isKeyPressed(key);
}

bool Raylib::isKeyDown(int key)
{
    return this->_raylib->isKeyDown(key);
}

bool Raylib::isGamepadButtonReleased(int key)
{
    return this->_raylib->isGamepadButtonReleased(0, key);
}

bool Raylib::isGamepadButtonPressed(int key)
{
    return this->_raylib->isGamepadButtonPressed(0, key);
}

bool Raylib::isGamepadButtonDown(int key)
{
    return this->_raylib->isGamepadButtonDown(0, key);
}

bool Raylib::isGamepadAvailable()
{
    return this->_raylib->isGamepadAvailable(0);
}
