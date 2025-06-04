/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** IUIElement
*/

#pragma once

#include "../../RayLib/RayLib.hpp"

/**
 * @brief Interface for all UI elements
 *
 * Base interface that all UI elements (buttons, text, scrollbars, etc.) must implement
 */
class IUIElement {
    public:
        virtual ~IUIElement() = default;

        /**
         * @brief Draw the UI element
         */
        virtual void draw() = 0;

        /**
         * @brief Update the UI element's state
         */
        virtual void update() = 0;

        /**
         * @brief Set the position of the UI element
         *
         * @param x X coordinate
         * @param y Y coordinate
         */
        virtual void setPosition(float x, float y) = 0;

        /**
         * @brief Get the bounds of the UI element
         *
         * @return Rectangle The bounds of the element
         */
        virtual Rectangle getBounds() const = 0;

        /**
         * @brief Check if the UI element contains a point
         *
         * @param x X coordinate
         * @param y Y coordinate
         *
         * @return true If the point is inside the element
         * @return false Otherwise
         */
        virtual bool contains(float x, float y) const = 0;

        /**
         * @brief Set the visibility of the UI element
         *
         * @param visible Visibility state
         */
        virtual void setVisible(bool visible) = 0;

        /**
         * @brief Check if the UI element is visible
         *
         * @return true If visible
         * @return false Otherwise
         */
        virtual bool isVisible() const = 0;
};
