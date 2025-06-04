/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** CameraManager
*/

#include <cmath>
#include "CameraManager.hpp"
#include "raylib.h"
#include "raymath.h"

CameraManager::CameraManager(std::shared_ptr<RayLib> raylib) : _raylib(raylib),
    _mapCenter({ 0.0f, 0.0f, 0.0f }), _mapWidth(0), _mapHeight(0),
    _targetDistance(30.0f), _targetAngleXZ(0.0f), _targetAngleY(0.5f), _isDragging(false)
{
}

CameraManager::~CameraManager()
{
}

float CameraManager::getCurrentCameraDistance() const
{
    Camera3D camera = _raylib->getCamera();

    return _raylib->vector3Distance(camera.position, camera.target);
}

void CameraManager::setTargetDistance(float distance)
{
    if (distance < 5.0f)
        distance = 5.0f;

    if (distance > 100.0f)
        distance = 100.0f;

    _targetDistance = distance;
}

void CameraManager::initTargetPositionFromCurrentCamera()
{
    Camera3D camera = _raylib->getCamera();

    float distance = _raylib->vector3Distance(camera.position, _mapCenter);
    Vector3 direction = _raylib->vector3Subtract(camera.position, _mapCenter);

    direction = _raylib->vector3Normalize(direction);
    _targetAngleY = asinf(direction.y);
    _targetAngleXZ = atan2f(direction.z, direction.x);
    _targetDistance = distance;
}

void CameraManager::updateCamera(zappy::gui::CameraMode mode)
{
    switch (mode) {
        case zappy::gui::CameraMode::FREE:
            updateCameraFreeMode();
            break;
        case zappy::gui::CameraMode::TARGETED:
            updateCameraTargetMode();
            break;
        case zappy::gui::CameraMode::PLAYER:
            updateCameraPlayerMode();
            break;
        default:
            updateCameraFreeMode();
            break;
    }
}

void CameraManager::updateCameraFreeMode()
{
    _raylib->updateCameraFreeMode();
}

void CameraManager::updateCameraTargetMode()
{
    if (_raylib->isMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        _isDragging = true;

        int screenCenterX = _raylib->getScreenWidth() / 2;
        int screenCenterY = _raylib->getScreenHeight() / 2;

        _raylib->setMousePosition(screenCenterX, screenCenterY);
        _raylib->disableCursor();
    }

    if (_raylib->isMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
        _isDragging = false;
        _raylib->enableCursor();
    }

    if (_isDragging) {
        Vector2 mouseDelta = _raylib->getMouseDelta();
        const float rotationSensitivity = 0.005f;

        _targetAngleXZ += mouseDelta.x * rotationSensitivity;
        _targetAngleY -= mouseDelta.y * rotationSensitivity;

        const float maxVerticalAngle = 1.5f;
        if (_targetAngleY > maxVerticalAngle) _targetAngleY = maxVerticalAngle;
        if (_targetAngleY < 0.1f) _targetAngleY = 0.1f;

        if (_isDragging) {
            int screenCenterX = _raylib->getScreenWidth() / 2;
            int screenCenterY = _raylib->getScreenHeight() / 2;
            _raylib->setMousePosition(screenCenterX, screenCenterY);
        }
    }

    float wheelMove = _raylib->getMouseWheelMove();
    if (wheelMove != 0) {
        _targetDistance -= wheelMove * 2.0f;

        if (_targetDistance < 5.0f) _targetDistance = 5.0f;
        if (_targetDistance > 100.0f) _targetDistance = 100.0f;
    }

    float posX = _mapCenter.x + _targetDistance * cosf(_targetAngleY) * cosf(_targetAngleXZ);
    float posY = _mapCenter.y + _targetDistance * sinf(_targetAngleY);
    float posZ = _mapCenter.z + _targetDistance * cosf(_targetAngleY) * sinf(_targetAngleXZ);

    _raylib->setCameraPosition({ posX, posY, posZ });
    _raylib->setCameraTarget(_mapCenter);
}

void CameraManager::updateCameraPlayerMode()
{
    updateCameraFreeMode();
}

void CameraManager::setMapCenter(const Vector3& center)
{
    _mapCenter = center;
}

void CameraManager::setMapSize(int width, int height)
{
    _mapWidth = width;
    _mapHeight = height;

    if (_targetDistance <= 0.0f)
        _targetDistance = std::max(_mapWidth, _mapHeight) * 1.5f;
}
