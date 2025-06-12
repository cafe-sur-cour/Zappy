/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** HUD
*/

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include "../../Utils/Constants.hpp"
#include "HUD.hpp"

HUD::HUD(std::shared_ptr<IDisplay> display, std::shared_ptr<GameInfos> gameInfos,
         std::shared_ptr<IAudio> audio, std::function<void()> resetCameraFunc)
    : _containers(), _display(display), _gameInfos(gameInfos), _audio(audio),
      _resetCameraFunc(resetCameraFunc)
{
    _help = std::make_shared<Help>(display, audio);
    initDefaultLayout(15.0f, 20.0f);
    initExitButton();
    initSettingsButton();
    initHelpButton();
    initCameraResetButton();
    initTeamPlayersDisplay(_gameInfos);
}

HUD::~HUD()
{
    _containers.clear();
}

void HUD::draw()
{
    for (auto &pair : _containers) {
        pair.second->draw();
    }

    if (_help && _help->isVisible()) {
        _help->draw();
    }
}

void HUD::update()
{
    for (auto& pair : _containers) {
        pair.second->update();
    }

    if (_help) {
        _help->update();
    }

    updateTeamPlayersDisplay(_gameInfos);
}

std::shared_ptr<Containers> HUD::addContainer(
    const std::string& id,
    float x, float y,
    float width, float height,
    Color32 backgroundColor
)
{
    if (_containers.find(id) != _containers.end())
        return nullptr;

    auto container = std::make_shared<Containers>(
        _display,
        _audio,
        x,
        y,
        width,
        height,
        backgroundColor);

    _containers[id] = container;

    return container;
}

std::shared_ptr<Containers> HUD::getContainer(const std::string& id) const
{
    auto it = _containers.find(id);

    if (it != _containers.end())
        return it->second;

    return nullptr;
}

bool HUD::removeContainer(const std::string& id)
{
    auto it = _containers.find(id);

    if (it != _containers.end()) {
        _containers.erase(it);
        return true;
    }
    return false;
}

void HUD::handleResize(int oldWidth, int oldHeight, int newWidth, int newHeight)
{
    for (auto& pair : _containers) {
        pair.second->handleResize(oldWidth, oldHeight, newWidth, newHeight);
    }

    if (_help) {
        _help->handleResize(oldWidth, oldHeight, newWidth, newHeight);
    }
}

void HUD::clearAllContainers()
{
    _containers.clear();
}

void HUD::initDefaultLayout(float sideWidthPercent, float bottomHeightPercent)
{
    if (sideWidthPercent <= 0.0f)
        sideWidthPercent = 15.0f;
    if (bottomHeightPercent <= 0.0f)
        bottomHeightPercent = 20.0f;

    auto screenSize = this->_display->getScreenSize();
    int screenHeight = screenSize.x;
    int screenWidth = screenSize.y;

    float sideWidth = (screenWidth * sideWidthPercent) / 100.0f;
    float bottomHeight = (screenHeight * bottomHeightPercent) / 100.0f;
    float squareSize = sideWidth;

    auto squareContainer = createSquareContainer(squareSize, sideWidthPercent);

    float sideYStart = squareSize;
    float sideHeight = screenHeight - squareSize - bottomHeight;

    auto sideContainer = createSideContainer(
                            sideYStart,
                            sideWidth,
                            sideHeight,
                            sideWidthPercent,
                            bottomHeightPercent);

    auto bottomContainer = createBottomContainer(
                            screenWidth,
                            screenHeight,
                            bottomHeight,
                            bottomHeightPercent);
}

std::shared_ptr<Containers> HUD::getSideContainer() const
{
    return getContainer("side_container");
}

std::shared_ptr<Containers> HUD::getBottomContainer() const
{
    return getContainer("bottom_container");
}

std::shared_ptr<Containers> HUD::getSquareContainer() const
{
    return getContainer("square_container");
}

void HUD::initExitButton()
{
    auto squareContainer = getSquareContainer();
    if (!squareContainer)
        return;

    squareContainer->addButtonPercent(
        "exit_button",
        15.0f, 10.0f,
        70.0f, 15.0f,
        "EXIT",
        [this]() {
            this->_display->closeWindow();
        },
        {240, 60, 60, 255},
        {255, 100, 100, 255},
        {200, 40, 40, 255},
        {255, 255, 255, 255}
    );
}

