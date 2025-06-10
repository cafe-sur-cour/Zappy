/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** RaylibGamepad
*/

#include <iostream>
#include "RayLib.hpp"
#include "../Utils/GamepadConstants.hpp"

bool RayLib::isGamepadAvailable(int gamepad) const
{
    return IsGamepadAvailable(gamepad);
}

bool RayLib::isGamepadButtonPressed(int gamepad, int button) const
{
    if (!isGamepadAvailable(gamepad))
        return false;

    return IsGamepadButtonPressed(gamepad, button);
}

bool RayLib::isGamepadButtonDown(int gamepad, int button) const
{
    if (!isGamepadAvailable(gamepad))
        return false;

    return IsGamepadButtonDown(gamepad, button);
}

bool RayLib::isGamepadButtonReleased(int gamepad, int button) const
{
    if (!isGamepadAvailable(gamepad))
        return false;

    return IsGamepadButtonReleased(gamepad, button);
}

float RayLib::getGamepadAxisMovement(int gamepad, int axis) const
{
    if (!isGamepadAvailable(gamepad))
        return false;

    return GetGamepadAxisMovement(gamepad, axis);
}
