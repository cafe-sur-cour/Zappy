/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Containers
*/

#include "Containers.hpp"
#include <string>
#include <memory>

Containers::Containers(
    std::shared_ptr<RayLib> raylib,
    float x,
    float y,
    float width,
    float height,
    Color backgroundColor)
    : AContainers(raylib, x, y, width, height),
      _raylib(raylib),
      _hasBackgroundTexture(false),
      _elements()
{
    _backgroundColor = backgroundColor;
}

Containers::~Containers()
{
    if (_hasBackgroundTexture)
        _raylib->unloadTexture(_backgroundTexture);
}

void Containers::draw()
{
    if (!_visible)
        return;

    if (_hasBackground) {
        if (_hasBackgroundTexture) {
            _raylib->drawTextureRec(_backgroundTexture,
                (Rectangle){0, 0, _bounds.width, _bounds.height},
                (Vector2){_bounds.x, _bounds.y},
                WHITE);
        } else {
            _raylib->drawRectangleRec(_bounds, _backgroundColor);
        }
    }

    for (auto& pair : _elements) {
        pair.second->draw();
    }
}

void Containers::update()
{
    if (!_visible)
        return;

    for (auto& pair : _elements) {
        pair.second->update();
    }
}

void Containers::setBackgroundColor(Color color)
{
    _backgroundColor = color;
}

void Containers::setHasBackground(bool hasBackground)
{
    _hasBackground = hasBackground;
}

void Containers::setBackgroundTexture(Texture2D texture)
{
    if (_hasBackgroundTexture)
        _raylib->unloadTexture(_backgroundTexture);

    _backgroundTexture = texture;
    _hasBackgroundTexture = true;
}

bool Containers::hasBackgroundTexture() const
{
    return _hasBackgroundTexture;
}

bool Containers::addElement(const std::string& id, std::shared_ptr<IUIElement> element)
{
    if (_elements.find(id) != _elements.end())
        return false;

    Vector2 elemPos = {element->getBounds().x, element->getBounds().y};
    element->setPosition(_bounds.x + elemPos.x, _bounds.y + elemPos.y);

    _elements[id] = element;
    return true;
}

std::shared_ptr<IUIElement> Containers::getElement(const std::string& id) const
{
    auto it = _elements.find(id);
    if (it != _elements.end())
        return it->second;

    return nullptr;
}

bool Containers::removeElement(const std::string& id)
{
    auto it = _elements.find(id);
    if (it != _elements.end()) {
        _elements.erase(it);
        return true;
    }
    return false;
}

std::shared_ptr<Button> Containers::addButton(
    const std::string& id,
    float x, float y,
    float width, float height,
    const std::string& text,
    std::function<void()> callback
)
{
    auto button = std::make_shared<Button>(_raylib, x, y, width, height, text, callback);

    if (addElement(id, button))
        return button;

    return nullptr;
}

std::shared_ptr<Button> Containers::addButton(
    const std::string& id,
    float x, float y,
    float width, float height,
    const std::string& text,
    std::function<void()> callback,
    Color normalColor,
    Color hoverColor,
    Color pressedColor,
    Color textColor
)
{
    auto button = std::make_shared<Button>(_raylib, x, y, width, height, text, callback);
    button->setColors(normalColor, hoverColor, pressedColor, textColor);

    if (addElement(id, button))
        return button;

    return nullptr;
}

std::shared_ptr<Text> Containers::addText(
    const std::string& id,
    float x, float y,
    const std::string& text,
    float fontSize,
    Color color
)
{
    auto textElement = std::make_shared<Text>(_raylib, x, y, text, fontSize, color);

    if (addElement(id, textElement))
        return textElement;

    return nullptr;
}

std::shared_ptr<ScrollBar> Containers::addScrollBar(
    const std::string& id,
    float x, float y,
    float length, float thickness,
    ScrollBarOrientation orientation,
    std::function<void(float)> onValueChanged
)
{
    auto scrollBar = std::make_shared<ScrollBar>(
        _raylib,
        x,
        y,
        length,
        thickness,
        orientation,
        onValueChanged);

    if (addElement(id, scrollBar))
        return scrollBar;

    return nullptr;
}

void Containers::clearElements()
{
    _elements.clear();
}

void Containers::handleResize(int oldWidth, int oldHeight, int newWidth, int newHeight)
{
    float scaleX = static_cast<float>(newWidth) / static_cast<float>(oldWidth);
    float scaleY = static_cast<float>(newHeight) / static_cast<float>(oldHeight);

    _bounds.x *= scaleX;
    _bounds.y *= scaleY;
    _bounds.width *= scaleX;
    _bounds.height *= scaleY;

    for (auto& pair : _elements) {
        Rectangle elemBounds = pair.second->getBounds();

        float relX = (elemBounds.x - _bounds.x) * scaleX;
        float relY = (elemBounds.y - _bounds.y) * scaleY;

        pair.second->setPosition(_bounds.x + relX, _bounds.y + relY);
    }
}