void HUD::initSettingsButton()
{
    auto squareContainer = getSquareContainer();
    if (!squareContainer)
        return;

    squareContainer->addButtonPercent(
        "settings_button",
        15.0f, 30.0f,
        70.0f, 15.0f,
        "SETTINGS",
        []() {
            // Placeholder for settings functionality
        },
        {60, 60, 240, 255},
        {100, 100, 255, 255},
        {40, 40, 200, 255},
        {255, 255, 255, 255}
    );
}

void HUD::initHelpButton()
{
    auto squareContainer = getSquareContainer();
    if (!squareContainer)
        return;

    squareContainer->addButtonPercent(
        "help_button",
        15.0f, 50.0f,
        70.0f, 15.0f,
        "HELP",
        [this]() {
            if (_help && !_help->isVisible()) {
                _help->show();
            }
        },
        {60, 240, 60, 255},
        {100, 255, 100, 255},
        {40, 200, 40, 255},
        {255, 255, 255, 255}
    );
}

void HUD::initCameraResetButton()
{
    auto squareContainer = getSquareContainer();
    if (!squareContainer)
        return;

    squareContainer->addButtonPercent(
        "camera_reset_button",
        15.0f, 70.0f,
        70.0f, 15.0f,
        "RESET CAMERA",
        [this]() {
            this->_display->initCamera();

            if (_resetCameraFunc) {
                _resetCameraFunc();
            }
        },
        {240, 240, 60, 255},
        {255, 255, 100, 255},
        {200, 200, 40, 255},
        {255, 255, 255, 255}
    );
}

void HUD::initTeamPlayersDisplay(std::shared_ptr<GameInfos> gameInfos)
{
    auto sideContainer = getSideContainer();
    if (!sideContainer || !gameInfos)
        return;

    clearTeamDisplayElements(sideContainer);

    const std::vector<std::string> teams = gameInfos->getTeamNames();
    const std::vector<zappy::structs::Player> players = gameInfos->getPlayers();

    float yPos = 5.0f;

    for (int i = static_cast<int>(teams.size()) - 1; i >= 0; i--) {
        std::string teamId = "team_display_" + std::to_string(i);
        const std::string& teamName = teams[i];

        if (i < static_cast<int>(teams.size()) - 1) {
            sideContainer->addTextPercent(
                teamId + "_separator",
                5.0f,
                yPos - 0.5f,
                std::string(90, '-'),
                1.0f,
                {120, 120, 120, 150}
            );
            yPos += 1.0f;
        }

        sideContainer->addTextPercent(
            teamId + "_title",
            5.0f,
            yPos,
            "TEAM: " + teamName,
            3.5f,
            {255, 255, 255, 255}
        );

        yPos += 4.0f;

        std::vector<int> teamPlayerNumbers = getTeamPlayerNumbers(teamName, players);
        addPlayerListText(sideContainer, teamId, yPos, teamPlayerNumbers);

        yPos += 3.0f;
        yPos += 2.0f;
    }
}

void HUD::updateTeamPlayersDisplay(std::shared_ptr<GameInfos> gameInfos)
{
    auto sideContainer = getSideContainer();
    if (!sideContainer || !gameInfos)
        return;

    static std::vector<std::string> lastTeamNames;
    const std::vector<std::string> currentTeamNames = gameInfos->getTeamNames();

    if (lastTeamNames != currentTeamNames) {
        lastTeamNames = currentTeamNames;
        initTeamPlayersDisplay(gameInfos);
        return;
    }

    const std::vector<zappy::structs::Player> players = gameInfos->getPlayers();

    for (int i = static_cast<int>(currentTeamNames.size()) - 1; i >= 0; i--) {
        std::string teamId = "team_display_" + std::to_string(i);
        const std::string& teamName = currentTeamNames[i];

        auto titleElem = std::dynamic_pointer_cast<Text>(
            sideContainer->getElement(teamId + "_title"));

        if (titleElem) {
            float yPos = titleElem->getBounds().y - sideContainer->getBounds().y;
            yPos = yPos / sideContainer->getBounds().height * 100.0f;
            yPos += 4.0f;

            for (int j = 0; j < 50; j++) {
                sideContainer->removeElement(teamId + "_player_" + std::to_string(j));
            }

            std::vector<int> teamPlayerNumbers = getTeamPlayerNumbers(teamName, players);
            addPlayerListText(sideContainer, teamId, yPos, teamPlayerNumbers);
        }
    }
}

void HUD::clearTeamDisplayElements(std::shared_ptr<Containers> container)
{
    if (!container)
        return;

    for (int i = 0; i < 100; i++) {
        std::string idBase = "team_display_" + std::to_string(i);
        container->removeElement(idBase + "_title");
        container->removeElement(idBase + "_separator");
        container->removeElement(idBase + "_stats");

        for (int j = 0; j < 50; j++) {
            container->removeElement(idBase + "_player_" + std::to_string(j));
        }
    }
}

