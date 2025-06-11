/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Containers
*/

#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include <memory>
#include <string>

#include "AContainers.hpp"
#include "../UIElement/IUIElement.hpp"
#include "../Button/Button.hpp"
#include "../Text/Text.hpp"
#include "../../../Audio/IAudio.hpp"
#include "../../../IDisplay.hpp"

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
         * @param display Reference to the IDisplay instance
         * @param audio Reference to the Audio instance
         * @param x X coordinate
         * @param y Y coordinate
         * @param width Container width
         * @param height Container height
         * @param backgroundColor Background color (default: semi-transparent dark gray)
         */
        Containers(std::shared_ptr<IDisplay> display, std::shared_ptr<IAudio> audio,
            float x, float y, float width, float height,
            Color32 backgroundColor = {40, 40, 40, 200});

        /**
         * @brief Destroy the Container
         */
        ~Containers() override = default;

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
        void setBackgroundColor(Color32 color);

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
         * @brief Create and add a button to the container with custom colors
         *
         * @param id Unique identifier for the button
         * @param x X coordinate relative to container
         * @param y Y coordinate relative to container
         * @param width Button width
         * @param height Button height
         * @param text Button text
         * @param callback Function to call when button is clicked
         * @param normalColor Color when not interacting
         * @param hoverColor Color when mouse is hovering over button
         * @param pressedColor Color when button is pressed
         * @param textColor Color of the button text
         *
         * @return std::shared_ptr<Button> Pointer to the created button, or nullptr if ID already exists
         */
        std::shared_ptr<Button> addButton(
            const std::string& id,
            float x, float y,
            float width, float height,
            const std::string& text,
            std::function<void()> callback,
            Color32 normalColor,
            Color32 hoverColor,
            Color32 pressedColor,
            Color32 textColor
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
            Color32 color = CBLACK
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

        /**
         * @brief Create and add a button to the container using relative percentages
         *
         * @param id Unique identifier for the button
         * @param xPercent X position as percentage of container width (0-100)
         * @param yPercent Y position as percentage of container height (0-100)
         * @param widthPercent Width as percentage of container width (0-100)
         * @param heightPercent Height as percentage of container height (0-100)
         * @param text Button text
         * @param callback Function to call when button is clicked
         *
         * @return std::shared_ptr<Button> Pointer to the created button, or nullptr if ID already exists
         */
        std::shared_ptr<Button> addButtonPercent(
            const std::string& id,
            float xPercent, float yPercent,
            float widthPercent, float heightPercent,
            const std::string& text,
            std::function<void()> callback
        );

        /**
         * @brief Create and add a button to the container with custom colors using relative percentages
         *
         * @param id Unique identifier for the button
         * @param xPercent X position as percentage of container width (0-100)
         * @param yPercent Y position as percentage of container height (0-100)
         * @param widthPercent Width as percentage of container width (0-100)
         * @param heightPercent Height as percentage of container height (0-100)
         * @param text Button text
         * @param callback Function to call when button is clicked
         * @param normalColor Color when not interacting
         * @param hoverColor Color when mouse is hovering over button
         * @param pressedColor Color when button is pressed
         * @param textColor Color of the button text
         *
         * @return std::shared_ptr<Button> Pointer to the created button, or nullptr if ID already exists
         */
        std::shared_ptr<Button> addButtonPercent(
            const std::string& id,
            float xPercent, float yPercent,
            float widthPercent, float heightPercent,
            const std::string& text,
            std::function<void()> callback,
            Color32 normalColor,
            Color32 hoverColor,
            Color32 pressedColor,
            Color32 textColor
        );

        /**
         * @brief Create and add a text element to the container using relative percentages
         *
         * @param id Unique identifier for the text element
         * @param xPercent X position as percentage of container width (0-100)
         * @param yPercent Y position as percentage of container height (0-100)
         * @param text Text content
         * @param fontSizePercent Font size as percentage of container height (0-100)
         * @param color Text color
         *
         * @return std::shared_ptr<Text> Pointer to the created text element, or nullptr if ID already exists
         */
        std::shared_ptr<Text> addTextPercent(
            const std::string& id,
            float xPercent, float yPercent,
            const std::string& text,
            float fontSizePercent = 5.0f,
            Color32 color = CBLACK
        );

    private:
        std::shared_ptr<IAudio> _audio;
        std::unordered_map<std::string, std::shared_ptr<IUIElement>> _elements;
};
