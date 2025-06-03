/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Text
*/

#include "Text.hpp"

Text::Text(
    std::shared_ptr<RayLib> raylib,
    float x, float y,
    const std::string& text,
    float fontSize,
    Color color
) : AUIElement(x, y, raylib->measureText(text, fontSize), fontSize),
    _text(text),
    _fontSize(fontSize),
    _color(color),
    _raylib(raylib)
{
    _bounds.width = _raylib->measureText(_text, _fontSize);
    _bounds.height = fontSize;
}

void Text::draw()
{
    if (!_visible)
        return;

    _raylib->drawText(_text, _bounds.x, _bounds.y, _fontSize, _color);
}

void Text::update()
{

}

void Text::setText(const std::string& text)
{
    _text = text;

    _bounds.width = _raylib->measureText(_text, _fontSize);
}

std::string Text::getText() const
{
    return _text;
}

void Text::setFontSize(float fontSize)
{
    _fontSize = fontSize;

    _bounds.width = _raylib->measureText(_text, _fontSize);
    _bounds.height = _fontSize;
}

float Text::getFontSize() const
{
    return _fontSize;
}

void Text::setColor(Color color)
{
    _color = color;
}

Color Text::getColor() const
{
    return _color;
}
