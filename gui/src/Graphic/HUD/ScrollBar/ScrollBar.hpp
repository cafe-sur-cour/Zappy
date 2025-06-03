/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** ScrollBar
*/

#pragma once

#include "../UIElement/AUIElement.hpp"
#include "Graphic/RayLib/RayLib.hpp"
#include <functional>
#include <memory>

/**
 * @brief Scrollbar orientation
 */
enum class ScrollBarOrientation {
    VERTICAL,
    HORIZONTAL
};

/**
 * @brief ScrollBar UI element
 *
 * A UI element for scrolling content
 */
class ScrollBar : public AUIElement {
    public:
        /**
         * @brief Construct a new ScrollBar
         *
         * @param x X coordinate
         * @param y Y coordinate
         * @param length Length of the scrollbar
         * @param thickness Thickness of the scrollbar
         * @param orientation Orientation (vertical or horizontal)
         * @param onValueChanged Callback when value changes
         */
        ScrollBar(
            std::shared_ptr<RayLib> raylib,
            float x, float y,
            float length, float thickness,
            ScrollBarOrientation orientation = ScrollBarOrientation::VERTICAL,
            std::function<void(float)> onValueChanged = nullptr
        );

        /**
         * @brief Destroy the ScrollBar
         */
        ~ScrollBar() override = default;

        /**
         * @brief Draw the scrollbar
         */
        void draw() override;

        /**
         * @brief Update the scrollbar state
         */
        void update() override;

        /**
         * @brief Set the current value
         *
         * @param value New value (between 0.0 and 1.0)
         */
        void setValue(float value);

        /**
         * @brief Get the current value
         *
         * @return float Current value (between 0.0 and 1.0)
         */
        float getValue() const;

        /**
         * @brief Set the colors of the scrollbar
         *
         * @param background Background color
         * @param handle Handle color
         * @param handleHover Handle hover color
         * @param handlePressed Handle pressed color
         */
        void setColors(
            Color background,
            Color handle,
            Color handleHover,
            Color handlePressed
        );

        /**
         * @brief Set the callback for value changes
         *
         * @param onValueChanged Callback function
         */
        void setOnValueChanged(std::function<void(float)> onValueChanged);

    private:
        std::shared_ptr<RayLib> _raylib;
        float _value;
        float _handleSize;
        bool _isDragging;
        ScrollBarOrientation _orientation;
        std::function<void(float)> _onValueChanged;

        Color _backgroundColor;
        Color _handleColor;
        Color _handleHoverColor;
        Color _handlePressedColor;

        bool _isHandleHovered;

        /**
         * @brief Calculate the bounds of the handle
         *
         * @return Rectangle Handle bounds
         */
        Rectangle getHandleBounds() const;

        /**
         * @brief Calculate value from mouse position
         *
         * @param mousePosition Mouse position
         *
         * @return float Calculated value (0.0 - 1.0)
         */
        float calculateValueFromMousePosition(Vector2 mousePosition) const;
};
