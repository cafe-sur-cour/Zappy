/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** RayLib
*/

#ifndef RAYLIB_HPP_
#define RAYLIB_HPP_

#include <string>
#include <map>
#include <memory>
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
        int getMonitorWidth(int monitor) const;
        int getMonitorHeight(int monitor) const;
        void waitTime(float seconds) const;
        void setTargetFPS(int fps) const;
        int getFPS() const;
        float getFrameTime() const;

        // Collision methods
        bool checkCollisionPointRec(Vector2 point, Rectangle rec) const;

        // Texture methods
        void drawTextureRec(Texture2D texture, Rectangle source, Vector2 position, Color tint);
        void unloadTexture(Texture2D texture);

        // Input methods
        bool isMouseButtonDown(int button) const;
        bool isMouseButtonPressed(int button) const;
        bool isMouseButtonReleased(int button) const;
        bool isKeyDown(int key) const;
        bool isKeyPressed(int key) const;
        bool isKeyReleased(int key) const;
        Vector2 getMouseDelta();
        Vector2 getMousePosition() const;
        void setMousePosition(int x, int y);
        void disableCursor();
        void enableCursor();
        int getScreenWidth() const;
        int getScreenHeight() const;
        float getMouseWheelMove() const;

        // Scissor mode methods for clipping
        void beginScissorMode(int x, int y, int width, int height);
        void endScissorMode();

        // 3D Environment methods
        void begin3DMode();
        void end3DMode();
        float vector3Distance(Vector3 v1, Vector3 v2) const;
        Vector3 vector3Normalize(Vector3 v) const;
        Vector3 vector3Subtract(Vector3 v1, Vector3 v2) const;
        Vector3 vector3Add(Vector3 v1, Vector3 v2) const;

        // Camera methods
        void initCamera();
        void setCameraPosition(Vector3 position);
        void setCameraTarget(Vector3 target);
        void setCameraUp(Vector3 up);
        void setCameraFovy(float fovy);
        void setCameraProjection(int projection);
        void updateCamera(int mode = CAMERA_FREE);
        void updateCameraFreeMode();
        Camera3D getCamera() const;

        // 3D Drawing methods
        void drawGrid(int slices, float spacing);
        void drawCube(Vector3 position, float width, float height, float length, Color color);
        void drawCubeWires(Vector3 position, float width, float height, float length, Color color);
        void drawSphere(Vector3 position, float radius, Color color);
        void drawSphereWires(Vector3 position, float radius, int rings, int slices, Color color);
        void drawCylinder(Vector3 position, float radiusTop, float radiusBottom, float height, int slices, Color color);
        void drawCylinderWires(Vector3 position, float radiusTop, float radiusBottom, float height, int slices, Color color);
        void drawCylinderEx(Vector3 startPos, Vector3 endPos, float startRadius, float endRadius, int sides, Color color);
        void drawPlane(Vector3 position, Vector2 size, Color color);
        void drawLine3D(Vector3 startPos, Vector3 endPos, Color color);

        // 3D Model methods
        bool loadModel(const std::string& id, const std::string& filepath, Vector3 center = {0.0f, 0.0f, 0.0f});
        void drawModel(const std::string& id, Vector3 position, float scale, Color tint = WHITE);
        void drawModelEx(const std::string& id, Vector3 position, Vector3 rotationAxis,
                         float rotationAngle, Vector3 scale, Color tint = WHITE);
        void drawModelWires(const std::string& id, Vector3 position, float scale, Color tint = WHITE);
        void drawModelWiresEx(const std::string& id, Vector3 position, Vector3 rotationAxis,
                              float rotationAngle, Vector3 scale, Color tint = WHITE);
        void unloadModel(const std::string& id);
        void unloadAllModels();
        bool modelExists(const std::string& id) const;

        // 2D Drawing methods
        void drawRectangleRec(Rectangle rec, Color color);
        void drawText(const std::string& text, float x, float y, float fontSize, Color color);
        float measureText(const std::string& text, float fontSize) const;

        // Audio methods
        void initAudioDevice();
        void closeAudioDevice();
        bool isAudioDeviceReady() const;

    protected:
    private:
        bool _isInitialized;
        Camera3D _camera;
        Vector2 _previousMousePosition;
        bool _isCursorLocked;

        struct ModelData {
            Model model;
            unsigned int animationCount;
            Vector3 center;
        };

        std::map<std::string, ModelData> _models;
        std::map<std::string, Sound> _sounds;
        std::map<std::string, Music> _musics;
};

#endif /* !RAYLIB_HPP_ */
