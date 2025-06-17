/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Camera_manager
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "../../../../gui/src/Graphic/Camera/CameraManager.hpp"
#include "../../../../gui/src/Utils/Constants.hpp"

// Mock class for IDisplay
class MockDisplay : public IDisplay {
public:
    MOCK_METHOD(float, getFrameTime, (), (override));
    MOCK_METHOD(void, updateCameraFreeMode, (float cameraMovingSpeed, float cameraRotatingSpeed), (override));
    MOCK_METHOD(bool, isMouseButtonPressed, (int button), (override));
    MOCK_METHOD(bool, isMouseButtonReleased, (int button), (override));
    MOCK_METHOD(int, getKeyId, (Key key), (override));
    MOCK_METHOD(Vector2i, getScreenSize, (), (override));
    MOCK_METHOD(void, setMousePosition, (Vector2f position), (override));
    MOCK_METHOD(void, disableCursor, (), (override));
    MOCK_METHOD(void, enableCursor, (), (override));
    MOCK_METHOD(Vector2f, getMouseDelta, (), (override));
    MOCK_METHOD(float, getMouseWheelMove, (), (override));
    MOCK_METHOD(bool, isGamepadButtonDown, (int button), (override));
    MOCK_METHOD(bool, isKeyDown, (int key), (override));
    MOCK_METHOD(bool, isGamepadAvailable, (), (override));
    MOCK_METHOD(float, getGamepadAxisMovement, (int axis), (override));
    MOCK_METHOD(void, setCameraPosition, (Vector3f position), (override));
    MOCK_METHOD(void, setCameraTarget, (Vector3f target), (override));
    MOCK_METHOD(float, vector3DDistanceFromCamera, (Vector3f position), (override));
    MOCK_METHOD(Vector3f, vector3SubtractFromCamera, (Vector3f position), (override));
    MOCK_METHOD(Vector3f, vector3Normalize, (Vector3f v), (override));

    MOCK_METHOD(Vector2i, getMonitorSize, (), (override));
    MOCK_METHOD(void, initWindow, (int width, int height, std::string title), (override));
    MOCK_METHOD(void, initCamera, (), (override));
    MOCK_METHOD(bool, isWindowReady, (), (override));
    MOCK_METHOD(void, setTargetFPS, (unsigned int FPS), (override));
    MOCK_METHOD(bool, isOpen, (), (override));
    MOCK_METHOD(void, closeWindow, (), (override));
    MOCK_METHOD(bool, isKeyReleased, (int key), (override));
    MOCK_METHOD(bool, isKeyPressed, (int key), (override));
    MOCK_METHOD(bool, isGamepadButtonReleased, (int key), (override));
    MOCK_METHOD(bool, isGamepadButtonPressed, (int key), (override));
    MOCK_METHOD(bool, isMouseButtonDown, (int key), (override));
    MOCK_METHOD(Vector2f, getMousePosition, (), (override));
    MOCK_METHOD(float, measureText, (const std::string& text, float fontSize), (const, override));
    MOCK_METHOD(bool, checkCollisionPointRec, (Vector2f point, FloatRect rec), (override));
    MOCK_METHOD(Ray3D, getMouseRay, (Vector2f mousePosition), (override));
    MOCK_METHOD(RayCollision3D, getRayCollisionBox, (Ray3D ray, BoundingBox3D box), (override));
    MOCK_METHOD(RayCollision3D, getRayCollisionSphere, (Ray3D ray, Vector3f center, float radius), (override));
    MOCK_METHOD(bool, checkCollisionBoxes, (BoundingBox3D box1, BoundingBox3D box2), (override));
    MOCK_METHOD(Ray3D, getMouseRayFromCurrent, (), (override));
    MOCK_METHOD(BoundingBox3D, createBoundingBox, (Vector3f center, Vector3f size), (override));
    MOCK_METHOD(BoundingBox3D, createBoundingBoxFromMinMax, (Vector3f min, Vector3f max), (override));
    MOCK_METHOD(void, beginDrawing, (), (override));
    MOCK_METHOD(void, endDrawing, (), (override));
    MOCK_METHOD(void, clearBackground, (Color32 color), (override));
    MOCK_METHOD(void, begin3DMode, (), (override));
    MOCK_METHOD(void, end3DMode, (), (override));
    MOCK_METHOD(void, endScissorMode, (), (override));
    MOCK_METHOD(void, beginScissorMode, (IntRect rect), (override));
    MOCK_METHOD(bool, loadModel, (const std::string& id, const std::string& filepath, Vector3f position), (override));
    MOCK_METHOD(void, drawCube, (Vector3f position, float width, float height, float length, Color32 color), (override));
    MOCK_METHOD(void, drawCubeWires, (Vector3f position, float width, float height, float length, Color32 color), (override));
    MOCK_METHOD(void, drawSphere, (Vector3f position, float radius, Color32 color), (override));
    MOCK_METHOD(void, drawSphereWires, (Vector3f position, float radius, int rings, int slices, Color32 color), (override));
    MOCK_METHOD(void, drawCylinder, (Vector3f position, float radiusTop, float radiusBottom, float height, int slices, Color32 color), (override));
    MOCK_METHOD(void, drawCylinderWires, (Vector3f position, float radiusTop, float radiusBottom, float height, int slices, Color32 color), (override));
    MOCK_METHOD(void, drawCylinderEx, (Vector3f startPos, Vector3f endPos, float startRadius, float endRadius, int sides, Color32 color), (override));
    MOCK_METHOD(void, drawPlane, (Vector3f position, Vector2f size, Color32 color), (override));
    MOCK_METHOD(void, drawLine3D, (Vector3f startPos, Vector3f endPos, Color32 color), (override));
    MOCK_METHOD(void, drawModelEx, (const std::string& id, Vector3f position, Vector3f rotationAxis, float rotationAngle, Vector3f scale, Color32 tint), (override));
    MOCK_METHOD(void, drawCircle, (float centerX, float centerY, float radius, Color32 color), (override));
    MOCK_METHOD(void, drawCircleLines, (float centerX, float centerY, float radius, Color32 color), (override));
    MOCK_METHOD(void, drawText, (const std::string& text, float x, float y, float fontSize, Color32 color), (override));
    MOCK_METHOD(void, drawTextEx, (const std::string& text, float x, float y, float fontSize, float spacing, Color32 tint), (override));
    MOCK_METHOD(void, drawRectangleRec, (FloatRect rec, Color32 color), (override));
    MOCK_METHOD(bool, loadTexture, (const std::string& id, const std::string& filepath), (override));
    MOCK_METHOD(bool, loadFont, (const std::string& id, const std::string& filepath), (override));
    MOCK_METHOD(void, drawTexture, (const std::string& id, float x, float y, Color32 tint), (override));
    MOCK_METHOD(void, drawTextureScaled, (const std::string& id, float x, float y, float width, float height, Color32 tint), (override));
    MOCK_METHOD(Vector2f, getTextureSize, (const std::string& id), (const, override));
    MOCK_METHOD(bool, loadSkybox, (const std::string& id, const std::string& filepath), (override));
    MOCK_METHOD(void, drawSkybox, (const std::string& id), (override));
    MOCK_METHOD(void, drawSimpleSkybox, (), (override));
};

