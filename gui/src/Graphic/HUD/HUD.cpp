/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** HUD
*/

#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include "../../Utils/Constants.hpp"
#include "HUD.hpp"

HUD::HUD(std::shared_ptr<IDisplay> display, std::shared_ptr<GameInfos> gameInfos,
         std::shared_ptr<IAudio> audio,
         std::shared_ptr<CameraManager> camera, std::function<void()> resetCameraFunc)
    : _containers(), _display(display), _gameInfos(gameInfos), _audio(audio), _camera(camera),
      _resetCameraFunc(resetCameraFunc)
{
    _help = std::make_shared<Help>(display, audio);
    _settings = std::make_shared<Settings>(display, audio, camera);
    initDefaultLayout(15.0f, 20.0f);
    initExitButton();
    initSettingsButton();
    initHelpButton();
    initCameraResetButton();
    initTeamPlayersDisplay(_gameInfos);
    initTpsSlider(_gameInfos, _display, _audio);
    this->_initHelpInformation();

    if (_gameInfos)
        _gameInfos->addObserver(std::shared_ptr<IObserver>(this, [](IObserver*) {}));
}

HUD::~HUD()
{
    _containers.clear();
}

void HUD::draw()
{
    for (const auto &pair : _containers) {
        pair.second->draw();
    }

    if (_help && _help->isVisible()) {
        _help->draw();
    }
    if (this->_settings && this->_settings->isVisible()) {
        this->_settings->draw();
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

    if (this->_settings) {
        this->_settings->update();
    }

    updateTeamPlayersDisplay(_gameInfos);
    updateTpsSlider(_gameInfos);
    updateGameMessages();
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

    if (this->_settings) {
        this->_settings->handleResize(oldWidth, oldHeight, newWidth, newHeight);
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

    auto tpsContainer = createTpsContainer(
                            screenWidth,
                            screenHeight,
                            bottomHeight,
                            bottomHeightPercent);
}

void HUD::_initHelpInformation()
{
    auto bottomContainer = this->getBottomContainer();

    bottomContainer->addTextPercent(
        "cam_info_mode",
        2.f, 10.f,
        "Camera mod: NONE",
        7.0f,
        {245, 224, 80, 255}
    );

    bottomContainer->addTextPercent(
        "help_cam_key",
        2.f, 30.f,
        "NONE",
        7.0f,
        {255, 236, 183, 255}
    );
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

std::shared_ptr<Containers> HUD::getTpsContainer() const
{
    return getContainer("tps_container");
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
        [this]() {
            if (this->_settings && !this->_settings->isVisible() &&
                !this->_help->isVisible()) {
                this->_settings->show();
            }
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
            if (this->_help && !this->_help->isVisible() && !this->_settings->isVisible()) {
                this->_help->show();
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
        {245, 224, 80, 255},
        {255, 235, 120, 255},
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
        "player_info_id", "player_info_ritual",
        "player_info_position", "player_info_orientation",
        "inventory_title",
        "inventory_separator",
        "inventory_food", "inventory_linemate", "inventory_deraumere", "inventory_sibur",
        "inventory_mendiane", "inventory_phiras", "inventory_thystame",
        "kill_player_button"
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

std::shared_ptr<Containers> HUD::createTpsContainer(
    int screenWidth,
    int screenHeight,
    float bottomHeight,
    float bottomHeightPercent)
{
    float containerWidth = 200.0f;
    float containerHeight = 120.0f;
    (void)bottomHeight;
    (void)bottomHeightPercent;

    auto tpsContainer = addContainer(
        "tps_container",
        screenWidth - containerWidth, 0,
        containerWidth, containerHeight,
        {40, 40, 40, 200}
    );

    if (tpsContainer) {
        float widthPercent = (containerWidth / screenWidth) * 100.0f;
        float heightPercent = (containerHeight / screenHeight) * 100.0f;

        tpsContainer->setRelativePosition(
            100.0f - widthPercent,
            0,
            widthPercent,
            heightPercent);
    }

    return tpsContainer;
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
        "player_info_id",
        55.0f, 30.0f,
        "ID: " + std::to_string(player.number),
        7.0f,
        {220, 220, 220, 255}
    );

    bottomContainer->addTextPercent(
        "player_info_team",
        55.0f, 45.0f,
        "Team: " + player.teamName,
        7.0f,
        {220, 220, 220, 255}
    );

    bottomContainer->addTextPercent(
        "player_info_level",
        55.0f, 60.0f,
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
        65.0f, 45.0f,
        "Orientation: " + orientationStr,
        7.0f,
        {220, 220, 220, 255}
    );

    bool inRitual = isPlayerInIncantation(player.number);
    bottomContainer->addTextPercent(
        "player_info_ritual",
        65.0f, 60.0f,
        "Ritual: " + std::string(inRitual ? "Yes" : "No"),
        7.0f,
        inRitual ? Color32{255, 215, 0, 255} : Color32{220, 220, 220, 255}
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

    bottomContainer->addButtonPercent(
        "kill_player_button",
        87.5f, 6.5f,
        5.0f, 10.0f,
        "KILL",
        [this]() {
            _gameInfos->killPlayer(0);
        },
        {200, 50, 50, 255},
        {255, 80, 80, 255},
        {150, 30, 30, 255},
        {255, 255, 255, 255}
    );
}

std::string HUD::_camModeToText(zappy::gui::CameraMode cameraMode, bool isGamePadAvailable)
{
    std::string changeCamKey = "";
    changeCamKey = isGamePadAvailable ? "LB" : "TAB";
    switch (cameraMode)
    {
    case zappy::gui::CameraMode::FREE:
        return "Free\nChange camera mod: " + changeCamKey;
    case zappy::gui::CameraMode::PLAYER:
        return "Player\nChange camera mod: " + changeCamKey;
    case zappy::gui::CameraMode::TARGETED:
        return "Targeted\nChange camera mod: " + changeCamKey;
    default:
        return "Unknown\nChange camera mod: " + changeCamKey;
    }
    return "Unknown\nChange camera mod: " + changeCamKey;
}

std::string HUD::_camKeyHelp(zappy::gui::CameraMode cameraMode, bool isGamePadAvailable)
{
    if (isGamePadAvailable) {
        switch (cameraMode)
        {
        case zappy::gui::CameraMode::FREE:
            return "Right joystick = Change camera direction\n\n"
                     "Left joystick = Move camera x and z\n"
                     "RT | LT = Move camera y\n\n"
                     "H = Toggle HUD\n";
        case zappy::gui::CameraMode::PLAYER:
            return "UP | DOWN = Next / Previous player\n\n"
                     "Right joystick = Change camera direction\n\n"
                     "H = Toggle HUD\n";
        case zappy::gui::CameraMode::TARGETED:
            return "Right joystick = Rotate camera around map origin\n\n"
                     "RT | LT = Zoom / Unzoom\n\n"
                     "H = Toggle HUD\n";
        default:
            return "Unknown";
        }
    }
    switch (cameraMode)
    {
    case zappy::gui::CameraMode::FREE:
        return "Z | Q | S | D = Move camera\n\n"
                 "UP | DOWN | RIGHT | LEFT = Change camera direction\n\n"
                 "H = Toggle HUD\n";
    case zappy::gui::CameraMode::PLAYER:
        return "UP | DOWN = Next / Previous player\n\n"
                 "RIGHT | LEFT = Change camera direction\n\n"
                 "H = Toggle HUD\n";
    case zappy::gui::CameraMode::TARGETED:
        return "UP | DOWN | RIGHT | LEFT = Rotate camera around map origin\n\n"
                 "RT | LT = Zoom / Unzoom\n\n"
                 "H = Toggle HUD\n";
    default:
        return "Unknown";
    }
    return "Unknown";
}

void HUD::updateHelpInformationHUD(zappy::gui::CameraMode cameraMode)
{
    auto bottomContainer = getBottomContainer();
    if (!bottomContainer)
        return;

    auto camInfoElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("cam_info_mode"));
    auto strCamMode = this->_camModeToText(cameraMode, this->_display->isGamepadAvailable());
    if (camInfoElem && camInfoElem->getText() != "Camera mod: " + strCamMode) {
        camInfoElem->setText("Camera mod: " + strCamMode);
        return;
    }
    auto keyHelp = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("help_cam_key"));
    auto camHelpKey = this->_camKeyHelp(cameraMode, this->_display->isGamepadAvailable());
    if (keyHelp && keyHelp->getText() != camHelpKey) {
        keyHelp->setText(camHelpKey);
        return;
    }
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

    auto killButton = std::dynamic_pointer_cast<Button>(
        bottomContainer->getElement("kill_player_button"));
    if (killButton) {
        killButton->setCallback([this, playerId]() {
            _gameInfos->killPlayer(playerId);
        });
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

    auto teamElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("player_info_team"));
    if (teamElem) {
        teamElem->setText("Team: " + player.teamName);
    }

    auto idElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("player_info_id"));
    if (idElem) {
        idElem->setText("ID: " + std::to_string(player.number));
    }

    bool inRitual = isPlayerInIncantation(player.number);
    auto ritualElem = std::dynamic_pointer_cast<Text>(
        bottomContainer->getElement("player_info_ritual"));
    if (ritualElem) {
        ritualElem->setText("Ritual: " + std::string(inRitual ? "Yes" : "No"));
        ritualElem->setColor(inRitual ? Color32{255, 215, 0, 255} :
                                        Color32{220, 220, 220, 255});
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

void HUD::initTpsSlider(
    std::shared_ptr<GameInfos> gameInfos,
    std::shared_ptr<IDisplay> display,
    std::shared_ptr<IAudio> audio)
{
    auto tpsContainer = getTpsContainer();
    if (!tpsContainer || !gameInfos || !display || !audio)
        return;

    tpsContainer->addSliderPercent(
        "tps_slider", 5.0f, 15.0f, 150.0f, 100.0f,
        1.0f, 250.0f, gameInfos->getTimeUnit(),
        "Frequency (TPS): ",
        [gameInfos, display, audio](float value) {
            gameInfos->setTimeUnit(static_cast<int>(value), true);
        }
    );
}

void HUD::updateTpsSlider(std::shared_ptr<GameInfos> gameInfos)
{
    auto tpsContainer = getTpsContainer();
    if (!tpsContainer || !gameInfos)
        return;

    auto slider = std::dynamic_pointer_cast<Slider>(
        tpsContainer->getElement("tps_slider"));

    if (!slider)
        return;

    static std::chrono::steady_clock::time_point lastUpdateTime;
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = now - lastUpdateTime;
    if (elapsed.count() < 5.0f)
        return;

    lastUpdateTime = now;
    if (slider->isDragging())
        return;

    slider->setValue(static_cast<float>(gameInfos->getTimeUnit()));
}

bool HUD::isPlayerInIncantation(int playerId) const
{
    const auto& incantations = _gameInfos->getIncantations();

    for (const auto& incantation : incantations) {
        for (int id : incantation.players) {
            if (id == playerId) {
                return true;
            }
        }
    }

    return false;
}

void HUD::createMessageContainer()
{
    if (_containers.find("message_container") != _containers.end()) {
        _containers.erase("message_container");
    }

    int screenWidth = _display->getScreenSize().x;
    int screenHeight = _display->getScreenSize().y;

    float containerWidth = screenWidth * 0.3f;
    float containerHeight = screenHeight * 0.2f;
    float containerX = (screenWidth - containerWidth) / 2.0f;
    float containerY = (screenHeight - containerHeight) / 2.0f;

    Color32 backgroundColor = {0, 0, 0, 180};

    auto container = addContainer("message_container", containerX, containerY,
                containerWidth, containerHeight, backgroundColor);

    if (container) {
        float xPercent = (containerX / screenWidth) * 100.0f;
        float yPercent = (containerY / screenHeight) * 100.0f;
        float widthPercent = (containerWidth / screenWidth) * 100.0f;
        float heightPercent = (containerHeight / screenHeight) * 100.0f;

        container->setRelativePosition(xPercent, yPercent, widthPercent, heightPercent);
    }
}

void HUD::displayWinMessage(const std::string& teamName)
{
    try {
        createMessageContainer();

        std::string msgId = "win_message_" + std::to_string(
            std::chrono::steady_clock::now().time_since_epoch().count());
        std::string message = "Team " + teamName + " WINS!";

        auto container = getContainer("message_container");
        if (container) {
            try {
                for (auto& msg : _gameMessages) {
                    container->removeElement(msg.id);
                }
                _gameMessages.clear();
            } catch (const std::exception& e) {
                std::cout << colors::T_RED << "[ERROR] Exception clearing messages: "
                          << e.what() << colors::RESET << std::endl;
            }

            Color32 winColor = {50, 255, 50, 255};

            try {
                float fontSize = 30.0f;

                auto titleElement = container->addTextPercent(
                    "win_title", 20.0f, 25.0f, "VICTORY!", fontSize, winColor);

                auto teamElement = container->addTextPercent(
                    msgId, 20.0f, 50.0f, message, fontSize - 5.0f, winColor);

                _gameMessages.push_back({
                    msgId,
                    message,
                    winColor,
                    std::chrono::steady_clock::now(),
                    MESSAGE_DURATION * 2
                });
            } catch (const std::exception& e) {
                std::cout << colors::T_RED << "[ERROR] Exception adding win message: "
                          << e.what() << colors::RESET << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cout << colors::T_RED << "[ERROR] Exception in displayWinMessage: "
                  << e.what() << colors::RESET << std::endl;
    } catch (...) {
        std::cout << colors::T_RED << "[ERROR] Unknown exception in displayWinMessage"
                  << colors::RESET << std::endl;
    }
}


void HUD::updateGameMessages()
{
    auto currentTime = std::chrono::steady_clock::now();
    auto container = getContainer("message_container");

    if (!container || _gameMessages.empty()) {
        return;
    }

    auto it = _gameMessages.begin();
    while (it != _gameMessages.end()) {
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            currentTime - it->startTime).count();

        if (elapsed >= it->duration) {
            container->removeElement(it->id);
            it = _gameMessages.erase(it);
        } else {
            if (elapsed > it->duration - 1.0f) {
                float alpha = 255.0f * (1.0f - (elapsed - (it->duration - 1.0f)));
                if (auto textElem = std::dynamic_pointer_cast<Text>(
                    container->getElement(it->id))) {
                    Color32 color = it->color;
                    color.a = static_cast<unsigned char>(alpha);
                    textElem->setColor(color);
                }
            }
            ++it;
        }
    }

    if (_gameMessages.empty() && container) {
        _containers.erase("message_container");
    }
}

void HUD::onGameEvent(GameEventType eventType, const std::string& teamName)
{
    switch (eventType) {
        case GameEventType::TEAM_WIN:
            displayWinMessage(teamName);
            break;
        default:
            break;
    }
}
