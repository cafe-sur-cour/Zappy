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

bool RayLib::isMouseButtonDown(int button) const
{
    return IsMouseButtonDown(button);
}

bool RayLib::isMouseButtonPressed(int button) const
{
    return IsMouseButtonPressed(button);
}

bool RayLib::isMouseButtonReleased(int button) const
{
    return IsMouseButtonReleased(button);
}

bool RayLib::isKeyDown(int key) const
{
    return IsKeyDown(key);
}

bool RayLib::isKeyPressed(int key) const
{
    return IsKeyPressed(key);
}

bool RayLib::isKeyReleased(int key) const
{
    return IsKeyReleased(key);
}

Vector2 RayLib::getMousePosition() const
{
    return GetMousePosition();
}

void RayLib::setMousePosition(int x, int y)
{
    SetMousePosition(x, y);
}

void RayLib::disableCursor()
{
    if (!_isCursorLocked) {
        DisableCursor();
        _isCursorLocked = true;
    }
}

void RayLib::enableCursor()
{
    if (_isCursorLocked) {
        EnableCursor();
        _isCursorLocked = false;
    }
}

int RayLib::getScreenWidth() const
{
    return GetScreenWidth();
}

int RayLib::getScreenHeight() const
{
    return GetScreenHeight();
}

Vector2 RayLib::getMouseDelta()
{
    if (_isCursorLocked) {
        return GetMouseDelta();
    } else {
        Vector2 currentMousePosition = GetMousePosition();
        Vector2 delta = { currentMousePosition.x - _previousMousePosition.x,
            currentMousePosition.y - _previousMousePosition.y };

        this->_previousMousePosition = currentMousePosition;
        return delta;
    }
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
    _camera.position = (Vector3){ 0.0f, 30.0f, 30.0f };
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

void RayLib::updateCameraFreeMode()
{
    const float moveSpeed = 0.025f;
    const float rotationSpeed = 0.001f;

    Vector3 forward = Vector3Subtract(_camera.target, _camera.position);
    forward = Vector3Normalize(forward);

    Vector3 right = Vector3CrossProduct(forward, _camera.up);
    right = Vector3Normalize(right);

    if (IsKeyDown(KEY_Z) || IsKeyDown(KEY_W)) {
        _camera.position = Vector3Add(_camera.position,
                                    Vector3Scale(forward, moveSpeed));
        _camera.target = Vector3Add(_camera.target, Vector3Scale(forward, moveSpeed));
    }
    if (IsKeyDown(KEY_S)) {
        _camera.position = Vector3Subtract(_camera.position,
                                        Vector3Scale(forward, moveSpeed));
        _camera.target = Vector3Subtract(_camera.target, Vector3Scale(forward, moveSpeed));
    }
    if (IsKeyDown(KEY_Q) || IsKeyDown(KEY_A)) {
        _camera.position = Vector3Subtract(_camera.position,
                                        Vector3Scale(right, moveSpeed));
        _camera.target = Vector3Subtract(_camera.target, Vector3Scale(right, moveSpeed));
    }
    if (IsKeyDown(KEY_D)) {
        _camera.position = Vector3Add(_camera.position, Vector3Scale(right, moveSpeed));
        _camera.target = Vector3Add(_camera.target, Vector3Scale(right, moveSpeed));
    }
    if (IsKeyDown(KEY_SPACE)) {
        _camera.position.y += moveSpeed * 0.5;
        _camera.target.y += moveSpeed * 0.5;
    }
    if (IsKeyDown(KEY_LEFT_CONTROL)) {
        _camera.position.y -= moveSpeed * 0.5;
        _camera.target.y -= moveSpeed * 0.5;
    }

    if (isMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        int screenCenterX = getScreenWidth() / 2;
        int screenCenterY = getScreenHeight() / 2;

        setMousePosition(screenCenterX, screenCenterY);
        _previousMousePosition = Vector2{ static_cast<float>(screenCenterX),
            static_cast<float>(screenCenterY) };
        disableCursor();
    }

    if (isMouseButtonReleased(MOUSE_BUTTON_RIGHT))
        enableCursor();

    if (isMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 mouseDelta = getMouseDelta();

        if (mouseDelta.x != 0) {
            Vector3 viewDir = Vector3Subtract(_camera.target, _camera.position);
            float rotAngle = mouseDelta.x * rotationSpeed;

            float cosRotAngle = cosf(rotAngle);
            float sinRotAngle = sinf(rotAngle);
            float newDirX = viewDir.x * cosRotAngle - viewDir.z * sinRotAngle;
            float newDirZ = viewDir.x * sinRotAngle + viewDir.z * cosRotAngle;
            viewDir.x = newDirX;
            viewDir.z = newDirZ;

            _camera.target = Vector3Add(_camera.position, viewDir);
        }

        if (mouseDelta.y != 0) {
            Vector3 viewDir = Vector3Subtract(_camera.target, _camera.position);
            float rotAngle = -mouseDelta.y * rotationSpeed;

            Vector3 rightVec = Vector3CrossProduct(viewDir, _camera.up);
            rightVec = Vector3Normalize(rightVec);
            viewDir = Vector3RotateByAxisAngle(viewDir, rightVec, rotAngle);

            Vector3 newUp = Vector3CrossProduct(rightVec, viewDir);
            if (newUp.y > 0.0f)
                _camera.target = Vector3Add(_camera.position, viewDir);
        }

        if (_isCursorLocked) {
            int screenCenterX = getScreenWidth() / 2;
            int screenCenterY = getScreenHeight() / 2;
            setMousePosition(screenCenterX, screenCenterY);
        }
    }
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
