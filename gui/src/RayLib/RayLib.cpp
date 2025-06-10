/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** RayLib
*/

#define NDEBUG
#define SUPPORT_TRACELOG_DEBUG    0
#define TRACELOG_SILENT

#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include "../Utils/Constants.hpp"
#include "RayLib.hpp"
#include "raylib.h"
#include "raymath.h"

RayLib::RayLib() : _isInitialized(false), _isCursorLocked(false)
{
    _camera = {{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
        0.0f, CAMERA_PERSPECTIVE };
    _previousMousePosition = (Vector2){ 0.0f, 0.0f };
}

RayLib::~RayLib()
{
    if (_isInitialized)
        closeWindow();

    unloadAllModels();
}

void RayLib::drawRectangleRec(Rectangle rec, Color color)
{
    DrawRectangleRec(rec, color);
}

void RayLib::drawText(const std::string& text, float x, float y, float fontSize, Color color)
{
    DrawText(text.c_str(),
             static_cast<int>(x),
             static_cast<int>(y),
             static_cast<int>(fontSize),
             color);
}

float RayLib::measureText(const std::string& text, float fontSize) const
{
    return static_cast<float>(MeasureText(text.c_str(), static_cast<int>(fontSize)));
}

void RayLib::drawTextureRec(Texture2D texture, Rectangle source, Vector2 position, Color tint)
{
    DrawTextureRec(texture, source, position, tint);
}

void RayLib::unloadTexture(Texture2D texture)
{
    UnloadTexture(texture);
}

bool RayLib::checkCollisionPointRec(Vector2 point, Rectangle rec) const
{
    return CheckCollisionPointRec(point, rec);
}

void RayLib::beginScissorMode(int x, int y, int width, int height)
{
    BeginScissorMode(x, y, width, height);
}

void RayLib::endScissorMode()
{
    EndScissorMode();
}