void HUD::clearPlayerInventoryElements()
{
    auto bottomContainer = getBottomContainer();
    if (!bottomContainer)
        return;

    std::vector<std::string> elementIds = {
        "player_info_title",
        "player_info_separator",
        "player_info_level", "player_info_team",
        "player_info_position", "player_info_orientation",
        "inventory_title",
        "inventory_separator",
        "inventory_food", "inventory_linemate", "inventory_deraumere", "inventory_sibur",
        "inventory_mendiane", "inventory_phiras", "inventory_thystame"
    };

    for (const auto& id : elementIds) {
        bottomContainer->removeElement(id);
    }
}

std::vector<int> HUD::getTeamPlayerNumbers(
    const std::string& teamName,
    const std::vector<zappy::structs::Player>& players)
{
    std::vector<int> teamPlayerNumbers;
    for (const auto& player : players) {
        if (player.teamName == teamName)
            teamPlayerNumbers.push_back(player.number);
    }
    return teamPlayerNumbers;
}

std::string HUD::createPlayerListText(const std::vector<int>& playerNumbers)
{
    if (playerNumbers.empty())
        return "No players";

    std::string playerList = "Players: ";
    for (size_t j = 0; j < playerNumbers.size(); ++j) {
        playerList += std::to_string(playerNumbers[j]);
        if (j < playerNumbers.size() - 1)
            playerList += ", ";
    }
    return playerList;
}

void HUD::addPlayerListText(
    std::shared_ptr<Containers> container,
    const std::string& teamId,
    float yPos,
    const std::vector<int>& playerNumbers)
{
    if (!container)
        return;

    std::string playerList = createPlayerListText(playerNumbers);

    if (!playerNumbers.empty()) {
        container->addTextPercent(
            teamId + "_player_0",
            10.0f,
            yPos,
            playerList,
            2.2f,
            {200, 200, 200, 255}
        );
    } else {
        container->addTextPercent(
            teamId + "_player_0",
            10.0f,
            yPos,
            playerList,
            2.0f,
            {150, 150, 150, 200}
        );
    }
}

std::shared_ptr<Containers> HUD::createSquareContainer(
    float squareSize,
    float sideWidthPercent)
{
    auto squareContainer = addContainer(
        "square_container",
        0, 0,
        squareSize, squareSize,
        {60, 60, 60, 220}
    );

    if (squareContainer)
        squareContainer->setRelativePosition(0, 0, sideWidthPercent, sideWidthPercent);

    return squareContainer;
}

std::shared_ptr<Containers> HUD::createSideContainer(
    float sideYStart,
    float sideWidth,
    float sideHeight,
    float sideWidthPercent,
    float bottomHeightPercent)
{
    auto sideContainer = addContainer(
        "side_container",
        0, sideYStart,
        sideWidth, sideHeight,
        {40, 40, 40, 200}
    );

    if (sideContainer) {
        sideContainer->setRelativePosition(
            0,
            sideWidthPercent,
            sideWidthPercent,
            100.0f - sideWidthPercent - bottomHeightPercent);
    }

    return sideContainer;
}

std::shared_ptr<Containers> HUD::createBottomContainer(
    int screenWidth,
    int screenHeight,
    float bottomHeight,
    float bottomHeightPercent)
{
    auto bottomContainer = addContainer(
        "bottom_container",
        0, screenHeight - bottomHeight,
        screenWidth, bottomHeight,
        {40, 40, 40, 200}
    );

    if (bottomContainer) {
        bottomContainer->setRelativePosition(
            0,
            100.0f - bottomHeightPercent,
            100.0f, bottomHeightPercent);
    }

    return bottomContainer;
}

std::pair<float, float> HUD::calculateContentMetrics(
    std::shared_ptr<Containers> container,
    const std::unordered_map<std::string, float> &initialYPositions)
{
    FloatRect containerBounds = container->getBounds();
    float maxY = containerBounds.y;

    for (const auto& pair : initialYPositions) {
        auto element = container->getElement(pair.first);
        if (element) {
            FloatRect bounds = element->getBounds();
            float elemBottom = bounds.y + bounds.height;
            if (elemBottom > maxY) maxY = elemBottom;
        }
    }

    float contentHeight = maxY - containerBounds.y;
    float teamCount = 0;

    for (int i = 0; i < 100; i++) {
        if (container->getElement("team_display_" + std::to_string(i) + "_title"))
            teamCount++;
    }

    float extraSpace = teamCount > 10 ? (teamCount - 10) * 20.0f : 100.0f;
    contentHeight += extraSpace;

    return {contentHeight, teamCount};
}

