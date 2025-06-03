/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** RayLib
*/

#ifndef RAYLIB_HPP_
#define RAYLIB_HPP_

#include <string>
#include "raylib.h"

class RayLib {
    public:
        RayLib();
        ~RayLib();

        // Window management methods
        void initWindow(int width, int height, const std::string &title);
        void closeWindow();
        bool windowShouldClose() const;
        void beginDrawing();
        void endDrawing();
        void clearBackground(Color color = WHITE);
        bool isWindowReady() const;

        // 3D Environment methods
        void begin3DMode();
        void end3DMode();

        // Camera methods
        void initCamera();
        void setCameraPosition(Vector3 position);
        void setCameraTarget(Vector3 target);
        void setCameraUp(Vector3 up);
        void setCameraFovy(float fovy);
        void setCameraProjection(int projection);
        void updateCamera(int mode = CAMERA_FREE);
        Camera3D getCamera() const;

        // 3D Drawing methods
        void drawGrid(int slices, float spacing);
        void drawCube(Vector3 position, float width, float height, float length, Color color);
        void drawCubeWires(Vector3 position, float width, float height, float length, Color color);
        void drawSphere(Vector3 position, float radius, Color color);
        void drawCylinder(Vector3 position, float radiusTop, float radiusBottom, float height, int slices, Color color);
        void drawPlane(Vector3 position, Vector2 size, Color color);

    protected:
    private:
        bool _isInitialized;
        Camera3D _camera;
};

#endif /* !RAYLIB_HPP_ */
