/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** RaylibSkybox
*/

#include "RayLibEnc.hpp"
#include <iostream>
#include <cmath>

bool RayLibEnc::loadSkybox(const std::string& id, const std::string& filepath)
{
    Texture2D texture = LoadTexture(filepath.c_str());
    if (texture.id == 0) {
        std::cout << "Failed to load skybox texture " << filepath << std::endl;
        return false;
    }

    Mesh cube = GenMeshCube(10000.0f, 10000.0f, 10000.0f);

    for (int i = 0; i < cube.vertexCount; i++) {
        Vector3 v = *(Vector3*)&cube.vertices[i*3];
        float length = std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
        if (length > 0) {
            v.x /= length;
            v.y /= length;
            v.z /= length;
        }

        cube.texcoords[i*2] = (v.x + 1.0f)/2.0f;
        cube.texcoords[i*2 + 1] = (v.y + 1.0f)/2.0f;
    }

    Model skybox = LoadModelFromMesh(cube);

    skybox.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    skybox.materials[0].shader = LoadShader(
        "gui/assets/shaders/skybox.vs",
        "gui/assets/shaders/skybox.fs"
    );

    ModelData modelData;
    modelData.model = skybox;
    modelData.animationCount = 0;
    modelData.center = {0.0f, 0.0f, 0.0f};

    _models[id] = modelData;
    _textures[id] = texture;
    return true;
}

void RayLibEnc::drawSkybox(const std::string& id)
{
    auto it = _models.find(id);
    if (it == _models.end()) {
        std::cout << "Skybox " << id << " not found" << std::endl;
        return;
    }

    Vector3 position = _camera.position;

    DrawModel(it->second.model, position, 1.0f, WHITE);
}
