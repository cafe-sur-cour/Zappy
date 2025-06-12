/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** RayLibEnc
*/

#define NDEBUG
#define SUPPORT_TRACELOG_DEBUG    0
#define TRACELOG_SILENT

#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include "../../Utils/Constants.hpp"
#include "RayLibEnc.hpp"
#include "raylib.h"
#include "raymath.h"

RayLibEnc::RayLibEnc() : _isInitialized(false), _isCursorLocked(false)
{
    _camera = {{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
        0.0f, CAMERA_PERSPECTIVE };
    _previousMousePosition = (Vector2){ 0.0f, 0.0f };
}

RayLibEnc::~RayLibEnc()
{
    if (_isInitialized)
        closeWindow();

    unloadAllModels();
}

void RayLibEnc::drawRectangleRec(Rectangle rec, Color color)
{
    DrawRectangleRec(rec, color);
}

void RayLibEnc::drawText(const std::string& text, float x, float y, float fontSize,
    Color color)
{
    DrawText(text.c_str(),
             static_cast<int>(x),
             static_cast<int>(y),
             static_cast<int>(fontSize),
             color);
}

void RayLibEnc::drawCircle(float centerX, float centerY, float radius, Color color)
{
    DrawCircle(static_cast<int>(centerX), static_cast<int>(centerY), radius, color);
}

void RayLibEnc::drawCircleLines(float centerX, float centerY, float radius, Color color)
{
    DrawCircleLines(static_cast<int>(centerX), static_cast<int>(centerY), radius, color);
}

float RayLibEnc::measureText(const std::string& text, float fontSize) const
{
    return static_cast<float>(MeasureText(text.c_str(), static_cast<int>(fontSize)));
}

void RayLibEnc::drawTextureRec(Texture2D texture, Rectangle source, Vector2 position,
    Color tint)
{
    DrawTextureRec(texture, source, position, tint);
}

void RayLibEnc::unloadTexture(Texture2D texture)
{
    UnloadTexture(texture);
}

bool RayLibEnc::checkCollisionPointRec(Vector2 point, Rectangle rec) const
{
    return CheckCollisionPointRec(point, rec);
}

void RayLibEnc::beginScissorMode(int x, int y, int width, int height)
{
    BeginScissorMode(x, y, width, height);
}

void RayLibEnc::endScissorMode()
{
    EndScissorMode();
}
