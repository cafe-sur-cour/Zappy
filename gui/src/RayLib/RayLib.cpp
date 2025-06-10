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

void RayLib::drawGrid(int slices, float spacing)
{
    DrawGrid(slices, spacing);
}

void RayLib::drawCube(Vector3 position, float width, float height, float length, Color color)
{
    DrawCube(position, width, height, length, color);
}

void RayLib::drawCubeWires(Vector3 position, float width, float height, float length,
    Color color)
{
    DrawCubeWires(position, width, height, length, color);
}

void RayLib::drawSphere(Vector3 position, float radius, Color color)
{
    DrawSphere(position, radius, color);
}

void RayLib::drawSphereWires(Vector3 position, float radius, int rings,
    int slices, Color color)
{
    DrawSphereWires(position, radius, rings, slices, color);
}

void RayLib::drawCylinder(Vector3 position, float radiusTop, float radiusBottom,
    float height, int slices, Color color)
{
    DrawCylinder(position, radiusTop, radiusBottom, height, slices, color);
}

void RayLib::drawCylinderWires(Vector3 position, float radiusTop, float radiusBottom,
    float height, int slices, Color color)
{
    DrawCylinderWires(position, radiusTop, radiusBottom, height, slices, color);
}

void RayLib::drawCylinderEx(Vector3 startPos, Vector3 endPos, float startRadius,
    float endRadius, int sides, Color color)
{
    DrawCylinderEx(startPos, endPos, startRadius, endRadius, sides, color);
}

void RayLib::drawPlane(Vector3 position, Vector2 size, Color color)
{
    DrawPlane(position, size, color);
}

void RayLib::drawLine3D(Vector3 startPos, Vector3 endPos, Color color)
{
    DrawLine3D(startPos, endPos, color);
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

void RayLib::initAudioDevice()
{
    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        std::cerr << "Failed to initialize audio device." << std::endl;
        return;
    }
}

void RayLib::closeAudioDevice()
{
    CloseAudioDevice();
}

bool RayLib::isAudioDeviceReady() const
{
    return IsAudioDeviceReady();
}

bool RayLib::loadModel(const std::string& id, const std::string& filepath, Vector3 center)
{
    if (_models.find(id) != _models.end())
        unloadModel(id);

    ModelData modelData;
    modelData.model = LoadModel(filepath.c_str());
    if (!modelData.model.meshCount)
        return false;
    modelData.center = center;

    _models[id] = modelData;
    return true;
}

void RayLib::drawModel(const std::string& id, Vector3 position, float scale, Color tint)
{
    auto it = _models.find(id);
    if (it != _models.end()) {
        Vector3 finalPosition = {
            position.x - it->second.center.x * scale,
            position.y - it->second.center.y * scale,
            position.z - it->second.center.z * scale
        };
        DrawModel(it->second.model, finalPosition, scale, tint);
    }
}

void RayLib::drawModelEx(const std::string& id, Vector3 position, Vector3 rotationAxis,
                      float rotationAngle, Vector3 scale, Color tint)
{
    auto it = _models.find(id);
    if (it != _models.end()) {
        Matrix rotation = MatrixRotate(rotationAxis, rotationAngle * DEG2RAD);
        Vector3 rotatedCenter;
        Vector3 center = {
            it->second.center.x * scale.x,
            it->second.center.y * scale.y,
            it->second.center.z * scale.z
        };

        rotatedCenter.x = center.x * rotation.m0 + center.y *
            rotation.m4 + center.z * rotation.m8;
        rotatedCenter.y = center.x * rotation.m1 + center.y *
            rotation.m5 + center.z * rotation.m9;
        rotatedCenter.z = center.x * rotation.m2 + center.y *
            rotation.m6 + center.z * rotation.m10;

        Vector3 finalPosition = {
            position.x - rotatedCenter.x,
            position.y - rotatedCenter.y,
            position.z - rotatedCenter.z
        };

        DrawModelEx(it->second.model, finalPosition, rotationAxis, rotationAngle, scale, tint);
    }
}

void RayLib::drawModelWires(const std::string& id, Vector3 position, float scale, Color tint)
{
    auto it = _models.find(id);
    if (it != _models.end()) {
        Vector3 finalPosition = {
            position.x - it->second.center.x * scale,
            position.y - it->second.center.y * scale,
            position.z - it->second.center.z * scale
        };
        DrawModelWires(it->second.model, finalPosition, scale, tint);
    }
}

void RayLib::drawModelWiresEx(const std::string& id, Vector3 position, Vector3 rotationAxis,
                           float rotationAngle, Vector3 scale, Color tint)
{
    auto it = _models.find(id);
    if (it != _models.end()) {
        Matrix rotation = MatrixRotate(rotationAxis, rotationAngle * DEG2RAD);
        Vector3 rotatedCenter;
        Vector3 center = {
            it->second.center.x * scale.x,
            it->second.center.y * scale.y,
            it->second.center.z * scale.z
        };

        rotatedCenter.x = center.x * rotation.m0 + center.y *
            rotation.m4 + center.z * rotation.m8;
        rotatedCenter.y = center.x * rotation.m1 + center.y *
            rotation.m5 + center.z * rotation.m9;
        rotatedCenter.z = center.x * rotation.m2 + center.y *
            rotation.m6 + center.z * rotation.m10;

        Vector3 finalPosition = {
            position.x - rotatedCenter.x,
            position.y - rotatedCenter.y,
            position.z - rotatedCenter.z
        };

        DrawModelWiresEx(it->second.model, finalPosition, rotationAxis,
            rotationAngle, scale, tint);
    }
}

void RayLib::unloadModel(const std::string& id)
{
    auto it = _models.find(id);
    if (it != _models.end()) {
        UnloadModel(it->second.model);
        _models.erase(it);
    }
}

void RayLib::unloadAllModels()
{
    for (auto& pair : _models) {
        UnloadModel(pair.second.model);
    }

    _models.clear();
}

bool RayLib::modelExists(const std::string& id) const
{
    return _models.find(id) != _models.end();
}
