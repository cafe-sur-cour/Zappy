/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Containers
*/

#pragma once

#include "AContainers.hpp"
#include "../UIElement/IUIElement.hpp"
#include "../Button/Button.hpp"
#include "../Text/Text.hpp"
#include "../ScrollBar/ScrollBar.hpp"
#include "Graphic/RayLib/RayLib.hpp"
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>

/**
 * @brief Container class for organizing UI elements
 *
 * Containers can hold UI elements like buttons, text, and scrollbars.
 * They can have a background color or texture.
 */
class Containers : public AContainers {
    public:
        /**
         * @brief Construct a new Container
         *
         * @param raylib Reference to the RayLib instance
         * @param x X coordinate
         * @param y Y coordinate
         * @param width Container width
         * @param height Container height
         * @param backgroundColor Background color (default: semi-transparent dark gray)
         */
        Containers(std::shared_ptr<RayLib> raylib, float x, float y, float width, float height, Color backgroundColor = {40, 40, 40, 200});

        /**
         * @brief Destroy the Container
         */
        ~Containers() override;

        /**
         * @brief Draw the container and its contents
         */
        void draw() override;

        /**
         * @brief Update the container state and its contents
         */
        void update() override;

        /**
         * @brief Set the background color
         *
         * @param color New background color
         */
        void setBackgroundColor(Color color);

        /**
         * @brief Set whether to draw the background
         *
         * @param hasBackground True to draw background, false otherwise
         */
        void setHasBackground(bool hasBackground);

        /**
         * @brief Set background texture for the container
         *
         * @param texture Texture to use as background
         */
        void setBackgroundTexture(Texture2D texture);

        /**
         * @brief Check if the container has a background texture
         *
         * @return true If the container has a background texture
         * @return false Otherwise
         */
        bool hasBackgroundTexture() const;

        /**
         * @brief Add a UI element to the container
         *
         * @param id Unique identifier for the element
         * @param element UI element to add
         *
         * @return true If element was added successfully
         * @return false If element with same ID already exists
         */
        bool addElement(const std::string& id, std::shared_ptr<IUIElement> element);

        /**
         * @brief Get a UI element by its ID
         *
         * @param id Element identifier
         *
         * @return std::shared_ptr<IUIElement> Pointer to the element, or nullptr if not found
         */
        std::shared_ptr<IUIElement> getElement(const std::string& id) const;

        /**
         * @brief Remove a UI element
         *
         * @param id Element identifier
         *
         * @return true If element was found and removed
         * @return false If element was not found
         */
        bool removeElement(const std::string& id);

        /**
         * @brief Create and add a button to the container
         *
         * @param id Unique identifier for the button
         * @param x X coordinate relative to container
         * @param y Y coordinate relative to container
         * @param width Button width
         * @param height Button height
         * @param text Button text
         * @param callback Function to call when button is clicked
         *
         * @return std::shared_ptr<Button> Pointer to the created button, or nullptr if ID already exists
         */
        std::shared_ptr<Button> addButton(
            const std::string& id,
            float x, float y,
            float width, float height,
            const std::string& text,
            std::function<void()> callback
        );

        /**
         * @brief Create and add a text element to the container
         *
         * @param id Unique identifier for the text element
         * @param x X coordinate relative to container
         * @param y Y coordinate relative to container
         * @param text Text content
         * @param fontSize Font size
         * @param color Text color
         *
         * @return std::shared_ptr<Text> Pointer to the created text element, or nullptr if ID already exists
         */
        std::shared_ptr<Text> addText(
            const std::string& id,
            float x, float y,
            const std::string& text,
            float fontSize = 20.0f,
            Color color = BLACK
        );

        /**
         * @brief Create and add a scrollbar to the container
         *
         * @param id Unique identifier for the scrollbar
         * @param x X coordinate relative to container
         * @param y Y coordinate relative to container
         * @param length Length of the scrollbar
         * @param thickness Thickness of the scrollbar
         * @param orientation Orientation (vertical or horizontal)
         * @param onValueChanged Callback when value changes
         *
         * @return std::shared_ptr<ScrollBar> Pointer to the created scrollbar, or nullptr if ID already exists
         */
        std::shared_ptr<ScrollBar> addScrollBar(
            const std::string& id,
            float x, float y,
            float length, float thickness,
            ScrollBarOrientation orientation = ScrollBarOrientation::VERTICAL,
            std::function<void(float)> onValueChanged = nullptr
        );

        /**
         * @brief Clear all UI elements from the container
         */
        void clearElements();

        /**
         * @brief Handle window resize event
         *
         * @param oldWidth Previous window width
         * @param oldHeight Previous window height
         * @param newWidth New window width
         * @param newHeight New window height
         */
        void handleResize(int oldWidth, int oldHeight, int newWidth, int newHeight);

    private:
        std::shared_ptr<RayLib> _raylib;
        Texture2D _backgroundTexture;
        bool _hasBackgroundTexture;
        std::unordered_map<std::string, std::shared_ptr<IUIElement>> _elements;
};
