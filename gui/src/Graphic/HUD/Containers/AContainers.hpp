/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** AContainers
*/

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "IContainers.hpp"

/**
 * @brief Structure to store relative positions and sizes as percentages
 */
struct RelativePosition {
    float xPercent;
    float yPercent;
    float widthPercent;
    float heightPercent;
};

/**
 * @brief Abstract base class for containers
 *
 * Provides common functionality for all container types
 */
class AContainers : public IContainers {
    public:
        /**
         * @brief Construct a new AContainers object
         *
         * @param x X coordinate
         * @param y Y coordinate
         * @param width Container width
         * @param height Container height
         */
        AContainers(std::shared_ptr<IDisplay> display, float x, float y, float width,
            float height);

        /**
         * @brief Destroy the AContainers object
         */
        virtual ~AContainers() = default;

        void setPosition(float x, float y) override;
        void setSize(float width, float height) override;
        FloatRect getBounds() const override;
        bool contains(float x, float y) const override;
        void setVisible(bool visible) override;
        bool isVisible() const override;

        /**
         * @brief Set position and size as percentages of screen dimensions
         *
         * @param xPercent X position as percentage of screen width (0-100)
         * @param yPercent Y position as percentage of screen height (0-100)
         * @param widthPercent Width as percentage of screen width (0-100)
         * @param heightPercent Height as percentage of screen height (0-100)
         */
        void setRelativePosition(float xPercent, float yPercent, float widthPercent,
            float heightPercent);

        /**
         * @brief Get the container's relative position
         *
         * @return RelativePosition The relative position and size
         */
        RelativePosition getRelativePosition() const;

        /**
         * @brief Update the container's absolute position from relative position
         */
        void updatePositionFromRelative();

    protected:
        std::shared_ptr<IDisplay> _display;
        FloatRect _bounds;
        RelativePosition _relativePos;
        Color32 _backgroundColor;
        bool _visible;
        bool _hasBackground;
};
