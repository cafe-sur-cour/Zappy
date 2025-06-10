/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** IGUI.hpp
*/

#ifndef IGUI_HPP
#define IGUI_HPP

class IGUI
{
    virtual bool isRunning() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual ~IGUI() = default;
}

#endif //IGUI_HPP
