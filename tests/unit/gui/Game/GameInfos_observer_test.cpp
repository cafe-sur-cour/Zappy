/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GameInfos_observer_test - Test Observer notifications in GameInfos
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

// Special test observer that exposes when update is called
class TestObserver : public IObserver {
public:
    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, onGameEvent, (GameEventType, const std::string&), (override));

    class TestableGameInfos : public GameInfos {
    public:
        TestableGameInfos(std::shared_ptr<ICommunication> communication)
            : GameInfos(communication) {}

        void testNotifyObservers() {
            notifyObservers();
        }
    };
};

class GameInfosObserverTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockCommunication = std::make_shared<testing::NiceMock<MockCommunication>>();
        mockAudio = std::make_shared<testing::NiceMock<MockAudio>>();
        mockObserver = std::make_shared<TestObserver>();
        gameInfos = std::make_unique<TestObserver::TestableGameInfos>(mockCommunication);
        gameInfos->setAudio(mockAudio);
        gameInfos->addObserver(mockObserver);
    }

    void TearDown() override {
        gameInfos.reset();
        mockCommunication.reset();
        mockAudio.reset();
        mockObserver.reset();
    }

    std::unique_ptr<TestObserver::TestableGameInfos> gameInfos;
    std::shared_ptr<testing::NiceMock<MockCommunication>> mockCommunication;
    std::shared_ptr<testing::NiceMock<MockAudio>> mockAudio;
    std::shared_ptr<TestObserver> mockObserver;
};

// Test the notifyObservers method which is called by notifyStateChange
TEST_F(GameInfosObserverTest, NotifyObservers) {
    EXPECT_CALL(*mockObserver, update()).Times(1);

    gameInfos->testNotifyObservers();
}
