/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** HUD
*/

#pragma once

#include "Containers/Containers.hpp"
#include "../RayLib/RayLib.hpp"
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

/**
 * @brief Main HUD class to manage all UI elements
 *
 * This class handles the creation, management, and rendering of all UI containers
 * and their elements like buttons, text, scrollbars, etc.
 */
class HUD {
    public:
        /**
         * @brief Construct a new HUD object
         *
         * @param raylib Reference to the RayLib instance
         */
        HUD(std::shared_ptr<RayLib> raylib);

        /**
         * @brief Destroy the HUD object
         */
        ~HUD();

        /**
         * @brief Draw all visible HUD elements
         */
        void draw();

        /**
         * @brief Update all HUD elements
         */
        void update();

        /**
         * @brief Add a new container to the HUD
         *
         * @param id Unique identifier for the container
         * @param x X coordinate
         * @param y Y coordinate
         * @param width Container width
         * @param height Container height
         * @param backgroundColor Background color (optional)
         *
         * @return std::shared_ptr<Containers> Pointer to the created container
         */
        std::shared_ptr<Containers> addContainer(
            const std::string& id,
            float x, float y,
            float width, float height,
            Color backgroundColor = {40, 40, 40, 200}
        );

        /**
         * @brief Get a container by its ID
         *
         * @param id Container identifier
         *
         * @return std::shared_ptr<Containers> Pointer to the container, or nullptr if not found
         */
        std::shared_ptr<Containers> getContainer(const std::string& id) const;

        /**
         * @brief Remove a container and all its child elements
         *
         * @param id Container identifier
         *
         * @return true If container was found and removed
         * @return false If container was not found
         */
        bool removeContainer(const std::string& id);

        /**
         * @brief Handle window resize event
         *
         * Updates all containers and UI elements to adjust to the new window size
         *
         * @param oldWidth Previous window width
         * @param oldHeight Previous window height
         * @param newWidth New window width
         * @param newHeight New window height
         */
        void handleResize(int oldWidth, int oldHeight, int newWidth, int newHeight);

        /**
         * @brief Clear all containers from the HUD
         */
        void clearAllContainers();

        /**
         * @brief Initialize default layout with side and bottom containers
         *
         * Creates and adds default containers for the left side and bottom of the screen
         *
         * @param sideWidth Width of the side container (default: 250 pixels)
         * @param bottomHeight Height of the bottom container (default: 200 pixels)
         */
        void initDefaultLayout(float sideWidth = 250.0f, float bottomHeight = 200.0f);

        /**
         * @brief Get the side container
         *
         * @return std::shared_ptr<Containers> Pointer to the side container
         */
        std::shared_ptr<Containers> getSideContainer() const;

        /**
         * @brief Get the bottom container
         *
         * @return std::shared_ptr<Containers> Pointer to the bottom container
         */
        std::shared_ptr<Containers> getBottomContainer() const;

    private:
        std::unordered_map<std::string, std::shared_ptr<Containers>> _containers;
        std::shared_ptr<RayLib> _raylib;
};
