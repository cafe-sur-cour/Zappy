/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Raylib
*/

#ifndef RAYLIB_HPP_
#define RAYLIB_HPP_
#include "../IDisplay.hpp"
#include "RaylibEnc/RayLibEnc.hpp"
#include <memory>

class Raylib : public IDisplay {
    private:
        std::unique_ptr<RayLibEnc> _raylib;
    public:
        virtual Vector2i getMonitorSize();
        virtual Vector2i getScreenSize();

        virtual void initWindow(int width, int height, std::string);
        virtual void initCamera();

        virtual bool isWindowReady();
        virtual void setTargetFPS(unsigned int FPS);
        virtual bool isOpen();
        virtual void closeWindow();

        virtual int getKeyId(enum Key);
        virtual bool isKeyReleased(int key);
        virtual bool isKeyPressed(int key);
        virtual bool isKeyDown(int key);

        virtual bool isGamepadAvailable();
        virtual bool isGamepadButtonReleased(int key);
        virtual bool isGamepadButtonPressed(int key);
        virtual bool isGamepadButtonDown(int key);


        Raylib() = default;
        ~Raylib() = default;
};

#endif /* !RAYLIB_HPP_ */
