/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GameInfos_additional_test - Additional test coverage for GameInfos
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../gui/src/Game/GameInfos.hpp"
#include "../../../../gui/src/Exceptions/Exceptions.hpp"

// Mock for ICommunication
class MockCommunication : public ICommunication {
public:
    MOCK_METHOD(void, sendMessage, (const std::string &message), (override));
    MOCK_METHOD(bool, hasMessages, (), (const, override));
    MOCK_METHOD(std::string, popMessage, (), (override));
    MOCK_METHOD(bool, isConnected, (), (const, override));
    MOCK_METHOD(void, disconnect, (), (override));
};

// Mock for IAudio
class MockAudio : public IAudio {
public:
    MOCK_METHOD(float, getSFXVolumeLevel, (), (override));
    MOCK_METHOD(float, getMusicVolumeLevel, (), (override));
    MOCK_METHOD(void, setSFXVolumeLevel, (float), (override));
    MOCK_METHOD(void, setMusicVolumeLevel, (float), (override));
    MOCK_METHOD(bool, loadSound, (const std::string& id, const std::string& filepath), (override));
    MOCK_METHOD(void, playMainTheme, (float volume), (override));
    MOCK_METHOD(void, playNextTheme, (float volume), (override));
    MOCK_METHOD(void, playSound, (const std::string& id, float volume), (override));
    MOCK_METHOD(void, stopSound, (const std::string& id), (override));
    MOCK_METHOD(bool, isSoundPlaying, (const std::string& id), (const, override));
    MOCK_METHOD(void, setSoundLooping, (const std::string& id, bool looping), (override));
    MOCK_METHOD(void, setSoundVolume, (const std::string& id, float volume), (override));
};

// Mock Observer to test notifications
class MockObserver : public IObserver {
public:
    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, onGameEvent, (GameEventType, const std::string&), (override));
};

class GameInfosAdditionalTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockCommunication = std::make_shared<testing::NiceMock<MockCommunication>>();
        mockAudio = std::make_shared<testing::NiceMock<MockAudio>>();
        mockObserver = std::make_shared<testing::NiceMock<MockObserver>>();
        gameInfos = std::make_unique<GameInfos>(mockCommunication);
        gameInfos->setAudio(mockAudio);
        gameInfos->addObserver(mockObserver);
    }

    void TearDown() override {
        gameInfos.reset();
        mockCommunication.reset();
        mockAudio.reset();
        mockObserver.reset();
    }

    std::unique_ptr<GameInfos> gameInfos;
    std::shared_ptr<testing::NiceMock<MockCommunication>> mockCommunication;
    std::shared_ptr<testing::NiceMock<MockAudio>> mockAudio;
    std::shared_ptr<testing::NiceMock<MockObserver>> mockObserver;
};

// Test for setCurrentCameraMode
TEST_F(GameInfosAdditionalTest, SetAndUseCurrentCameraMode) {
    gameInfos->setCurrentCameraMode(zappy::gui::CameraMode::PLAYER);

    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);
    gameInfos->setCurrentPlayerFocus(1);

    EXPECT_CALL(*mockAudio, playSound("collect", 100.0f)).Times(1);
    gameInfos->updatePlayerResourceAction(1, 0, true);
}

// Test for setCurrentPlayerFocus
TEST_F(GameInfosAdditionalTest, SetCurrentPlayerFocus) {
    gameInfos->setCurrentPlayerFocus(5);

    zappy::structs::Player player(5, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);
    gameInfos->setCurrentCameraMode(zappy::gui::CameraMode::PLAYER);

    EXPECT_CALL(*mockAudio, playSound("collect", 100.0f)).Times(1);
    gameInfos->updatePlayerResourceAction(5, 0, true);
}

// Test killPlayer method
TEST_F(GameInfosAdditionalTest, KillPlayer) {
    gameInfos->killPlayer(-1);

    EXPECT_CALL(*mockCommunication, sendMessage("kil #5\n")).Times(1);
    gameInfos->killPlayer(5);

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());
    EXPECT_CALL(*mockCommunication, sendMessage("kil #10\n")).Times(1);
    gameInfos->killPlayer(10);

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());
    ON_CALL(*mockCommunication, sendMessage).WillByDefault(
        [](const std::string &) { throw Exceptions::NetworkException("Test exception"); }
    );
    gameInfos->killPlayer(15);
}

