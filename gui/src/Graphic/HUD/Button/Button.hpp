/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Button
*/

#pragma once

#include <string>
#include <functional>
#include <memory>

#include "../UIElement/AUIElement.hpp"
#include "../../RayLib/RayLib.hpp"
#include "../../Audio/Audio.hpp"

/**
 * @brief Button UI element
 *
 * A clickable button with text that can trigger a callback when clicked
 */
class Button : public AUIElement {
    public:
        /**
         * @brief Construct a new Button
         *
         * @param x X coordinate
         * @param y Y coordinate
         * @param width Button width
         * @param height Button height
         * @param text Button text
         * @param callback Function to call when button is clicked
         */
        Button(
            std::shared_ptr<RayLib> raylib,
            std::shared_ptr<Audio> audio,
            float x, float y,
            float width, float height,
            const std::string& text,
            std::function<void()> callback
        );

        /**
         * @brief Destroy the Button
         */
        ~Button() override = default;

        /**
         * @brief Draw the button
         */
        void draw() override;

        /**
         * @brief Update the button state
         */
        void update() override;

        /**
         * @brief Set the text of the button
         *
         * @param text New text
         */
        void setText(const std::string& text);

        /**
         * @brief Get the text of the button
         *
         * @return std::string Button text
         */
        std::string getText() const;

        /**
         * @brief Set the callback function
         *
         * @param callback Function to call when button is clicked
         */
        void setCallback(std::function<void()> callback);

        /**
         * @brief Set the colors of the button
         *
         * @param normal Normal color
         * @param hover Hover color
         * @param pressed Pressed color
         * @param textColor Text color
         */
        void setColors(
            Color normal,
            Color hover,
            Color pressed,
            Color textColor
        );

        /**
         * @brief Set the size of the button
         *
         * @param width New button width
         * @param height New button height
         */
        void setSize(float width, float height) override;

    private:
        std::string _text;
        std::function<void()> _callback;

        Color _normalColor;
        Color _hoverColor;
        Color _pressedColor;
        Color _textColor;

        bool _isHovered;
        bool _isPressed;

        std::shared_ptr<RayLib> _raylib;
        std::shared_ptr<Audio> _audio;
};
