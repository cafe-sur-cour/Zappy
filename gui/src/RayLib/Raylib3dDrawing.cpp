/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Raylib3dDrawing
*/

#include "RayLib.hpp"

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
