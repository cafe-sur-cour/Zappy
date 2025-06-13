/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Image
*/

#include "Image.hpp"
#include <memory>
#include <string>

Image::Image(
    std::shared_ptr<IDisplay> display,
    float x, float y,
    float width, float height,
    const std::string& imagePath
) : AUIElement(display, x, y, width, height),
    _imagePath(imagePath),
    _tint({255, 255, 255, 255}), // White tint (no color modification)
    _maintainAspectRatio(true),
    _imageLoaded(false)
{
    loadImage();
}

void Image::draw()
{
    if (!_visible)
        return;

    if (_imageLoaded) {
        // Draw the actual texture using the display interface
        this->_display->drawTextureScaled(_imagePath, _bounds.x, _bounds.y, _bounds.width, _bounds.height, _tint);
    } else {
        // Draw placeholder rectangle with a different color to indicate missing image
        Color32 placeholderColor = {200, 200, 200, 128};
        this->_display->drawRectangleRec(_bounds, placeholderColor);
        
        // Draw "No Image" text in the center
        float fontSize = _bounds.height * 0.2f;
        std::string placeholderText = "No Image";
        float textWidth = this->_display->measureText(placeholderText, fontSize);
        float textX = _bounds.x + (_bounds.width - textWidth) * 0.5f;
        float textY = _bounds.y + (_bounds.height - fontSize) * 0.5f;
        
        Color32 textColor = {100, 100, 100, 255};
        this->_display->drawText(placeholderText, textX, textY, fontSize, textColor);
    }
}

void Image::update()
{
    if (!_visible)
        return;
    
    // Images typically don't need complex update logic
    // But this can be extended for animations, hover effects, etc.
}

void Image::setImagePath(const std::string& imagePath)
{
    if (_imagePath != imagePath) {
        _imagePath = imagePath;
        loadImage();
    }
}

std::string Image::getImagePath() const
{
    return _imagePath;
}

void Image::setTint(Color32 tint)
{
    _tint = tint;
}

Color32 Image::getTint() const
{
    return _tint;
}

void Image::setSize(float width, float height)
{
    if (_maintainAspectRatio) {
        // Calculate aspect ratio and adjust dimensions accordingly
        // This is a simplified version - you might want more sophisticated logic
        float aspectRatio = _bounds.width / _bounds.height;
        float newAspectRatio = width / height;
        
        if (newAspectRatio > aspectRatio) {
            // Width is too large, adjust it
            width = height * aspectRatio;
        } else {
            // Height is too large, adjust it
            height = width / aspectRatio;
        }
    }
    
    AUIElement::setSize(width, height);
}

void Image::setMaintainAspectRatio(bool maintain)
{
    _maintainAspectRatio = maintain;
}

bool Image::getMaintainAspectRatio() const
{
    return _maintainAspectRatio;
}

void Image::loadImage()
{
    // Load the texture using the display interface
    if (!_imagePath.empty()) {
        _imageLoaded = this->_display->loadTexture(_imagePath, _imagePath);
    } else {
        _imageLoaded = false;
    }
}
