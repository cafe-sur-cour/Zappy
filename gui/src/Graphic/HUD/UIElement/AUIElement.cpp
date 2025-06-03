/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** AUIElement
*/

#include "AUIElement.hpp"

AUIElement::AUIElement(float x, float y, float width, float height)
    : _bounds({x, y, width, height}), _visible(true)
{

}

void AUIElement::setPosition(float x, float y)
{
    _bounds.x = x;
    _bounds.y = y;
}

Rectangle AUIElement::getBounds() const
{
    return _bounds;
}

bool AUIElement::contains(float x, float y) const
{
    return CheckCollisionPointRec({x, y}, _bounds);
}

void AUIElement::setVisible(bool visible)
{
    _visible = visible;
}

bool AUIElement::isVisible() const
{
    return _visible;
}
