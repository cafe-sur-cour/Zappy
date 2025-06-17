/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** MsgHandler_test - Tests for MsgHandler functionality
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include "../../../../gui/src/Client/MsgHandler.hpp"
#include "../../../../gui/src/Game/GameInfos.hpp"
#include "../../../../gui/src/Communication/ICommunication.hpp"
#include "../../../../gui/src/Exceptions/Exceptions.hpp"
#include "../../../../gui/src/Utils/Constants.hpp"

// Mock Communication class
class MockCommunication : public ICommunication {
public:
    MOCK_METHOD(void, sendMessage, (const std::string& message), (override));
    MOCK_METHOD(bool, hasMessages, (), (const, override));
    MOCK_METHOD(std::string, popMessage, (), (override));
    MOCK_METHOD(bool, isConnected, (), (const, override));
    MOCK_METHOD(void, disconnect, (), (override));
};

class MsgHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockCommunication = std::make_shared<MockCommunication>();
        gameInfos = std::make_shared<GameInfos>(mockCommunication);
    }

    void TearDown() override {
        if (msgHandler) {
            msgHandler->stop();
            msgHandler.reset();
        }
        gameInfos.reset();
        mockCommunication.reset();
    }

    std::shared_ptr<GameInfos> gameInfos;
    std::shared_ptr<MockCommunication> mockCommunication;
    std::unique_ptr<MsgHandler> msgHandler;
};

// Test Welcome Message
TEST_F(MsgHandlerTest, HandleWelcomeMessage) {
    EXPECT_CALL(*mockCommunication, sendMessage("GRAPHIC\n"))
        .Times(1);
    
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("WELCOME");
}

// Test Map Size Message
TEST_F(MsgHandlerTest, HandleMszMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("msz 10 20");
    
    auto mapSize = gameInfos->getMapSize();
    EXPECT_EQ(mapSize.first, 10);
    EXPECT_EQ(mapSize.second, 20);
}

TEST_F(MsgHandlerTest, HandleMszMessageInvalid) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("msz -5 20");
    
    // Should not update map size for invalid input
    auto mapSize = gameInfos->getMapSize();
    EXPECT_EQ(mapSize.first, 0);
    EXPECT_EQ(mapSize.second, 0);
}

// Test Time Unit Messages
TEST_F(MsgHandlerTest, HandleSgtMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("sgt 100");
    
    EXPECT_EQ(gameInfos->getTimeUnit(), 100);
}

TEST_F(MsgHandlerTest, HandleSstMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("sst 150");
    
    EXPECT_EQ(gameInfos->getTimeUnit(), 150);
}

// Test Tile Content Message
TEST_F(MsgHandlerTest, HandleBctMessage) {
    gameInfos->setMapSize(20, 20);
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    
    msgHandler->handleMessage("bct 5 8 10 1 2 3 4 5 6");
    
    auto tile = gameInfos->getTile(5, 8);
    EXPECT_EQ(tile.x, 5);
    EXPECT_EQ(tile.y, 8);
    EXPECT_EQ(tile.food, 10);
    EXPECT_EQ(tile.linemate, 1);
    EXPECT_EQ(tile.deraumere, 2);
    EXPECT_EQ(tile.sibur, 3);
    EXPECT_EQ(tile.mendiane, 4);
    EXPECT_EQ(tile.phiras, 5);
    EXPECT_EQ(tile.thystame, 6);
}

// Test Team Name Message
TEST_F(MsgHandlerTest, HandleTnaMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("tna Team1");
    
    auto teamNames = gameInfos->getTeamNames();
    EXPECT_EQ(teamNames.size(), 1);
    EXPECT_EQ(teamNames[0], "Team1");
}

// Test Player Connection Message
TEST_F(MsgHandlerTest, HandlePnwMessage) {
    gameInfos->updateTeamName("Team1");
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    
    msgHandler->handleMessage("pnw #1 5 10 2 3 Team1");
    
    auto players = gameInfos->getPlayers();
    EXPECT_EQ(players.size(), 1);
    EXPECT_EQ(players[0].number, 1);
    EXPECT_EQ(players[0].x, 5);
    EXPECT_EQ(players[0].y, 10);
    EXPECT_EQ(players[0].orientation, 2);
    EXPECT_EQ(players[0].level, 3);
    EXPECT_EQ(players[0].teamName, "Team1");
}