// Mock class for ICommunication - needed for GameInfos
class MockICommunication : public ICommunication {
public:
    MOCK_METHOD(void, sendMessage, (const std::string& message), (override));
    MOCK_METHOD(bool, hasMessages, (), (const, override));
    MOCK_METHOD(std::string, popMessage, (), (override));
    MOCK_METHOD(bool, isConnected, (), (const, override));
    MOCK_METHOD(void, disconnect, (), (override));
};

// Mock class for GameInfos
class MockGameInfos : public GameInfos {
public:
    explicit MockGameInfos(std::shared_ptr<ICommunication> communication) : GameInfos(communication) {}

    MOCK_METHOD(const std::vector<zappy::structs::Player>, getPlayers, ());
};

// Mock class for Map with public methods we need to test
class MockMap : public Map {
public:
    explicit MockMap(std::shared_ptr<GameInfos> gameInfos, std::shared_ptr<IDisplay> display)
        : Map(gameInfos, display) {}

    MOCK_METHOD(Vector3f, getPlayerInterpolatedPosition, (int playerId, int serverX, int serverY));
    MOCK_METHOD(float, getOffset, (DisplayPriority priority, int x, int y, size_t stackIndex));
};

class CameraManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockDisplay = std::make_shared<MockDisplay>();
        cameraManager = std::make_unique<CameraManager>(mockDisplay);
    }

    void TearDown() override {
        cameraManager.reset();
        mockDisplay.reset();
    }

    std::unique_ptr<CameraManager> cameraManager;
    std::shared_ptr<MockDisplay> mockDisplay;
};

// Test le constructeur pour vérifier que les valeurs initiales sont correctes
TEST_F(CameraManagerTest, Constructor) {
    EXPECT_FLOAT_EQ(cameraManager->getCameraMovingSpeed(), 7.5f);
    EXPECT_FLOAT_EQ(cameraManager->getCameraRotaSpeed(), 2.0f);
    EXPECT_FLOAT_EQ(cameraManager->getCameraZoomSpeed(), 120.0f);
    EXPECT_EQ(cameraManager->getPlayerId(), -1);
}