void HUD::updateElementPositions(
    std::shared_ptr<Containers> container,
    const std::unordered_map<std::string, float>& initialYPositions,
    float offset)
{
    for (int i = 0; i < 100; i++) {
        std::string idBase = "team_display_" + std::to_string(i);

        auto separatorElem = container->getElement(idBase + "_separator");
        if (separatorElem && initialYPositions.find(idBase + "_separator") !=
            initialYPositions.end()) {
            float initialY = initialYPositions.at(idBase + "_separator");
            separatorElem->setPosition(separatorElem->getBounds().x, initialY + offset);
        }

        auto titleElem = container->getElement(idBase + "_title");
        if (titleElem && initialYPositions.find(idBase + "_title") !=
            initialYPositions.end()) {
            float initialY = initialYPositions.at(idBase + "_title");
            titleElem->setPosition(titleElem->getBounds().x, initialY + offset);

            auto statsElem = container->getElement(idBase + "_stats");
            if (statsElem && initialYPositions.find(idBase + "_stats") !=
                initialYPositions.end()) {
                float initialStatsY = initialYPositions.at(idBase + "_stats");
                statsElem->setPosition(statsElem->getBounds().x, initialStatsY + offset);
            }
        }

        for (int j = 0; j < 50; j++) {
            std::string playerID = idBase + "_player_" + std::to_string(j);
            auto playerElem = container->getElement(playerID);
            if (playerElem && initialYPositions.find(playerID) !=
                initialYPositions.end()) {
                float initialY = initialYPositions.at(playerID);
                playerElem->setPosition(playerElem->getBounds().x, initialY + offset);
            }
        }
    }
}

zappy::structs::Player HUD::getPlayerById(int playerId) const
{
    const auto& players = _gameInfos->getPlayers();

    for (const auto& player : players) {
        if (player.number == playerId)
            return player;
    }

    return zappy::structs::Player();
}

void HUD::initPlayerInventoryDisplay(int playerId)
{
    auto bottomContainer = getBottomContainer();
    if (!bottomContainer)
        return;

    clearPlayerInventoryElements();

    const zappy::structs::Player player = getPlayerById(playerId);
    if (player.number <= 0)
        return;

    bottomContainer->addTextPercent(
        "player_info_title",
        55.0f, 10.0f,
        "PLAYER INFORMATION",
        8.0f,
        {255, 255, 255, 255}
    );

    bottomContainer->addTextPercent(
        "player_info_separator",
        55.0f, 17.0f,
        std::string(70, '-'),
        2.0f,
        {150, 150, 150, 200}
    );

    bottomContainer->addTextPercent(
        "player_info_team",
        55.0f, 30.0f,
        "Team: " + player.teamName,
        7.0f,
        {220, 220, 220, 255}
    );

    bottomContainer->addTextPercent(
        "player_info_level",
        55.0f, 50.0f,
        "Level: " + std::to_string(player.level),
        7.0f,
        {220, 220, 220, 255}
    );

    std::string orientationStr;
    switch (player.orientation) {
        case 1: orientationStr = "North"; break;
        case 2: orientationStr = "East"; break;
        case 3: orientationStr = "South"; break;
        case 4: orientationStr = "West"; break;
        default: orientationStr = "Unknown";
    }

    bottomContainer->addTextPercent(
        "player_info_position",
        65.0f, 30.0f,
        "Position: [" + std::to_string(player.x) + ", " + std::to_string(player.y) + "]",
        7.0f,
        {220, 220, 220, 255}
    );

    bottomContainer->addTextPercent(
        "player_info_orientation",
        65.0f, 50.0f,
        "Orientation: " + orientationStr,
        7.0f,
        {220, 220, 220, 255}
    );

    bottomContainer->addTextPercent(
        "inventory_title",
        75.0f, 10.0f,
        "INVENTORY",
        8.0f,
        {255, 255, 255, 255}
    );

    bottomContainer->addTextPercent(
        "inventory_separator",
        75.0f, 17.0f,
        std::string(70, '-'),
        2.0f,
        {150, 150, 150, 200}
    );

    bottomContainer->addTextPercent(
        "inventory_food",
        82.0f, 24.0f,
        "Food: " + std::to_string(player.inventory.food),
        7.5f,
        {255, 215, 0, 255}
    );

    float yPosCol1 = 34.0f;
    float xPosCol1 = 75.0f;

    bottomContainer->addTextPercent(
        "inventory_linemate",
        xPosCol1, yPosCol1,
        "Linemate: " + std::to_string(player.inventory.linemate),
        7.0f,
        {200, 200, 200, 255}
    );
    yPosCol1 += 13.0f;

    bottomContainer->addTextPercent(
        "inventory_deraumere",
        xPosCol1, yPosCol1,
        "Deraumere: " + std::to_string(player.inventory.deraumere),
        7.0f,
        {65, 105, 225, 255}
    );
    yPosCol1 += 13.0f;

    bottomContainer->addTextPercent(
        "inventory_sibur",
        xPosCol1, yPosCol1,
        "Sibur: " + std::to_string(player.inventory.sibur),
        7.0f,
        {50, 205, 50, 255}
    );

    float yPosCol2 = 34.0f;
    float xPosCol2 = 88.0f;

    bottomContainer->addTextPercent(
        "inventory_mendiane",
        xPosCol2, yPosCol2,
        "Mendiane: " + std::to_string(player.inventory.mendiane),
        7.0f,
        {255, 165, 0, 255}
    );
    yPosCol2 += 13.0f;

    bottomContainer->addTextPercent(
        "inventory_phiras",
        xPosCol2, yPosCol2,
        "Phiras: " + std::to_string(player.inventory.phiras),
        7.0f,
        {138, 43, 226, 255}
    );
    yPosCol2 += 13.0f;

    bottomContainer->addTextPercent(
        "inventory_thystame",
        xPosCol2, yPosCol2,
        "Thystame: " + std::to_string(player.inventory.thystame),
        7.0f,
        {255, 20, 147, 255}
    );
}

