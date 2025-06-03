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
#include "RayLib.hpp"
#include "raylib.h"

RayLib::RayLib() : _isInitialized(false)
{
    _camera = {{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
        0.0f, CAMERA_PERSPECTIVE };
}

RayLib::~RayLib()
{
    if (_isInitialized)
        closeWindow();
}

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

void RayLib::begin3DMode()
{
    BeginMode3D(_camera);
}

void RayLib::end3DMode()
{
    EndMode3D();
}

void RayLib::initCamera()
{
    _camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
    _camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    _camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    _camera.fovy = 45.0f;
    _camera.projection = CAMERA_PERSPECTIVE;
}

void RayLib::setCameraPosition(Vector3 position)
{
    _camera.position = position;
}

void RayLib::setCameraTarget(Vector3 target)
{
    _camera.target = target;
}

void RayLib::setCameraUp(Vector3 up)
{
    _camera.up = up;
}

void RayLib::setCameraFovy(float fovy)
{
    _camera.fovy = fovy;
}

void RayLib::setCameraProjection(int projection)
{
    _camera.projection = projection;
}

void RayLib::updateCamera(int mode)
{
    UpdateCamera(&_camera, mode);
}

Camera3D RayLib::getCamera() const
{
    return _camera;
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

void RayLib::drawCylinder(Vector3 position, float radiusTop, float radiusBottom,
    float height, int slices, Color color)
{
    DrawCylinder(position, radiusTop, radiusBottom, height, slices, color);
}

void RayLib::drawPlane(Vector3 position, Vector2 size, Color color)
{
    DrawPlane(position, size, color);
}
