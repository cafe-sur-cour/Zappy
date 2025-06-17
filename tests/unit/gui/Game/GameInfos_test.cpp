/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GameInfos_test
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../../gui/src/Game/GameInfos.hpp"

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

class GameInfosTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockCommunication = std::make_shared<testing::NiceMock<MockCommunication>>();
        mockAudio = std::make_shared<testing::NiceMock<MockAudio>>();
        gameInfos = std::make_unique<GameInfos>(mockCommunication);
        gameInfos->setAudio(mockAudio);
    }

    void TearDown() override {
        gameInfos.reset();
        mockCommunication.reset();
        mockAudio.reset();
    }

    std::unique_ptr<GameInfos> gameInfos;
    std::shared_ptr<testing::NiceMock<MockCommunication>> mockCommunication;
    std::shared_ptr<testing::NiceMock<MockAudio>> mockAudio;
};

// Map size tests
TEST_F(GameInfosTest, SetAndGetMapSize) {
    gameInfos->setMapSize(10, 20);
    auto size = gameInfos->getMapSize();

    EXPECT_EQ(size.first, 10);
    EXPECT_EQ(size.second, 20);
}

// Time unit tests
TEST_F(GameInfosTest, SetAndGetTimeUnit) {
    gameInfos->setTimeUnit(100);

    EXPECT_EQ(gameInfos->getTimeUnit(), 100);
}

// Tile tests
TEST_F(GameInfosTest, UpdateAndGetTile) {
    zappy::structs::Tile tile(5, 8, 10, 1, 2, 3, 4, 5, 6);
    gameInfos->setMapSize(20, 20);
    gameInfos->updateTile(tile);

    auto retrievedTile = gameInfos->getTile(5, 8);

    EXPECT_EQ(retrievedTile.x, 5);
    EXPECT_EQ(retrievedTile.y, 8);
    EXPECT_EQ(retrievedTile.food, 10);
    EXPECT_EQ(retrievedTile.linemate, 1);
    EXPECT_EQ(retrievedTile.deraumere, 2);
    EXPECT_EQ(retrievedTile.sibur, 3);
    EXPECT_EQ(retrievedTile.mendiane, 4);
    EXPECT_EQ(retrievedTile.phiras, 5);
    EXPECT_EQ(retrievedTile.thystame, 6);
}

TEST_F(GameInfosTest, GetNonexistentTile) {
    gameInfos->setMapSize(20, 20);

    auto retrievedTile = gameInfos->getTile(15, 15);

    EXPECT_EQ(retrievedTile.x, 15);
    EXPECT_EQ(retrievedTile.y, 15);
    EXPECT_EQ(retrievedTile.food, 0);
    EXPECT_EQ(retrievedTile.linemate, 0);
    EXPECT_EQ(retrievedTile.deraumere, 0);
    EXPECT_EQ(retrievedTile.sibur, 0);
    EXPECT_EQ(retrievedTile.mendiane, 0);
    EXPECT_EQ(retrievedTile.phiras, 0);
    EXPECT_EQ(retrievedTile.thystame, 0);
}

TEST_F(GameInfosTest, UpdateExistingTile) {
    gameInfos->setMapSize(20, 20);

    zappy::structs::Tile tile1(5, 8, 10, 1, 2, 3, 4, 5, 6);
    gameInfos->updateTile(tile1);

    zappy::structs::Tile tile2(5, 8, 20, 2, 3, 4, 5, 6, 7);
    gameInfos->updateTile(tile2);

    auto retrievedTile = gameInfos->getTile(5, 8);

    EXPECT_EQ(retrievedTile.food, 20);
    EXPECT_EQ(retrievedTile.linemate, 2);
}

TEST_F(GameInfosTest, UpdateTileOutOfBounds) {
    gameInfos->setMapSize(10, 10);

    zappy::structs::Tile tile(15, 15, 10, 1, 2, 3, 4, 5, 6);
    gameInfos->updateTile(tile);

    auto tiles = gameInfos->getTiles();
    EXPECT_EQ(tiles.size(), 0);
}

TEST_F(GameInfosTest, GetAllTiles) {
    gameInfos->setMapSize(10, 10);

    zappy::structs::Tile tile1(1, 1, 10, 1, 2, 3, 4, 5, 6);
    zappy::structs::Tile tile2(2, 2, 20, 2, 3, 4, 5, 6, 7);

    gameInfos->updateTile(tile1);
    gameInfos->updateTile(tile2);

    auto tiles = gameInfos->getTiles();

    EXPECT_EQ(tiles.size(), 2);
}

