/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** ScrollBar
*/

#include "ScrollBar.hpp"
#include <algorithm>
#include <memory>

ScrollBar::ScrollBar(
    std::shared_ptr<RayLib> raylib,
    float x, float y,
    float length, float thickness,
    ScrollBarOrientation orientation,
    std::function<void(float)> onValueChanged
) : AUIElement(
      raylib,
      x, y,
      orientation == ScrollBarOrientation::VERTICAL ? thickness : length,
      orientation == ScrollBarOrientation::VERTICAL ? length : thickness
    ),
    _raylib(raylib),
    _value(0.0f),
    _handleSize(0.2f),
    _isDragging(false),
    _orientation(orientation),
    _onValueChanged(onValueChanged),
    _backgroundColor({150, 150, 150, 255}),
    _handleColor({200, 200, 200, 255}),
    _handleHoverColor({220, 220, 220, 255}),
    _handlePressedColor({170, 170, 170, 255}),
    _isHandleHovered(false)
{
}

void ScrollBar::draw()
{
    if (!_visible)
        return;

    _raylib->drawRectangleRec(_bounds, _backgroundColor);

    Rectangle handleBounds = getHandleBounds();

    Color currentHandleColor = _handleColor;
    if (_isDragging)
        currentHandleColor = _handlePressedColor;
    else if (_isHandleHovered)
        currentHandleColor = _handleHoverColor;

    _raylib->drawRectangleRec(handleBounds, currentHandleColor);
}

void ScrollBar::update()
{
    if (!_visible)
        return;

    Vector2 mousePoint = _raylib->getMousePosition();
    Rectangle handleBounds = getHandleBounds();

    _isHandleHovered = _raylib->checkCollisionPointRec(mousePoint, handleBounds);

    if (_raylib->isMouseButtonPressed(MOUSE_LEFT_BUTTON) && _isHandleHovered)
        _isDragging = true;

    if (_raylib->isMouseButtonReleased(MOUSE_LEFT_BUTTON))
        _isDragging = false;

    if (_isDragging) {
        _value = calculateValueFromMousePosition(mousePoint);
        if (_onValueChanged)
            _onValueChanged(_value);
    }
}

void ScrollBar::setValue(float value)
{
    float oldValue = _value;
    _value = std::max(0.0f, std::min(1.0f, value));

    if (oldValue != _value && _onValueChanged)
        _onValueChanged(_value);
}

float ScrollBar::getValue() const
{
    return _value;
}

void ScrollBar::setColors(
    Color background,
    Color handle,
    Color handleHover,
    Color handlePressed
)
{
    _backgroundColor = background;
    _handleColor = handle;
    _handleHoverColor = handleHover;
    _handlePressedColor = handlePressed;
}

void ScrollBar::setOnValueChanged(std::function<void(float)> onValueChanged)
{
    _onValueChanged = onValueChanged;
}

Rectangle ScrollBar::getHandleBounds() const
{
    Rectangle handle = _bounds;

    if (_orientation == ScrollBarOrientation::VERTICAL) {
        handle.height = _bounds.height * _handleSize;
        handle.y = _bounds.y + (_bounds.height - handle.height) * _value;
    } else {
        handle.width = _bounds.width * _handleSize;
        handle.x = _bounds.x + (_bounds.width - handle.width) * _value;
    }

    return handle;
}

float ScrollBar::calculateValueFromMousePosition(Vector2 mousePosition) const
{
    float position, size, mousePos;

    if (_orientation == ScrollBarOrientation::VERTICAL) {
        position = _bounds.y;
        size = _bounds.height * (1.0f - _handleSize);
        mousePos = mousePosition.y - position;
    } else {
        position = _bounds.x;
        size = _bounds.width * (1.0f - _handleSize);
        mousePos = mousePosition.x - position;
    }

    float value = mousePos / size;
    return std::max(0.0f, std::min(1.0f, value));
}

void ScrollBar::setSize(float width, float height)
{
    if (_orientation == ScrollBarOrientation::VERTICAL) {
        _bounds.width = width;
        _bounds.height = height;
    } else {
        _bounds.width = height;
        _bounds.height = width;
    }
}
