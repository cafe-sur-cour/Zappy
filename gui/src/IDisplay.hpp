/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** IDisplay
*/

#ifndef IDISPLAY_HPP_
#define IDISPLAY_HPP_

enum Key {
    TAB,
    UP,
    DOWN,
    RIGHT,
    LEFT,
    GM_PD_LEFT_SHOULDER,
    GM_PD_RIGHT_SHOULDER,
    GM_PD_LEFT_TRIGGER,
    GM_PD_RIGHT_TRIGGER,
    GM_PD_UP,
    GM_PD_DOWN,
};

class IDisplay {
    public:
        virtual std::pair<int, int> getMonitorSize() = 0;
        virtual std::pair<int, int> getScreenSize() = 0;

        virtual void initWindow(int width, int height, std::string) = 0;
        virtual void initCamera() = 0;
        virtual bool isWindowReady() = 0;
        virtual void setTargetFPS(unsigned int FPS) = 0;

        virtual bool isOpen() = 0;

        virtual int getKeyId(enum Key) = 0;

        virtual bool isKeyReleased(int key) = 0;
        virtual bool isGamepadButtonReleased(int key) = 0;

        virtual bool isKeyDown(int key) = 0;
        virtual bool isGamepadButtonDown(int key) = 0;

        virtual void closeWindow() = 0;
        ~IDisplay() = default;
};

#endif /* !IDISPLAY_HPP_ */
