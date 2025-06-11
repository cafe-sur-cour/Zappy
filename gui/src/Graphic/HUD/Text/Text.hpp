/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Text
*/

#pragma once

#include <memory>
#include <string>

#include "../UIElement/AUIElement.hpp"
#include "../../../RayLib/RayLib.hpp"
#include "../../IDisplay.hpp"

/**
 * @brief Text UI element
 *
 * A UI element for rendering text
 */
class Text : public AUIElement {
    public:
        /**
         * @brief Construct a new Text element
         *
         * @param x X coordinate
         * @param y Y coordinate
         * @param text Text content
         * @param fontSize Font size
         * @param color Text color
         */
        Text(
            std::shared_ptr<IDisplay> raylib,
            float x, float y,
            const std::string& text,
            float fontSize = 20.0f,
            Color color = BLACK
        );

        /**
         * @brief Destroy the Text element
         */
        ~Text() override = default;

        /**
         * @brief Draw the text
         */
        void draw() override;

        /**
         * @brief Update the text (does nothing for text elements)
         */
        void update() override;

        /**
         * @brief Set the text content
         *
         * @param text New text content
         */
        void setText(const std::string& text);

        /**
         * @brief Get the text content
         *
         * @return std::string Text content
         */
        std::string getText() const;

        /**
         * @brief Set the font size
         *
         * @param fontSize New font size
         */
        void setFontSize(float fontSize);

        /**
         * @brief Get the font size
         *
         * @return float Font size
         */
        float getFontSize() const;

        /**
         * @brief Set the text color
         *
         * @param color New text color
         */
        void setColor(Color color);

        /**
         * @brief Get the text color
         *
         * @return Color Text color
         */
        Color getColor() const;

        /**
         * @brief Set the size of the text element
         * For text elements, height determines font size and width is calculated based on text content
         *
         * @param width Desired width (may be adjusted based on text content)
         * @param height Desired height (used as font size)
         */
        void setSize(float width, float height) override;

    private:
        std::string _text;
        float _fontSize;
        Color _color;
        std::shared_ptr<IDisplay> _display;
};
