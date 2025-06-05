/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** AUIElement
*/

#pragma once

#include "IUIElement.hpp"
#include "../../RayLib/RayLib.hpp"
#include <memory>

/**
 * @brief Structure to store relative positions and sizes as percentages
 */
struct UIRelativePosition {
    float xPercent;
    float yPercent;
    float widthPercent;
    float heightPercent;
};

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
        AUIElement(std::shared_ptr<RayLib> raylib,float x, float y, float width, float height);

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

        /**
         * @brief Set the element size
         *
         * @param width New width
         * @param height New height
         */
        virtual void setSize(float width, float height);

        /**
         * @brief Set position and size as percentages of parent container
         *
         * @param xPercent X position as percentage of container width (0-100)
         * @param yPercent Y position as percentage of container height (0-100)
         * @param widthPercent Width as percentage of container width (0-100)
         * @param heightPercent Height as percentage of container height (0-100)
         */
        void setRelativePosition(float xPercent, float yPercent, float widthPercent, float heightPercent);

        /**
         * @brief Get the relative position
         *
         * @return UIRelativePosition The relative position and size
         */
        UIRelativePosition getRelativePosition() const;

    protected:
        std::shared_ptr<RayLib> _raylib;
        Rectangle _bounds;
        UIRelativePosition _relativePos;
        bool _visible;
};
