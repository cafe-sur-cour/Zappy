/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** CameraManager
*/

#ifndef CAMERA_MANAGER_HPP_
#define CAMERA_MANAGER_HPP_

#include <memory>
#include "../../RayLib/RayLib.hpp"
#include "../../Utils/Constants.hpp"
#include "../../Game/GameInfos.hpp"
#include "../Map.hpp"

class CameraManager {
    public:
        explicit CameraManager(std::shared_ptr<RayLib> raylib);
        ~CameraManager();

        void updateCamera(zappy::gui::CameraMode mode);
        void updateCameraFreeMode();
        void updateCameraTargetMode();
        void updateCameraPlayerMode();

        void setMapCenter(const Vector3& center);
        void setMapSize(int width, int height);

        float getCurrentCameraDistance() const;
        void setTargetDistance(float distance);
        void initTargetPositionFromCurrentCamera();

        void setPlayerId(int playerId);
        int getPlayerId() const;
        void setGameInfos(std::shared_ptr<GameInfos> gameInfos);
        void setMapInstance(std::shared_ptr<Map> map);

    private:
        std::shared_ptr<RayLib> _raylib;
        std::shared_ptr<GameInfos> _gameInfos;
        std::shared_ptr<Map> _map;
        Vector3 _mapCenter;
        int _mapWidth;
        int _mapHeight;

        float _targetDistance;
        float _targetAngleXZ;
        float _targetAngleY;
        bool _isDragging;
        int _playerId;

        float _playerAngleXZ;
        bool _isPlayerViewDragging;

        void handlePlayerCameraMouseInput();
        Vector3 calculatePlayerPosition(const zappy::structs::Player& player);
        Vector3 calculateCameraPosition(const Vector3& playerPos, float angleXZ);
};

#endif /* !CAMERA_MANAGER_HPP_ */
