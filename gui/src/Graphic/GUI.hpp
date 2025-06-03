/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GUI
*/

#ifndef GUI_HPP_
#define GUI_HPP_

#include "RayLib/RayLib.hpp"

class GUI {
    public:
        GUI();
        ~GUI();

        void run();

    private:
        void updateCamera();
        void drawEnvironment();

        RayLib _raylib;
        bool _isRunning;
};

#endif /* !GUI_HPP_ */
