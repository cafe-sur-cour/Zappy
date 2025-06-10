/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Raylib3dModel
*/

#include <string>
#include "RayLib.hpp"
#include "raymath.h"

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
