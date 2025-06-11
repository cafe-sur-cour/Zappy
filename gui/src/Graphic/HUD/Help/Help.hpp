/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Help
*/

#pragma once

#include <memory>
#include <string>
#include "../Containers/Containers.hpp"
#include "../../IDisplay.hpp"
#include "../../../Audio/IAudio.hpp"

/**
 * @brief Help popup display class
 *
 * This class manages the help popup that appears when the help button is clicked.
 * It displays helpful information about the game and provides a close button.
 */
class Help {
    public:
        /**
         * @brief Construct a new Help object
         *
         * @param display Reference to the IDisplay instance
         * @param audio Reference to the audio system
         */
        Help(std::shared_ptr<IDisplay> display, std::shared_ptr<IAudio> audio);

        /**
         * @brief Destroy the Help object
         */
        ~Help() = default;

        /**
         * @brief Show the help popup
         */
        void show();

        /**
         * @brief Hide the help popup
         */
        void hide();

        /**
         * @brief Check if the help popup is visible
         *
         * @return true if visible, false otherwise
         */
        bool isVisible() const;

        /**
         * @brief Updates the help popup state
         */
        void update();

        /**
         * @brief Draw the help popup
         */
        void draw();

        /**
         * @brief Handle window resize events
         *
         * @param oldWidth Previous window width
         * @param oldHeight Previous window height
         * @param newWidth New window width
         * @param newHeight New window height
         */
        void handleResize(int oldWidth, int oldHeight, int newWidth, int newHeight);

    private:
        /**
         * @brief Initialize the help popup container and its elements
         */
        void initHelpContainer();

        std::shared_ptr<IDisplay> _display;
        std::shared_ptr<IAudio> _audio;
        std::shared_ptr<Containers> _helpContainer;
        bool _visible;
};
