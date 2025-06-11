/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Button
*/

#include "Button.hpp"
#include <memory>
#include <string>

Button::Button(
    std::shared_ptr<IDisplay> display,
    std::shared_ptr<IAudio> audio,
    float x, float y,
    float width, float height,
    const std::string& text,
    std::function<void()> callback
) : AUIElement(display, x, y, width, height),
    _text(text),
    _callback(callback),
    _normalColor({230, 230, 230, 255}),
    _hoverColor({200, 200, 200, 255}),
    _pressedColor({130, 130, 130, 255}),
    _textColor({30, 30, 30, 255}),
    _isHovered(false),
    _isPressed(false),
    _display(display),
    _audio(audio)
{
}

void Button::draw()
{
    if (!_visible)
        return;

    Color currentColor = _normalColor;
    if (_isPressed)
        currentColor = _pressedColor;
    else if (_isHovered)
        currentColor = _hoverColor;

    this->_display->drawRectangleRec(_bounds, currentColor);

    float fontSize = _bounds.height * 0.5f;
    float textWidth = this->_display->measureText(_text, fontSize);
    float textX = _bounds.x + (_bounds.width - textWidth) * 0.5f;
    float textY = _bounds.y + (_bounds.height - fontSize) * 0.5f;

    this->_display->drawText(_text, textX, textY, fontSize, _textColor);
}

void Button::update()
{
    if (!_visible)
        return;

    Vector2 mousePoint = this->_display->getMousePosition();
    _isHovered = contains(mousePoint.x, mousePoint.y);

    bool wasPressed = _isPressed;
    _isPressed = _isHovered && this->_display->isMouseButtonDown(MOUSE_LEFT_BUTTON);

    if (wasPressed && !_isPressed && _isHovered && _callback) {
        _audio->playSound("click", 100.0f);
        _callback();
    }
}

void Button::setText(const std::string& text)
{
    _text = text;
}

std::string Button::getText() const
{
    return _text;
}

void Button::setCallback(std::function<void()> callback)
{
    _callback = callback;
}

void Button::setColors(
    Color normal,
    Color hover,
    Color pressed,
    Color textColor
)
{
    _normalColor = normal;
    _hoverColor = hover;
    _pressedColor = pressed;
    _textColor = textColor;
}

void Button::setSize(float width, float height)
{
    AUIElement::setSize(width, height);
}
