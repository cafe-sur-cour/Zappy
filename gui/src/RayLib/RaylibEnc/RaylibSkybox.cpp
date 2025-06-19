/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** RaylibSkybox
*/

#include <iostream>
#include <cmath>
#include <string>
#include "RayLibEnc.hpp"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

bool RayLibEnc::loadSkybox(const std::string& id, const std::string& filepath)
{
    Mesh mesh = GenMeshSphere(1.0f, 64, 32);

    for (int i = 0; i < mesh.vertexCount * 3; i++) {
        mesh.normals[i] = -mesh.normals[i];
    }

    for (int i = 0; i < mesh.vertexCount; i++) {
        float x = mesh.vertices[3*i];
        float y = mesh.vertices[3*i + 1];
        float z = mesh.vertices[3*i + 2];

        float theta = atan2f(y, x);
        float phi = asinf(z);

        float u = (theta + PI) / (2.0f * PI);
        float v = (phi + PI/2.0f) / PI;

        mesh.texcoords[2*i] = u;
        mesh.texcoords[2*i + 1] = 1.0f - v;
    }

    Model skyboxSphere = LoadModelFromMesh(mesh);
    Texture texture = LoadTexture(filepath.c_str());

    if (texture.id == 0) {
        std::cout << "Warning: Could not load skybox texture, using default" << std::endl;
        Image img = GenImageGradientRadial(512, 512, 0.0f, SKYBLUE, BLUE);
        texture = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    skyboxSphere.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    SetTextureWrap(texture, TEXTURE_WRAP_CLAMP);
    skyboxSphere.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = WHITE;

    ModelData skyboxData;
    skyboxData.model = skyboxSphere;
    skyboxData.animationCount = 0;
    skyboxData.center = {0.0f, 0.0f, 0.0f};

    _models[id] = skyboxData;
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

    float timeRotation = GetTime() * 2.5f;
    rlDisableBackfaceCulling();

    Matrix rotationX = MatrixRotateX(90.0f * DEG2RAD);
    Matrix rotationY = MatrixRotateY(timeRotation * DEG2RAD);
    Matrix finalRotation = MatrixMultiply(rotationX, rotationY);

    Vector3 axis;
    float angle;
    QuaternionToAxisAngle(QuaternionFromMatrix(finalRotation), &axis, &angle);

    DrawModelEx(it->second.model, position,
        axis, angle * RAD2DEG, {500.0f, 500.0f, 500.0f}, WHITE);
    rlEnableBackfaceCulling();
}
