/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GUI
*/

#include <cmath>
#include "GUI.hpp"
#include "../Utils/Constants.hpp"

GUI::GUI() : _isRunning(false)
{
    _raylib.initWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    _raylib.initCamera();
    _isRunning = _raylib.isWindowReady();
}

GUI::~GUI()
{
    _raylib.closeWindow();
}

void GUI::run()
{
    if (!_isRunning)
        return;

    while (!_raylib.windowShouldClose()) {
        updateCamera();

        _raylib.beginDrawing();
        _raylib.clearBackground(RAYWHITE);

        _raylib.begin3DMode();
        drawEnvironment();
        _raylib.end3DMode();
        _raylib.endDrawing();
    }
}

void GUI::updateCamera()
{
    static float angle = 0.0f;
    angle += 0.001f;
    float cameraX = 10.0f * cos(angle);
    float cameraZ = 10.0f * sin(angle);

    _raylib.setCameraPosition((Vector3){cameraX, 10.0f, cameraZ});
    _raylib.setCameraTarget((Vector3){0.0f, 0.0f, 0.0f});
    _raylib.updateCamera(CAMERA_FREE);
}

void GUI::drawEnvironment()
{
    _raylib.drawGrid(10, 1.0f);

    _raylib.drawCube((Vector3){0.0f, 0.0f, 0.0f}, 2.0f, 0.5f, 2.0f, RED);
    _raylib.drawCubeWires((Vector3){0.0f, 0.0f, 0.0f}, 2.0f, 0.5f, 2.0f, BLACK);

    _raylib.drawSphere((Vector3){-3.0f, 1.0f, -3.0f}, 0.5f, BLUE);
    _raylib.drawSphere((Vector3){3.0f, 1.0f, 3.0f}, 0.5f, GREEN);

    _raylib.drawCylinder((Vector3){-3.0f, 0.0f, 3.0f}, 0.5f, 0.5f, 2.0f, 8, PURPLE);

    _raylib.drawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){20.0f, 20.0f}, LIGHTGRAY);
}
