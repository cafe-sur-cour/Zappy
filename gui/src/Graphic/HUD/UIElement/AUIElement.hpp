/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** AUIElement
*/

#pragma once

#include "IUIElement.hpp"
#include "../../RayLib/RayLib.hpp"

/**
 * @brief Abstract base class for UI elements
 *
 * Provides common functionality for all UI elements
 */
class AUIElement : public IUIElement {
    public:
        /**
         * @brief Construct a new AUIElement object
         *
         * @param x X coordinate
         * @param y Y coordinate
         * @param width Element width
         * @param height Element height
         */
        AUIElement(float x, float y, float width, float height);

        /**
         * @brief Destroy the AUIElement object
         */
        virtual ~AUIElement() = default;

        // IUIElement implementation
        void setPosition(float x, float y) override;
        Rectangle getBounds() const override;
        bool contains(float x, float y) const override;
        void setVisible(bool visible) override;
        bool isVisible() const override;

    protected:
        Rectangle _bounds;
        bool _visible;
};
