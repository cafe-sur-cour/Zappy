/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** AContainers
*/

#pragma once

#include "IContainers.hpp"
#include "Graphic/RayLib/RayLib.hpp"
#include <string>
#include <memory>
#include <vector>

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
        AContainers(std::shared_ptr<RayLib> raylib, float x, float y, float width, float height);

        /**
         * @brief Destroy the AContainers object
         */
        virtual ~AContainers() = default;

        void setPosition(float x, float y) override;
        void setSize(float width, float height) override;
        Rectangle getBounds() const override;
        bool contains(float x, float y) const override;
        void setVisible(bool visible) override;
        bool isVisible() const override;

    protected:
        std::shared_ptr<RayLib> _raylib;
        Rectangle _bounds;
        Color _backgroundColor;
        bool _visible;
        bool _hasBackground;
};