TEST_F(MsgHandlerTest, HandlePnwMessageInvalidPlayerNumber) {
    gameInfos->updateTeamName("Team1");
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    
    msgHandler->handleMessage("pnw invalid 5 10 2 3 Team1");
    
    auto players = gameInfos->getPlayers();
    EXPECT_EQ(players.size(), 0);
}

// Test Player Position Message
TEST_F(MsgHandlerTest, HandlePpoMessage) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);
    
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("ppo #1 10 15 3");
    
    auto players = gameInfos->getPlayers();
    EXPECT_EQ(players[0].x, 10);
    EXPECT_EQ(players[0].y, 15);
    EXPECT_EQ(players[0].orientation, 3);
}

// Test Player Level Message
TEST_F(MsgHandlerTest, HandlePlvMessage) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);
    
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("plv #1 5");
    
    auto players = gameInfos->getPlayers();
    EXPECT_EQ(players[0].level, 5);
}

// Test Player Inventory Message
TEST_F(MsgHandlerTest, HandlePinMessage) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);
    
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pin #1 5 10 10 1 2 3 4 5 6");
    
    auto players = gameInfos->getPlayers();
    EXPECT_EQ(players[0].inventory.food, 10);
    EXPECT_EQ(players[0].inventory.linemate, 1);
    EXPECT_EQ(players[0].inventory.deraumere, 2);
    EXPECT_EQ(players[0].inventory.sibur, 3);
    EXPECT_EQ(players[0].inventory.mendiane, 4);
    EXPECT_EQ(players[0].inventory.phiras, 5);
    EXPECT_EQ(players[0].inventory.thystame, 6);
}

// Test Player Expulsion Message
TEST_F(MsgHandlerTest, HandlePexMessage) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);
    
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pex #1");
    
    // Just test that it doesn't crash - the actual function is void
    SUCCEED();
}

// Test Player Broadcast Message
TEST_F(MsgHandlerTest, HandlePbcMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pbc #1 Hello World");
    
    auto broadcasts = gameInfos->getPlayersBroadcasting();
    EXPECT_EQ(broadcasts.size(), 1);
    EXPECT_EQ(broadcasts[0].first, 1);
    EXPECT_EQ(broadcasts[0].second, " Hello World");
}

// Test Incantation Start Message
TEST_F(MsgHandlerTest, HandlePicMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pic 5 10 2 #1 #2 #3");
    
    // Test that it doesn't crash - incantations are added to GameInfos
    SUCCEED();
}

// Test Incantation End Message
TEST_F(MsgHandlerTest, HandlePieMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pie 5 10 1");
    
    // Test that it doesn't crash - incantations are removed from GameInfos
    SUCCEED();
}

// Test Player Fork Message
TEST_F(MsgHandlerTest, HandlePfkMessage) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);
    
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pfk #1");
    
    // Test that it doesn't crash
    SUCCEED();
}

// Test Player Drop Resource Message
TEST_F(MsgHandlerTest, HandlePdrMessage) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);
    
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pdr #1 0");
    
    // Test that it doesn't crash
    SUCCEED();
}

// Test Player Collect Resource Message
TEST_F(MsgHandlerTest, HandlePgtMessage) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);
    
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pgt #1 1");
    
    // Test that it doesn't crash
    SUCCEED();
}

// Test Player Death Message
TEST_F(MsgHandlerTest, HandlePdiMessage) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);
    
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pdi #1");
    
    auto players = gameInfos->getPlayers();
    EXPECT_EQ(players.size(), 0);
}

// Test Egg Laying Message
TEST_F(MsgHandlerTest, HandleEnwMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("enw #1 #2 5 10 Team1");
    
    // Test that it doesn't crash
    SUCCEED();
}

// Test Egg Hatching Message
TEST_F(MsgHandlerTest, HandleEboMessage) {
    zappy::structs::Egg egg(1, 2, 5, 5, false, "Team1");
    gameInfos->addEgg(egg);
    
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("ebo #1");
    
    auto eggs = gameInfos->getEggs();
    EXPECT_TRUE(eggs[0].hatched);
}