// Team name tests
TEST_F(GameInfosTest, UpdateAndGetTeamNames) {
    gameInfos->updateTeamName("Team1");
    gameInfos->updateTeamName("Team2");

    auto teamNames = gameInfos->getTeamNames();

    EXPECT_EQ(teamNames.size(), 2);
    EXPECT_EQ(teamNames[0], "Team1");
    EXPECT_EQ(teamNames[1], "Team2");
}

TEST_F(GameInfosTest, DuplicateTeamName) {
    gameInfos->updateTeamName("Team1");
    gameInfos->updateTeamName("Team1");

    auto teamNames = gameInfos->getTeamNames();

    EXPECT_EQ(teamNames.size(), 1);
    EXPECT_EQ(teamNames[0], "Team1");
}

// Player tests
TEST_F(GameInfosTest, AddAndGetPlayer) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    auto players = gameInfos->getPlayers();

    EXPECT_EQ(players.size(), 1);
    EXPECT_EQ(players[0].number, 1);
    EXPECT_EQ(players[0].x, 5);
    EXPECT_EQ(players[0].y, 5);
    EXPECT_EQ(players[0].orientation, 1);
    EXPECT_EQ(players[0].level, 3);
    EXPECT_EQ(players[0].teamName, "Team1");
}

TEST_F(GameInfosTest, UpdateExistingPlayer) {
    zappy::structs::Player player1(1, 5, 5, 1, 3, "Team1");
    zappy::structs::Player player2(1, 10, 10, 2, 4, "Team1");

    gameInfos->addPlayer(player1);
    gameInfos->addPlayer(player2);

    auto players = gameInfos->getPlayers();

    EXPECT_EQ(players.size(), 1);
    EXPECT_EQ(players[0].number, 1);
    EXPECT_EQ(players[0].x, 10);
    EXPECT_EQ(players[0].y, 10);
    EXPECT_EQ(players[0].orientation, 2);
    EXPECT_EQ(players[0].level, 4);
}

TEST_F(GameInfosTest, UpdatePlayerPosition) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);
    gameInfos->setMapSize(20, 20);

    gameInfos->updatePlayerPosition(1, 10, 15);

    auto players = gameInfos->getPlayers();

    EXPECT_EQ(players[0].x, 10);
    EXPECT_EQ(players[0].y, 15);
}

TEST_F(GameInfosTest, UpdatePlayerPositionOutOfBounds) {
    gameInfos->setMapSize(10, 10);
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    gameInfos->updatePlayerPosition(1, 20, 20);

    auto players = gameInfos->getPlayers();

    EXPECT_EQ(players[0].x, 5);
    EXPECT_EQ(players[0].y, 5);
}

TEST_F(GameInfosTest, UpdatePlayerLevel) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    gameInfos->updatePlayerLevel(1, 5);

    auto players = gameInfos->getPlayers();

    EXPECT_EQ(players[0].level, 5);
}

TEST_F(GameInfosTest, UpdatePlayerInventory) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    zappy::structs::Inventory inventory(10, 1, 2, 3, 4, 5, 6);
    gameInfos->updatePlayerInventory(1, inventory);

    auto players = gameInfos->getPlayers();

    EXPECT_EQ(players[0].inventory.food, 10);
    EXPECT_EQ(players[0].inventory.linemate, 1);
    EXPECT_EQ(players[0].inventory.deraumere, 2);
    EXPECT_EQ(players[0].inventory.sibur, 3);
    EXPECT_EQ(players[0].inventory.mendiane, 4);
    EXPECT_EQ(players[0].inventory.phiras, 5);
    EXPECT_EQ(players[0].inventory.thystame, 6);
}

TEST_F(GameInfosTest, PlayerDeath) {
    zappy::structs::Player player1(1, 5, 5, 1, 3, "Team1");
    zappy::structs::Player player2(2, 6, 6, 1, 3, "Team1");

    gameInfos->addPlayer(player1);
    gameInfos->addPlayer(player2);

    gameInfos->updatePlayerDeath(1);

    auto players = gameInfos->getPlayers();

    EXPECT_EQ(players.size(), 1);
    EXPECT_EQ(players[0].number, 2);
    gameInfos->updatePlayerDeath(-1);
}

