/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** ImageButton
*/

#include "ImageButton.hpp"

ImageButton::ImageButton(
    std::shared_ptr<IDisplay> display,
    std::shared_ptr<IAudio> audio,
    float x, float y,
    float width, float height,
    const std::string& imagePath,
    std::function<void()> callback
) : Image(display, x, y, width, height, imagePath),
    _callback(callback),
    _audio(audio),
    _isHovered(false),
    _isPressed(false)
{
}

void ImageButton::update()
{
    Image::update(); // Appeler l'update de la classe parent
    
    if (!_visible)
        return;
    
    Vector2f mousePoint = _display->getMousePosition();
    _isHovered = contains(mousePoint.x, mousePoint.y);
    
    bool wasPressed = _isPressed;
    _isPressed = _isHovered && _display->isMouseButtonDown(
        _display->getKeyId(MOUSE_LEFT));
    
    if (wasPressed && !_isPressed && _isHovered && _callback) {
        if (_audio) {
            _audio->playSound("click", this->_audio->getSFXVolumeLevel());
        }
        _callback();
    }
}

void ImageButton::setCallback(std::function<void()> callback)
{
    _callback = callback;
}

std::function<void()> ImageButton::getCallback() const
{
    return _callback;
}
