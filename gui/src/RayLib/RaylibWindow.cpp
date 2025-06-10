/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** WindowMethods
*/

#include <string>
#include "RayLib.hpp"

void RayLib::initWindow(int width, int height, const std::string &title)
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(width, height, title.c_str());
    _isInitialized = true;
}

void RayLib::closeWindow()
{
    if (_isInitialized) {
        CloseWindow();
        _isInitialized = false;
    }
}

bool RayLib::windowShouldClose() const
{
    return WindowShouldClose();
}

void RayLib::beginDrawing()
{
    BeginDrawing();
}

void RayLib::endDrawing()
{
    EndDrawing();
}

void RayLib::clearBackground(Color color)
{
    ClearBackground(color);
}

bool RayLib::isWindowReady() const
{
    return IsWindowReady();
}

int RayLib::getMonitorWidth(int monitor) const
{
    return GetMonitorWidth(monitor);
}

int RayLib::getMonitorHeight(int monitor) const
{
    return GetMonitorHeight(monitor);
}

void RayLib::waitTime(float seconds) const
{
    WaitTime(seconds);
}

void RayLib::setTargetFPS(int fps) const
{
    SetTargetFPS(fps);
}

int RayLib::getFPS() const
{
    return GetFPS();
}

float RayLib::getFrameTime() const
{
    return GetFrameTime();
}