// Broadcast tests
TEST_F(GameInfosTest, PlayerBroadcast) {
    gameInfos->addPlayerBroadcast(1, "Hello World");

    auto broadcasts = gameInfos->getPlayersBroadcasting();

    EXPECT_EQ(broadcasts.size(), 1);
    EXPECT_EQ(broadcasts[0].first, 1);
    EXPECT_EQ(broadcasts[0].second, "Hello World");
}

TEST_F(GameInfosTest, EmptyBroadcast) {
    gameInfos->addPlayerBroadcast(1, "");

    auto broadcasts = gameInfos->getPlayersBroadcasting();

    EXPECT_EQ(broadcasts.size(), 0);
}

// Incantation tests
TEST_F(GameInfosTest, AddAndRemoveIncantation) {
    std::vector<int> players = {1, 2, 3};
    zappy::structs::Incantation incantation(5, 5, 2, players);

    gameInfos->addIncantation(incantation);
    gameInfos->removeIncantation(5, 5, 1);

    SUCCEED();
}

// Egg tests
TEST_F(GameInfosTest, AddAndGetEgg) {
    zappy::structs::Egg egg(1, 2, 5, 5, false, "Team1");
    gameInfos->addEgg(egg);
    gameInfos->addEgg(egg);

    auto eggs = gameInfos->getEggs();

    EXPECT_EQ(eggs.size(), 1);
    EXPECT_EQ(eggs[0].eggNumber, 1);
    EXPECT_EQ(eggs[0].playerNumber, 2);
    EXPECT_EQ(eggs[0].x, 5);
    EXPECT_EQ(eggs[0].y, 5);
    EXPECT_FALSE(eggs[0].hatched);
    EXPECT_EQ(eggs[0].teamName, "Team1");
}

TEST_F(GameInfosTest, UpdateEggHatched) {
    zappy::structs::Egg egg(1, 2, 5, 5, false, "Team1");
    gameInfos->addEgg(egg);

    gameInfos->updateEggHatched(1);

    auto eggs = gameInfos->getEggs();

    EXPECT_TRUE(eggs[0].hatched);
}

TEST_F(GameInfosTest, EggDeath) {
    zappy::structs::Egg egg1(1, 2, 5, 5, false, "Team1");
    zappy::structs::Egg egg2(2, 3, 6, 6, false, "Team1");

    gameInfos->addEgg(egg1);
    gameInfos->addEgg(egg2);

    gameInfos->updateEggDeath(1);

    auto eggs = gameInfos->getEggs();

    EXPECT_EQ(eggs.size(), 1);
    EXPECT_EQ(eggs[0].eggNumber, 2);
}

// Game over tests
TEST_F(GameInfosTest, SetAndCheckGameOver) {
    gameInfos->setGameOver("Team1");

    auto gameOverInfo = gameInfos->isGameOver();

    EXPECT_TRUE(gameOverInfo.first);
    EXPECT_EQ(gameOverInfo.second, "Team1");
}

TEST_F(GameInfosTest, InitialGameOverStatus) {
    auto gameOverInfo = gameInfos->isGameOver();

    EXPECT_FALSE(gameOverInfo.first);
    EXPECT_TRUE(gameOverInfo.second.empty());
}

// updatePlayerExpulsion tests
TEST_F(GameInfosTest, UpdatePlayerExpulsion) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    gameInfos->updatePlayerExpulsion(1);
    gameInfos->updatePlayerExpulsion(1);
    gameInfos->updatePlayerExpulsion(2);
    gameInfos->updatePlayerExpulsion(-1);
}

// updatePlayerResourceAction tests
TEST_F(GameInfosTest, UpdatePlayerResourceAction) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    gameInfos->updatePlayerResourceAction(1, 0, true);
    gameInfos->updatePlayerResourceAction(1, 1, false);

    SUCCEED();
}

// updatePlayerFork tests
TEST_F(GameInfosTest, UpdatePlayerFork) {
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);

    gameInfos->updatePlayerFork(1);
    SUCCEED();
}

// Test audio integration
TEST_F(GameInfosTest, PlayDefeatSound) {
    EXPECT_CALL(*mockAudio, playSound(testing::_, testing::_)).Times(1);

    gameInfos->playDefeatSound("Team1");
}
