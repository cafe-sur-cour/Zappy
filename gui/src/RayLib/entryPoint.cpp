/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** entryPoint
*/

#include <memory>
#include "../IDisplay.hpp"
#include "Raylib.hpp"

extern "C" {
    std::shared_ptr<IDisplay> create() {
        return std::make_shared<Raylib>();
    }
}