void HUD::updatePlayerInventoryDisplay(int playerId, zappy::gui::CameraMode cameraMode)
{
    auto bottomContainer = getBottomContainer();
    if (!bottomContainer)
        return;

    if (cameraMode != zappy::gui::CameraMode::PLAYER) {
        clearPlayerInventoryElements();
        return;
    }

    const zappy::structs::Player player = getPlayerById(playerId);
    if (player.number <= 0) {
        clearPlayerInventoryElements();
        return;
    }

    auto titleElem = bottomContainer->getElement("player_info_title");
    if (!titleElem) {
        initPlayerInventoryDisplay(playerId);
        return;
    }

    auto levelElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("player_info_level"));
    if (levelElem) {
        levelElem->setText("Level: " + std::to_string(player.level));
    }

    auto posElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("player_info_position"));
    if (posElem) {
        posElem->setText("Position: [" + std::to_string(player.x) + ", " +
                         std::to_string(player.y) + "]");
    }

    std::string orientationStr;
    switch (player.orientation) {
        case 1: orientationStr = "North"; break;
        case 2: orientationStr = "East"; break;
        case 3: orientationStr = "South"; break;
        case 4: orientationStr = "West"; break;
        default: orientationStr = "Unknown";
    }

    auto orientationElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("player_info_orientation"));
    if (orientationElem) {
        orientationElem->setText("Orientation: " + orientationStr);
    }

    auto foodElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("inventory_food"));
    if (foodElem) {
        foodElem->setText("Food: " + std::to_string(player.inventory.food));
    }

    auto linemateElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("inventory_linemate"));
    if (linemateElem) {
        linemateElem->setText("Linemate: " + std::to_string(player.inventory.linemate));
    }

    auto deraumereElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("inventory_deraumere"));
    if (deraumereElem) {
        deraumereElem->setText("Deraumere: " + std::to_string(player.inventory.deraumere));
    }

    auto siburElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("inventory_sibur"));
    if (siburElem) {
        siburElem->setText("Sibur: " + std::to_string(player.inventory.sibur));
    }

    auto mendianeElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("inventory_mendiane"));
    if (mendianeElem) {
        mendianeElem->setText("Mendiane: " + std::to_string(player.inventory.mendiane));
    }

    auto phirasElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("inventory_phiras"));
    if (phirasElem) {
        phirasElem->setText("Phiras: " + std::to_string(player.inventory.phiras));
    }

    auto thystameElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("inventory_thystame"));
    if (thystameElem) {
        thystameElem->setText("Thystame: " + std::to_string(player.inventory.thystame));
    }
}

void HUD::setResetCameraCallback(std::function<void()> resetFunc)
{
    _resetCameraFunc = resetFunc;
}