// Test Egg Death Message
TEST_F(MsgHandlerTest, HandleEdiMessage) {
    zappy::structs::Egg egg1(1, 2, 5, 5, false, "Team1");
    zappy::structs::Egg egg2(2, 3, 6, 6, false, "Team1");
    gameInfos->addEgg(egg1);
    gameInfos->addEgg(egg2);
    
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("edi #1");
    
    auto eggs = gameInfos->getEggs();
    EXPECT_EQ(eggs.size(), 1);
    EXPECT_EQ(eggs[0].eggNumber, 2);
}

// Test Game Over Message
TEST_F(MsgHandlerTest, HandleSegMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("seg Team1");
    
    auto gameOverInfo = gameInfos->isGameOver();
    EXPECT_TRUE(gameOverInfo.first);
    EXPECT_EQ(gameOverInfo.second, "Team1");
}

// Test Server Message
TEST_F(MsgHandlerTest, HandleSmgMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("smg Hello from server");
    
    // Test that it doesn't crash
    SUCCEED();
}

// Test Unknown Command Message
TEST_F(MsgHandlerTest, HandleSucMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("suc");
    
    // Test that it doesn't crash
    SUCCEED();
}

// Test Bad Parameters Message
TEST_F(MsgHandlerTest, HandleSbpMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("sbp");
    
    // Test that it doesn't crash
    SUCCEED();
}

// Test Empty Message
TEST_F(MsgHandlerTest, HandleEmptyMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("");
    
    // Should not crash and should not process anything
    SUCCEED();
}

// Test Invalid Message
TEST_F(MsgHandlerTest, HandleInvalidMessage) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("invalid message format");
    
    // Should not crash
    SUCCEED();
}

// Test Invalid Player Number Format
TEST_F(MsgHandlerTest, HandleInvalidPlayerNumberFormat) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("ppo invalid_player 5 10 2");
    
    // Should not crash and should not update anything
    SUCCEED();
}

// Test Invalid Coordinates
TEST_F(MsgHandlerTest, HandleInvalidCoordinates) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);
    
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("ppo #1 -5 -10 2");
    
    // Should not crash but should handle invalid coordinates
    SUCCEED();
}

// Test Message with Network Exception
TEST_F(MsgHandlerTest, HandleMessageWithNetworkException) {
    EXPECT_CALL(*mockCommunication, disconnect())
        .Times(testing::AtLeast(0));
    
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    
    // This should not throw but handle the exception gracefully
    EXPECT_NO_THROW(msgHandler->handleMessage("WELCOME"));
}

// Test Start and Stop
TEST_F(MsgHandlerTest, StartAndStop) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    
    // The constructor automatically starts the handler
    // Just test that stop works without crashing
    msgHandler->stop();
    msgHandler->start();
    msgHandler->stop();
}

// Test Multiple Team Names
TEST_F(MsgHandlerTest, HandleMultipleTeamNames) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    
    msgHandler->handleMessage("tna Team1");
    msgHandler->handleMessage("tna Team2");
    msgHandler->handleMessage("tna Team3");
    
    auto teamNames = gameInfos->getTeamNames();
    EXPECT_EQ(teamNames.size(), 3);
    EXPECT_EQ(teamNames[0], "Team1");
    EXPECT_EQ(teamNames[1], "Team2");
    EXPECT_EQ(teamNames[2], "Team3");
}

// Test Invalid Resource IDs
TEST_F(MsgHandlerTest, HandleInvalidResourceIds) {
    gameInfos->updateTeamName("Team1");
    zappy::structs::Player player(1, 5, 5, 1, 3, "Team1");
    gameInfos->addPlayer(player);
    
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    msgHandler->handleMessage("pdr #1 -1");  // Invalid resource ID
    msgHandler->handleMessage("pgt #1 999"); // Invalid resource ID
    
    // Should not crash
    SUCCEED();
}

// Test Edge Cases for Numbers
TEST_F(MsgHandlerTest, HandleEdgeCaseNumbers) {
    msgHandler = std::make_unique<MsgHandler>(gameInfos, mockCommunication);
    
    // Test with very large numbers
    msgHandler->handleMessage("msz 99999 99999");
    
    // Test with zero
    msgHandler->handleMessage("sgt 0");
    
    // Should handle these gracefully
    SUCCEED();
}