// Test getIncantations method
TEST_F(GameInfosAdditionalTest, GetIncantations) {
    std::vector<int> players = {1, 2, 3};
    zappy::structs::Incantation incantation1(5, 5, 2, players);
    zappy::structs::Incantation incantation2(10, 10, 3, players);

    gameInfos->addIncantation(incantation1);
    gameInfos->addIncantation(incantation2);

    auto incantations = gameInfos->getIncantations();

    EXPECT_EQ(incantations.size(), 2);
    EXPECT_EQ(incantations[0].x, 5);
    EXPECT_EQ(incantations[0].y, 5);
    EXPECT_EQ(incantations[0].level, 2);
    EXPECT_EQ(incantations[1].x, 10);
    EXPECT_EQ(incantations[1].y, 10);
    EXPECT_EQ(incantations[1].level, 3);
}

// Test getPlayer method
TEST_F(GameInfosAdditionalTest, GetPlayer) {
    zappy::structs::Player player1(1, 5, 5, 1, 3, "Team1");
    zappy::structs::Player player2(2, 10, 10, 2, 4, "Team2");

    gameInfos->addPlayer(player1);
    gameInfos->addPlayer(player2);

    auto retrievedPlayer = gameInfos->getPlayer(2);

    EXPECT_EQ(retrievedPlayer.number, 2);
    EXPECT_EQ(retrievedPlayer.x, 10);
    EXPECT_EQ(retrievedPlayer.y, 10);
    EXPECT_EQ(retrievedPlayer.orientation, 2);
    EXPECT_EQ(retrievedPlayer.level, 4);
    EXPECT_EQ(retrievedPlayer.teamName, "Team2");

    auto nonExistentPlayer = gameInfos->getPlayer(99);
    EXPECT_EQ(nonExistentPlayer.number, 0);
}

// Test observer notification - we can't directly call notifyStateChange as it's private
TEST_F(GameInfosAdditionalTest, NotifyObserver) {
    EXPECT_CALL(*mockObserver, onGameEvent(GameEventType::TEAM_WIN, "TestTeam")).Times(1);
    gameInfos->setGameOver("TestTeam");
}

// Test updatePlayerOrientation with invalid orientation
TEST_F(GameInfosAdditionalTest, UpdatePlayerOrientationInvalid) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    gameInfos->updatePlayerOrientation(1, 0);
    gameInfos->updatePlayerOrientation(1, 5);

    auto players = gameInfos->getPlayers();
    EXPECT_EQ(players[0].orientation, 1);
}

// Additional tests for exception handling paths
TEST_F(GameInfosAdditionalTest, SetGameOverExceptionHandling) {
    ON_CALL(*mockAudio, playSound).WillByDefault(
        [](const std::string &, float) { throw std::runtime_error("Test exception"); }
    );

    gameInfos->setGameOver("Team1");

    auto gameOverInfo = gameInfos->isGameOver();
    EXPECT_TRUE(gameOverInfo.first);
    EXPECT_EQ(gameOverInfo.second, "Team1");
}

TEST_F(GameInfosAdditionalTest, PlayDefeatSoundExceptionHandling) {
    ON_CALL(*mockAudio, playSound).WillByDefault(
        [](const std::string &, float) { throw std::runtime_error("Test exception"); }
    );

    EXPECT_NO_THROW(gameInfos->playDefeatSound("Team1"));
}

// Test updatePlayerResourceAction with invalid resource ID
TEST_F(GameInfosAdditionalTest, UpdatePlayerResourceActionInvalidResource) {
    gameInfos->updatePlayerResourceAction(1, -1, true);
    gameInfos->updatePlayerResourceAction(1, 7, true);

    SUCCEED();
}

// Test updatePlayerDeath with non-existent player
TEST_F(GameInfosAdditionalTest, UpdatePlayerDeathNonExistentPlayer) {
    gameInfos->updatePlayerDeath(99);

    SUCCEED();
}

// Test setTimeUnit with server communication
TEST_F(GameInfosAdditionalTest, SetTimeUnitWithServerCommunication) {
    EXPECT_CALL(*mockCommunication, sendMessage("sst 200\n")).Times(1);
    gameInfos->setTimeUnit(200, true);

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());
    EXPECT_CALL(*mockCommunication, sendMessage("sst 300\n")).Times(1);
    gameInfos->setTimeUnit(300, true);

    testing::Mock::VerifyAndClearExpectations(mockCommunication.get());
    ON_CALL(*mockCommunication, sendMessage).WillByDefault(
        [](const std::string &) { throw Exceptions::NetworkException("Test exception"); }
    );
    gameInfos->setTimeUnit(400, true);
}
