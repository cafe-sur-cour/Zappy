/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** AContainers
*/

#include "AContainers.hpp"

AContainers::AContainers(std::shared_ptr<RayLib> raylib, float x, float y, float width, float height)
    : _raylib(raylib), _bounds({x, y, width, height}), _backgroundColor(RAYWHITE), _visible(true), _hasBackground(true)
{
}

void AContainers::setPosition(float x, float y)
{
    _bounds.x = x;
    _bounds.y = y;
}

void AContainers::setSize(float width, float height)
{
    _bounds.width = width;
    _bounds.height = height;
}

Rectangle AContainers::getBounds() const
{
    return _bounds;
}

bool AContainers::contains(float x, float y) const
{
    return _raylib->checkCollisionPointRec({x, y}, _bounds);
}

void AContainers::setVisible(bool visible)
{
    _visible = visible;
}

bool AContainers::isVisible() const
{
    return _visible;
}
