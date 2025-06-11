/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** entryPoint
*/

#include "../IDisplay.hpp"
#include "Raylib.hpp"
#include <memory>

extern "C" {
    __attribute__((visibility("default")))
    std::shared_ptr<IDisplay> create() {
        return std::make_shared<Raylib>();
    }
}