// Test les setters et getters pour les vitesses de caméra
TEST_F(CameraManagerTest, SpeedSettersAndGetters) {
    cameraManager->setCameraMovingSpeed(10.0f);
    EXPECT_FLOAT_EQ(cameraManager->getCameraMovingSpeed(), 10.0f);

    cameraManager->setCameraRotaSpeed(5.0f);
    EXPECT_FLOAT_EQ(cameraManager->getCameraRotaSpeed(), 5.0f);

    cameraManager->setCameraZoomSpeed(200.0f);
    EXPECT_FLOAT_EQ(cameraManager->getCameraZoomSpeed(), 200.0f);
}

// Test la fonction setPlayerId et getPlayerId
TEST_F(CameraManagerTest, PlayerIdSetterAndGetter) {
    int playerId = 42;
    cameraManager->setPlayerId(playerId);
    EXPECT_EQ(cameraManager->getPlayerId(), playerId);
}

// Test la fonction setTargetDistance
TEST_F(CameraManagerTest, SetTargetDistance) {
    cameraManager->setTargetDistance(50.0f);

    cameraManager->setTargetDistance(2.0f);

    cameraManager->setTargetDistance(150.0f);
}

// Test la fonction updateCamera pour différents modes
TEST_F(CameraManagerTest, UpdateCamera) {
    EXPECT_CALL(*mockDisplay, updateCameraFreeMode(testing::_, testing::_)).Times(2);

    cameraManager->updateCamera(zappy::gui::CameraMode::FREE);

    cameraManager->updateCamera(static_cast<zappy::gui::CameraMode>(99));
}

// Test la fonction updateCamera pour le mode TARGETED
TEST_F(CameraManagerTest, UpdateCameraTargetMode) {
    Vector2i screenSize = {1920, 1080};
    float frameTime = 0.016f;

    EXPECT_CALL(*mockDisplay, getFrameTime())
        .WillRepeatedly(testing::Return(frameTime));
    EXPECT_CALL(*mockDisplay, getScreenSize())
        .WillRepeatedly(testing::Return(screenSize));
    EXPECT_CALL(*mockDisplay, isMouseButtonPressed(testing::_))
        .WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isMouseButtonReleased(testing::_))
        .WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, getMouseWheelMove())
        .WillOnce(testing::Return(0.0f));
    EXPECT_CALL(*mockDisplay, setCameraPosition(testing::_)).Times(1);
    EXPECT_CALL(*mockDisplay, setCameraTarget(testing::_)).Times(1);

    cameraManager->updateCamera(zappy::gui::CameraMode::TARGETED);
}

// Test la fonction updateCamera pour le mode PLAYER
TEST_F(CameraManagerTest, UpdateCameraPlayerMode) {
    auto mockCommunication = std::make_shared<MockICommunication>();
    auto mockGameInfos = std::make_shared<MockGameInfos>(mockCommunication);
    auto mockMap = std::make_shared<MockMap>(mockGameInfos, mockDisplay);

    EXPECT_CALL(*mockDisplay, updateCameraFreeMode(testing::_, testing::_)).Times(1);

    cameraManager->updateCamera(zappy::gui::CameraMode::PLAYER);
}

// Test la fonction updateCameraFreeMode
TEST_F(CameraManagerTest, UpdateCameraFreeMode) {
    float movingSpeed = cameraManager->getCameraMovingSpeed();
    float rotaSpeed = cameraManager->getCameraRotaSpeed();

    EXPECT_CALL(*mockDisplay, updateCameraFreeMode(movingSpeed, rotaSpeed)).Times(1);
    cameraManager->updateCameraFreeMode();
}

// Test la fonction setMapCenter
TEST_F(CameraManagerTest, SetMapCenter) {
    Vector3f center = {10.0f, 5.0f, 3.0f};
    cameraManager->setMapCenter(center);
}

// Test la fonction setMapSize
TEST_F(CameraManagerTest, SetMapSize) {
    int width = 20;
    int height = 15;
    cameraManager->setMapSize(width, height);
}

// Test la fonction setMapInstance
TEST_F(CameraManagerTest, SetMapInstance) {
    auto mockCommunication = std::make_shared<MockICommunication>();
    auto mockGameInfos = std::make_shared<MockGameInfos>(mockCommunication);
    auto mockMap = std::make_shared<MockMap>(mockGameInfos, mockDisplay);
    cameraManager->setMapInstance(mockMap);
}

// Test la fonction setGameInfos
TEST_F(CameraManagerTest, SetGameInfos) {
    auto mockCommunication = std::make_shared<MockICommunication>();
    auto mockGameInfos = std::make_shared<MockGameInfos>(mockCommunication);
    cameraManager->setGameInfos(mockGameInfos);
}

