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
    std::shared_ptr<Audio> audio,
    float x,
    float y,
    float width,
    float height,
    Color backgroundColor)
    : AContainers(raylib, x, y, width, height),
      _raylib(raylib),
      _audio(audio),
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

    _raylib->beginScissorMode(
        static_cast<int>(_bounds.x),
        static_cast<int>(_bounds.y),
        static_cast<int>(_bounds.width),
        static_cast<int>(_bounds.height)
    );

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
        if (pair.first.find("scrollbar") != std::string::npos) {
            continue;
        }
        pair.second->draw();
    }

    _raylib->endScissorMode();

    for (auto& pair : _elements) {
        if (pair.first.find("scrollbar") != std::string::npos) {
            pair.second->draw();
        }
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

    Rectangle elemBounds = element->getBounds();

    auto button = std::dynamic_pointer_cast<Button>(element);
    auto text = std::dynamic_pointer_cast<Text>(element);

    float xPercent = (elemBounds.x / _bounds.width) * 100.0f;
    float yPercent = (elemBounds.y / _bounds.height) * 100.0f;
    float widthPercent = (elemBounds.width / _bounds.width) * 100.0f;
    float heightPercent = (elemBounds.height / _bounds.height) * 100.0f;

    if (button) {
        button->setRelativePosition(xPercent, yPercent, widthPercent, heightPercent);
    } else if (text) {
        text->setRelativePosition(xPercent, yPercent, 0.0f, heightPercent);
    }

    element->setPosition(_bounds.x + elemBounds.x, _bounds.y + elemBounds.y);

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
    auto button = std::make_shared<Button>(_raylib, _audio, x, y, width,
        height, text, callback);

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
    auto button = std::make_shared<Button>(_raylib, _audio, x, y, width, height,
        text, callback);
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

void Containers::clearElements()
{
    _elements.clear();
}

void Containers::handleResize(int oldWidth, int oldHeight, int newWidth, int newHeight)
{
    (void)oldWidth;
    (void)oldHeight;
    (void)newWidth;
    (void)newHeight;
    updatePositionFromRelative();

    for (auto& pair : _elements) {
        auto button = std::dynamic_pointer_cast<Button>(pair.second);
        auto text = std::dynamic_pointer_cast<Text>(pair.second);

        UIRelativePosition elemRelPos;
        bool hasRelativePos = false;

        if (button) {
            elemRelPos = button->getRelativePosition();
            hasRelativePos = true;
        } else if (text) {
            elemRelPos = text->getRelativePosition();
            hasRelativePos = true;
        }

        if (hasRelativePos) {
            float elemNewX = _bounds.x + (_bounds.width * elemRelPos.xPercent / 100.0f);
            float elemNewY = _bounds.y + (_bounds.height * elemRelPos.yPercent / 100.0f);
            float elemNewWidth = _bounds.width * elemRelPos.widthPercent / 100.0f;
            float elemNewHeight = _bounds.height * elemRelPos.heightPercent / 100.0f;

            pair.second->setPosition(elemNewX, elemNewY);

            if (button) {
                button->setSize(elemNewWidth, elemNewHeight);
            } else if (text) {
                text->setFontSize(elemNewHeight);
            }
        } else {
            Rectangle elemBounds = pair.second->getBounds();

            float elemRelXPercent = ((elemBounds.x - _bounds.x) / _bounds.width) * 100.0f;
            float elemRelYPercent = ((elemBounds.y - _bounds.y) / _bounds.height) * 100.0f;
            float elemWidthPercent = (elemBounds.width / _bounds.width) * 100.0f;
            float elemHeightPercent = (elemBounds.height / _bounds.height) * 100.0f;

            float elemNewX = _bounds.x + (_bounds.width * elemRelXPercent / 100.0f);
            float elemNewY = _bounds.y + (_bounds.height * elemRelYPercent / 100.0f);
            float elemNewWidth = _bounds.width * elemWidthPercent / 100.0f;
            float elemNewHeight = _bounds.height * elemHeightPercent / 100.0f;

            pair.second->setPosition(elemNewX, elemNewY);

            if (button) {
                button->setSize(elemNewWidth, elemNewHeight);
            } else if (text) {
                text->setFontSize(elemNewHeight);
            }
        }
    }
}

std::shared_ptr<Button> Containers::addButtonPercent(
    const std::string& id,
    float xPercent, float yPercent,
    float widthPercent, float heightPercent,
    const std::string& text,
    std::function<void()> callback
)
{
    float x = (_bounds.width * xPercent) / 100.0f;
    float y = (_bounds.height * yPercent) / 100.0f;
    float width = (_bounds.width * widthPercent) / 100.0f;
    float height = (_bounds.height * heightPercent) / 100.0f;

    auto button = std::make_shared<Button>(_raylib, _audio, x, y, width, height,
        text, callback);

    button->setRelativePosition(xPercent, yPercent, widthPercent, heightPercent);

    if (addElement(id, button))
        return button;

    return nullptr;
}

std::shared_ptr<Button> Containers::addButtonPercent(
    const std::string& id,
    float xPercent, float yPercent,
    float widthPercent, float heightPercent,
    const std::string& text,
    std::function<void()> callback,
    Color normalColor,
    Color hoverColor,
    Color pressedColor,
    Color textColor
)
{
    auto button = addButtonPercent(
        id,
        xPercent,
        yPercent,
        widthPercent,
        heightPercent,
        text,
        callback);

    if (button)
        button->setColors(normalColor, hoverColor, pressedColor, textColor);

    return button;
}

std::shared_ptr<Text> Containers::addTextPercent(
    const std::string& id,
    float xPercent, float yPercent,
    const std::string& text,
    float fontSizePercent,
    Color color
)
{
    float x = (_bounds.width * xPercent) / 100.0f;
    float y = (_bounds.height * yPercent) / 100.0f;
    float fontSize = (_bounds.height * fontSizePercent) / 100.0f;

    auto textElement = std::make_shared<Text>(_raylib, x, y, text, fontSize, color);

    textElement->setRelativePosition(xPercent, yPercent, 0.0f, fontSizePercent);

    if (addElement(id, textElement))
        return textElement;

    return nullptr;
}
