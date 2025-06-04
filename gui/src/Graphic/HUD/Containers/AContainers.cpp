/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** AContainers
*/

#include "AContainers.hpp"
#include <memory>

AContainers::AContainers(
    std::shared_ptr<RayLib> raylib,
    float x,
    float y,
    float width,
    float height)
    : _raylib(raylib),
      _bounds({x, y, width, height}),
      _backgroundColor(RAYWHITE),
      _visible(true),
      _hasBackground(true)
{
    int screenWidth = raylib->getScreenWidth();
    int screenHeight = raylib->getScreenHeight();

    _relativePos.xPercent = (x / screenWidth) * 100.0f;
    _relativePos.yPercent = (y / screenHeight) * 100.0f;
    _relativePos.widthPercent = (width / screenWidth) * 100.0f;
    _relativePos.heightPercent = (height / screenHeight) * 100.0f;
}

void AContainers::setPosition(float x, float y)
{
    _bounds.x = x;
    _bounds.y = y;

    int screenWidth = _raylib->getScreenWidth();
    int screenHeight = _raylib->getScreenHeight();

    _relativePos.xPercent = (x / screenWidth) * 100.0f;
    _relativePos.yPercent = (y / screenHeight) * 100.0f;
}

void AContainers::setSize(float width, float height)
{
    _bounds.width = width;
    _bounds.height = height;

    int screenWidth = _raylib->getScreenWidth();
    int screenHeight = _raylib->getScreenHeight();

    _relativePos.widthPercent = (width / screenWidth) * 100.0f;
    _relativePos.heightPercent = (height / screenHeight) * 100.0f;
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

void AContainers::setRelativePosition(float xPercent, float yPercent, float widthPercent, float heightPercent)
{
    _relativePos.xPercent = xPercent;
    _relativePos.yPercent = yPercent;
    _relativePos.widthPercent = widthPercent;
    _relativePos.heightPercent = heightPercent;

    updatePositionFromRelative();
}

RelativePosition AContainers::getRelativePosition() const
{
    return _relativePos;
}

void AContainers::updatePositionFromRelative()
{
    int screenWidth = _raylib->getScreenWidth();
    int screenHeight = _raylib->getScreenHeight();

    _bounds.x = (screenWidth * _relativePos.xPercent) / 100.0f;
    _bounds.y = (screenHeight * _relativePos.yPercent) / 100.0f;
    _bounds.width = (screenWidth * _relativePos.widthPercent) / 100.0f;
    _bounds.height = (screenHeight * _relativePos.heightPercent) / 100.0f;
}