// Test la fonction initTargetPositionFromCurrentCamera
TEST_F(CameraManagerTest, InitTargetPositionFromCurrentCamera) {
    Vector3f mockPosition = {10.0f, 5.0f, 3.0f};
    EXPECT_CALL(*mockDisplay, vector3DDistanceFromCamera(testing::_))
        .WillOnce(testing::Return(20.0f));
    EXPECT_CALL(*mockDisplay, vector3SubtractFromCamera(testing::_))
        .WillOnce(testing::Return(mockPosition));
    EXPECT_CALL(*mockDisplay, vector3Normalize(testing::_))
        .WillOnce(testing::Return(Vector3f{0.5f, 0.25f, 0.15f}));

    cameraManager->initTargetPositionFromCurrentCamera();
}

// Test de updateCameraPlayerMode quand gameInfos ou map ne sont pas définis
TEST_F(CameraManagerTest, UpdateCameraPlayerModeWithoutGameInfosOrMap) {
    EXPECT_CALL(*mockDisplay, updateCameraFreeMode(testing::_, testing::_)).Times(1);
    cameraManager->updateCameraPlayerMode();
}

// Test les interactions de la souris pendant le mode de caméra ciblé
TEST_F(CameraManagerTest, UpdateCameraTargetModeWithMouse) {
    Vector2i screenSize = {1920, 1080};
    float frameTime = 0.016f;

    EXPECT_CALL(*mockDisplay, getFrameTime())
        .WillRepeatedly(testing::Return(frameTime));
    EXPECT_CALL(*mockDisplay, getScreenSize())
        .WillRepeatedly(testing::Return(screenSize));
    EXPECT_CALL(*mockDisplay, getKeyId(testing::_))
        .WillRepeatedly(testing::Return(1));

    EXPECT_CALL(*mockDisplay, isMouseButtonPressed(1))
        .WillOnce(testing::Return(true));
    EXPECT_CALL(*mockDisplay, setMousePosition(testing::_)).Times(2);
    EXPECT_CALL(*mockDisplay, disableCursor()).Times(1);

    EXPECT_CALL(*mockDisplay, setCameraPosition(testing::_)).Times(1);
    EXPECT_CALL(*mockDisplay, setCameraTarget(testing::_)).Times(1);

    cameraManager->updateCameraTargetMode();
}

// Test du zoom avec la molette de la souris
TEST_F(CameraManagerTest, UpdateCameraTargetModeWithMouseWheel) {
    float frameTime = 0.016f;

    EXPECT_CALL(*mockDisplay, getFrameTime())
        .WillRepeatedly(testing::Return(frameTime));
    EXPECT_CALL(*mockDisplay, isMouseButtonPressed(testing::_))
        .WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, isMouseButtonReleased(testing::_))
        .WillOnce(testing::Return(false));
    EXPECT_CALL(*mockDisplay, getMouseWheelMove())
        .WillOnce(testing::Return(1.0f));

    EXPECT_CALL(*mockDisplay, setCameraPosition(testing::_)).Times(1);
    EXPECT_CALL(*mockDisplay, setCameraTarget(testing::_)).Times(1);

    cameraManager->updateCameraTargetMode();
}

// Test de la fonction calculateCameraPosition
TEST_F(CameraManagerTest, CalculateCameraPosition) {
    Vector3f playerPos = {10.0f, 5.0f, 3.0f};
    float angleXZ = 1.0f;

    Vector3f expectedPosition = {
        12.844959f,
        7.8765535,
        7.4307613
    };

    Vector3f calculatedPosition = cameraManager->calculateCameraPosition(playerPos, angleXZ);
    EXPECT_FLOAT_EQ(calculatedPosition.x, expectedPosition.x);
    EXPECT_FLOAT_EQ(calculatedPosition.y, expectedPosition.y);
    EXPECT_FLOAT_EQ(calculatedPosition.z, expectedPosition.z);
}

// Test de la fonction calculatePlayerPosition
TEST_F(CameraManagerTest, CalculatePlayerPosition) {
    zappy::structs::Player player;
    player.x = 10;
    player.y = 5;
    player.number = 42;

    Vector3f expectedPosition = {
        10.0f * zappy::gui::POSITION_MULTIPLIER,
        0.7f,
        5.0f * zappy::gui::POSITION_MULTIPLIER
    };

    Vector3f calculatedPosition = cameraManager->calculatePlayerPosition(player);
    EXPECT_FLOAT_EQ(calculatedPosition.x, expectedPosition.x);
    EXPECT_FLOAT_EQ(calculatedPosition.y, expectedPosition.y);
    EXPECT_FLOAT_EQ(calculatedPosition.z, expectedPosition.z);
}
