/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Slider
*/

#include "Slider.hpp"
#include <chrono>
#include <algorithm>
#include <cmath>

Slider::Slider(
    std::shared_ptr<RayLib> raylib,
    float x, float y,
    float width, float height,
    float minValue, float maxValue,
    float initialValue,
    const std::string& text,
    std::function<void(float)> onValueChanged
) : AUIElement(raylib, x, y, width, height),
    _value(initialValue),
    _minValue(minValue),
    _maxValue(maxValue),
    _text(text),
    _onValueChanged(onValueChanged),
    _isDragging(false),
    _sliderTrackWidth(width * 0.6f),
    _sliderHandleRadius(height * 0.15f),
    _trackColor({200, 200, 200, 255}),
    _fillColor({100, 150, 255, 255}),
    _handleColor({255, 255, 255, 255}),
    _textColor({255, 255, 255, 255}),
    _lastChangeTime(0.0f),
    _hasUnnotifiedChange(false),
    _lastNotifiedValue(initialValue)
{
    _value = std::max(_minValue, std::min(_maxValue, _value));
}

void Slider::draw()
{
    if (!_visible)
        return;

    float textFontSize = _bounds.height * 0.3f;

    float textX = _bounds.x;
    float textY = _bounds.y;
    _raylib->drawText(_text, textX, textY, textFontSize, _textColor);

    std::string valueText = std::to_string(static_cast<int>(_value));
    float valueFontSize = _bounds.height * 0.25f;
    float textWidth = _raylib->measureText(_text, textFontSize);
    float valueX = textX + textWidth + 10.0f;
    float valueY = _bounds.y;
    _raylib->drawText(valueText, valueX, valueY, valueFontSize, _textColor);

    float sliderY = _bounds.y + textFontSize + 10.0f;
    float sliderStartX = _bounds.x;

    float trackThickness = _bounds.height * 0.15f;
    Rectangle trackRect = {
        sliderStartX,
        sliderY - trackThickness * 0.5f,
        _sliderTrackWidth,
        trackThickness
    };
    _raylib->drawRectangleRec(trackRect, _trackColor);

    float handlePos = getHandlePosition();
    Rectangle fillRect = {
        sliderStartX,
        sliderY - trackThickness * 0.5f,
        handlePos - sliderStartX,
        trackThickness
    };
    _raylib->drawRectangleRec(fillRect, _fillColor);

    _raylib->drawCircle(handlePos, sliderY, _sliderHandleRadius, _handleColor);
    _raylib->drawCircleLines(handlePos, sliderY, _sliderHandleRadius, {100, 100, 100, 255});
}

void Slider::update()
{
    if (!_visible)
        return;

    Vector2 mousePos = _raylib->getMousePosition();

    if (_raylib->isMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (isMouseOverHandle(mousePos.x, mousePos.y)) {
            _isDragging = true;
        }
    }

    if (_isDragging) {
        if (_raylib->isMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            updateValueFromMousePosition(mousePos.x);
        } else {
            _isDragging = false;
        }
    }

    if (_hasUnnotifiedChange) {
        _lastChangeTime += _raylib->getFrameTime();
        if (_lastChangeTime >= 0.5f) {
            if (_onValueChanged && _value != _lastNotifiedValue) {
                _onValueChanged(_value);
                _lastNotifiedValue = _value;
            }
            _hasUnnotifiedChange = false;
            _lastChangeTime = 0.0f;
        }
    }
}

void Slider::setValue(float value)
{
    float oldValue = _value;
    _value = std::max(_minValue, std::min(_maxValue, value));

    if (_value != oldValue) {
        _lastChangeTime = 0.0f;
        _hasUnnotifiedChange = true;
    }
}

float Slider::getValue() const
{
    return _value;
}

void Slider::setMinValue(float minValue)
{
    _minValue = minValue;
    if (_value < _minValue) {
        setValue(_minValue);
    }
}

void Slider::setMaxValue(float maxValue)
{
    _maxValue = maxValue;
    if (_value > _maxValue)
        setValue(_maxValue);
}

float Slider::getMinValue() const
{
    return _minValue;
}

float Slider::getMaxValue() const
{
    return _maxValue;
}

void Slider::setText(const std::string& text)
{
    _text = text;
}

std::string Slider::getText() const
{
    return _text;
}

void Slider::setSize(float width, float height)
{
    AUIElement::setSize(width, height);
    _sliderTrackWidth = width * 0.6f;
    _sliderHandleRadius = height * 0.15f;
}

void Slider::updateValueFromMousePosition(float mouseX)
{
    float sliderStartX = _bounds.x;
    float sliderEndX = sliderStartX + _sliderTrackWidth;

    float clampedX = std::max(sliderStartX, std::min(sliderEndX, mouseX));

    float ratio = (clampedX - sliderStartX) / _sliderTrackWidth;
    float newValue = _minValue + ratio * (_maxValue - _minValue);

    setValue(newValue);
}

float Slider::getHandlePosition() const
{
    float sliderStartX = _bounds.x;

    float ratio = (_value - _minValue) / (_maxValue - _minValue);
    return sliderStartX + ratio * _sliderTrackWidth;
}

bool Slider::isMouseOverHandle(float mouseX, float mouseY) const
{
    float handleX = getHandlePosition();
    float textFontSize = _bounds.height * 0.3f;
    float handleY = _bounds.y + textFontSize + 10.0f;

    float dx = mouseX - handleX;
    float dy = mouseY - handleY;
    float distance = std::sqrt(dx * dx + dy * dy);

    return distance <= _sliderHandleRadius;
}

bool Slider::isDragging() const
{
    return _isDragging;
}
