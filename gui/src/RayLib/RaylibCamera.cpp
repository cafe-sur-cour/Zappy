/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** RaylibCamera
*/

#include "RayLib.hpp"
#include "raymath.h"
#include "../Utils/Constants.hpp"

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
    float deltaTime = getFrameTime();
    float moveSpeed = zappy::gui::CAMERA_SPEED * deltaTime;
    float rotationSpeed = zappy::gui::CAMERA_SENSITIVITY;

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

    if (IsKeyDown(KEY_RIGHT)) {
        Vector3 viewDir = Vector3Subtract(_camera.target, _camera.position);
        float rotAngle = zappy::gui::CAMERA_ROTATE_SPEED_KEY * deltaTime;

        float cosRotAngle = cosf(rotAngle);
        float sinRotAngle = sinf(rotAngle);
        float newDirX = viewDir.x * cosRotAngle - viewDir.z * sinRotAngle;
        float newDirZ = viewDir.x * sinRotAngle + viewDir.z * cosRotAngle;
        viewDir.x = newDirX;
        viewDir.z = newDirZ;

        _camera.target = Vector3Add(_camera.position, viewDir);
    }

    if (IsKeyDown(KEY_LEFT)) {
        Vector3 viewDir = Vector3Subtract(_camera.target, _camera.position);
        float rotAngle = -zappy::gui::CAMERA_ROTATE_SPEED_KEY * deltaTime;

        float cosRotAngle = cosf(rotAngle);
        float sinRotAngle = sinf(rotAngle);
        float newDirX = viewDir.x * cosRotAngle - viewDir.z * sinRotAngle;
        float newDirZ = viewDir.x * sinRotAngle + viewDir.z * cosRotAngle;
        viewDir.x = newDirX;
        viewDir.z = newDirZ;

        _camera.target = Vector3Add(_camera.position, viewDir);
    }

    if (IsKeyDown(KEY_UP)) {
        Vector3 viewDir = Vector3Subtract(_camera.target, _camera.position);
        float rotAngle = zappy::gui::CAMERA_ROTATE_SPEED_KEY * deltaTime;

        Vector3 rightVec = Vector3CrossProduct(viewDir, _camera.up);
        rightVec = Vector3Normalize(rightVec);
        viewDir = Vector3RotateByAxisAngle(viewDir, rightVec, rotAngle);

        Vector3 newUp = Vector3CrossProduct(rightVec, viewDir);
        if (newUp.y > 0.0f)
            _camera.target = Vector3Add(_camera.position, viewDir);
    }

    if (IsKeyDown(KEY_DOWN)) {
        Vector3 viewDir = Vector3Subtract(_camera.target, _camera.position);
        float rotAngle = -zappy::gui::CAMERA_ROTATE_SPEED_KEY * deltaTime;

        Vector3 rightVec = Vector3CrossProduct(viewDir, _camera.up);
        rightVec = Vector3Normalize(rightVec);
        viewDir = Vector3RotateByAxisAngle(viewDir, rightVec, rotAngle);

        Vector3 newUp = Vector3CrossProduct(rightVec, viewDir);
        if (newUp.y > 0.0f)
            _camera.target = Vector3Add(_camera.position, viewDir);
    }
}

Camera3D RayLib::getCamera() const
{
    return _camera;
}
