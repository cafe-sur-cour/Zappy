/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** CameraManager
*/

#include <cmath>
#include <algorithm>
#include "CameraManager.hpp"
#include "raylib.h"
#include "raymath.h"

CameraManager::CameraManager(std::shared_ptr<RayLib> raylib) : _raylib(raylib),
    _mapCenter({ 0.0f, 0.0f, 0.0f }), _mapWidth(0), _mapHeight(0),
    _targetDistance(30.0f), _targetAngleXZ(0.0f), _targetAngleY(0.5f), _isDragging(false),
    _playerId(-1), _playerAngleXZ(0.75f), _isPlayerViewDragging(false)
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

void CameraManager::setPlayerId(int playerId)
{
    _playerId = playerId;
}

int CameraManager::getPlayerId() const
{
    return _playerId;
}

void CameraManager::setGameInfos(std::shared_ptr<GameInfos> gameInfos)
{
    _gameInfos = gameInfos;
}

void CameraManager::handlePlayerCameraMouseInput()
{
    if (_raylib->isMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        _isPlayerViewDragging = true;

        int screenCenterX = _raylib->getScreenWidth() / 2;
        int screenCenterY = _raylib->getScreenHeight() / 2;

        _raylib->setMousePosition(screenCenterX, screenCenterY);
        _raylib->disableCursor();
    }

    if (_raylib->isMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
        _isPlayerViewDragging = false;
        _raylib->enableCursor();
    }

    if (_isPlayerViewDragging) {
        Vector2 mouseDelta = _raylib->getMouseDelta();
        const float rotationSensitivity = 0.005f;

        // Modifier uniquement l'angle horizontal (XZ)
        _playerAngleXZ += mouseDelta.x * rotationSensitivity;

        // Recentrer le curseur pour permettre une rotation continue
        int screenCenterX = _raylib->getScreenWidth() / 2;
        int screenCenterY = _raylib->getScreenHeight() / 2;
        _raylib->setMousePosition(screenCenterX, screenCenterY);
    }
}

Vector3 CameraManager::calculatePlayerPosition(const zappy::structs::Player& player)
{
    if (!_map) {
        // Fallback si Map n'est pas disponible
        const float basePlayerHeight = 0.2f;
        const float playerHeight = 0.5f;
        const float totalHeight = basePlayerHeight + playerHeight;
        
        return {
            static_cast<float>(player.x),
            totalHeight,
            static_cast<float>(player.y)
        };
    }
    
    int playerX = player.x;
    int playerY = player.y;
    
    // Trouver l'index du joueur sur sa case
    size_t playerIndex = 0;
    const auto& allPlayers = _gameInfos->getPlayers();
    size_t playerCount = 0;
    
    for (const auto& p : allPlayers) {
        if (p.x == playerX && p.y == playerY) {
            if (p.number == player.number) {
                playerIndex = playerCount;
            }
            playerCount++;
        }
    }
    
    // Obtenir la hauteur correcte en utilisant getOffset
    float playerHeightOffset = _map->getOffset(DisplayPriority::PLAYER, playerX, playerY, playerIndex);
    // Ajouter la moitié de la hauteur du joueur pour cibler le centre du joueur
    float playerEntityHeight = 0.4f; // Hauteur approximative du modèle du joueur
    float targetHeight = playerHeightOffset + playerEntityHeight / 2.0f;
    
    return {
        static_cast<float>(playerX),
        targetHeight,
        static_cast<float>(playerY)
    };
}

Vector3 CameraManager::calculateCameraPosition(const Vector3& playerPos, float angleXZ)
{
    const float fixedAngleY = 0.5f;
    const float fixedDistance = 6.0f;

    float posX = playerPos.x + fixedDistance * cosf(fixedAngleY) * cosf(angleXZ);
    float posY = playerPos.y + fixedDistance * sinf(fixedAngleY);
    float posZ = playerPos.z + fixedDistance * cosf(fixedAngleY) * sinf(angleXZ);

    return { posX, posY, posZ };
}

void CameraManager::updateCameraPlayerMode()
{
    if (!_gameInfos || _playerId < 0 || !_map) {
        updateCameraFreeMode();
        return;
    }

    const auto& players = _gameInfos->getPlayers();
    auto playerIt = std::find_if(players.begin(), players.end(),
        [this](const zappy::structs::Player& player) {
            return player.number == _playerId;
        });

    if (playerIt == players.end()) {
        updateCameraFreeMode();
        return;
    }

    handlePlayerCameraMouseInput();

    Vector3 playerPos = calculatePlayerPosition(*playerIt);
    Vector3 cameraPos = calculateCameraPosition(playerPos, _playerAngleXZ);

    _raylib->setCameraPosition(cameraPos);
    _raylib->setCameraTarget(playerPos);
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

void CameraManager::setMapInstance(std::shared_ptr<Map> map)
{
    _map = map;
}
