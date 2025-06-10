/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** RaylibInput
*/

#include "RayLib.hpp"


bool RayLib::isMouseButtonDown(int button) const
{
    return IsMouseButtonDown(button);
}

bool RayLib::isMouseButtonPressed(int button) const
{
    return IsMouseButtonPressed(button);
}

bool RayLib::isMouseButtonReleased(int button) const
{
    return IsMouseButtonReleased(button);
}

bool RayLib::isKeyDown(int key) const
{
    return IsKeyDown(key);
}

bool RayLib::isKeyPressed(int key) const
{
    return IsKeyPressed(key);
}

bool RayLib::isKeyReleased(int key) const
{
    return IsKeyReleased(key);
}

Vector2 RayLib::getMousePosition() const
{
    return GetMousePosition();
}

void RayLib::setMousePosition(int x, int y)
{
    SetMousePosition(x, y);
}

void RayLib::disableCursor()
{
    if (!_isCursorLocked) {
        DisableCursor();
        _isCursorLocked = true;
    }
}

void RayLib::enableCursor()
{
    if (_isCursorLocked) {
        EnableCursor();
        _isCursorLocked = false;
    }
}

int RayLib::getScreenWidth() const
{
    return GetScreenWidth();
}

int RayLib::getScreenHeight() const
{
    return GetScreenHeight();
}

Vector2 RayLib::getMouseDelta()
{
    if (_isCursorLocked) {
        return GetMouseDelta();
    } else {
        Vector2 currentMousePosition = GetMousePosition();
        Vector2 delta = { currentMousePosition.x - _previousMousePosition.x,
            currentMousePosition.y - _previousMousePosition.y };

        this->_previousMousePosition = currentMousePosition;
        return delta;
    }
}

float RayLib::getMouseWheelMove() const
{
    return GetMouseWheelMove();
}
