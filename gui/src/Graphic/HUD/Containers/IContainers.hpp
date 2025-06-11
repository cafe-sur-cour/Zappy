/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** IContainers
*/

#pragma once

#include <string>
#include <memory>
#include <vector>
#include "../../IDisplay.hpp"

/**
 * @brief Interface for HUD containers
 *
 * Containers are UI elements that can hold and organize other UI elements
 * like buttons, text, scrollbars, etc.
 */
class IContainers {
    public:
        virtual ~IContainers() = default;

        /**
         * @brief Draw the container and its contents
         */
        virtual void draw() = 0;

        /**
         * @brief Update the container's state
         */
        virtual void update() = 0;

        /**
         * @brief Set the position of the container
         *
         * @param x X coordinate
         * @param y Y coordinate
         */
        virtual void setPosition(float x, float y) = 0;

        /**
         * @brief Set the size of the container
         *
         * @param width Container width
         * @param height Container height
         */
        virtual void setSize(float width, float height) = 0;

        /**
         * @brief Get the current position of the container
         *
         * @return Rectangle Containing position and size
         */
        virtual FloatRect getBounds() const = 0;

        /**
         * @brief Check if a point is within the container
         *
         * @param x X coordinate
         * @param y Y coordinate
         *
         * @return true If point is within container
         * @return false Otherwise
         */
        virtual bool contains(float x, float y) const = 0;

        /**
         * @brief Set the visibility of the container
         *
         * @param visible Visibility state
         */
        virtual void setVisible(bool visible) = 0;

        /**
         * @brief Check if the container is visible
         *
         * @return true If visible
         * @return false Otherwise
         */
        virtual bool isVisible() const = 0;
};
