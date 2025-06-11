/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** entryPoint
*/

#include "../IDisplay.hpp"
#include "Raylib.hpp"

extern "C" {
    IDisplay *create() {
        return new Raylib();
    }
}
