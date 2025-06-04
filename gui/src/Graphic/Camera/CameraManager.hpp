/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** CameraManager
*/

#ifndef CAMERA_MANAGER_HPP_
#define CAMERA_MANAGER_HPP_

#include <memory>
#include "../RayLib/RayLib.hpp"
#include "../../Utils/Constants.hpp"

class CameraManager {
    public:
        CameraManager(std::shared_ptr<RayLib> raylib);
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

    private:
        std::shared_ptr<RayLib> _raylib;
        Vector3 _mapCenter;
        int _mapWidth;
        int _mapHeight;

        float _targetDistance;
        float _targetAngleXZ;
        float _targetAngleY;
        bool _isDragging;
};

#endif /* !CAMERA_MANAGER_HPP_ */
