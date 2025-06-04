/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** HUD
*/

#include <string>
#include <memory>
#include <vector>
#include "../../Utils/Constants.hpp"
#include "HUD.hpp"

HUD::HUD(std::shared_ptr<RayLib> raylib, std::shared_ptr<GameInfos> gameInfos)
    : _containers(), _raylib(raylib), _gameInfos(gameInfos)
{
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
    for (auto& pair : _containers) {
        pair.second->draw();
    }
}

void HUD::update()
{
    for (auto& pair : _containers) {
        pair.second->update();
    }
    updateTeamPlayersDisplay(_gameInfos);
}

std::shared_ptr<Containers> HUD::addContainer(
    const std::string& id,
    float x, float y,
    float width, float height,
    Color backgroundColor
)
{
    if (_containers.find(id) != _containers.end())
        return nullptr;

    auto container = std::make_shared<Containers>(
        _raylib,
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

    int screenHeight = _raylib->getScreenHeight();
    int screenWidth = _raylib->getScreenWidth();

    float sideWidth = (screenWidth * sideWidthPercent) / 100.0f;
    float bottomHeight = (screenHeight * bottomHeightPercent) / 100.0f;
    float squareSize = sideWidth;

    auto squareContainer = addContainer(
        "square_container",
        0, 0,
        squareSize, squareSize,
        {60, 60, 60, 220}
    );

    if (squareContainer) {
        squareContainer->setRelativePosition(0, 0, sideWidthPercent, sideWidthPercent);
    }

    float sideYStart = squareSize;
    float sideHeight = screenHeight - squareSize - bottomHeight;

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

        sideContainer->addScrollBarPercent(
            "side_scrollbar",
            92.0f, 0.0f,
            100.0f, 2.0f,
            ScrollBarOrientation::VERTICAL,
            [sideContainer](float value) {
                static std::unordered_map<std::string, float> initialYPositions;
                static float lastContainerHeight = 0;
                Rectangle containerBounds = sideContainer->getBounds();
                float containerHeight = containerBounds.height;

                bool positionsNeedUpdate = lastContainerHeight != containerHeight;

                if (positionsNeedUpdate || initialYPositions.empty()) {
                    initialYPositions.clear();
                    positionsNeedUpdate = false;
                    lastContainerHeight = containerHeight;

                    for (int i = 0; i < 100; i++) {
                        std::string idBase = "team_display_" + std::to_string(i);

                        auto separatorElem = sideContainer->getElement(idBase + "_separator");
                        if (separatorElem) {
                            initialYPositions[idBase + "_separator"] = separatorElem->getBounds().y;
                        }

                        auto titleElem = sideContainer->getElement(idBase + "_title");
                        if (titleElem) {
                            initialYPositions[idBase + "_title"] = titleElem->getBounds().y;

                            auto statsElem = sideContainer->getElement(idBase + "_stats");
                            if (statsElem) {
                                initialYPositions[idBase + "_stats"] = statsElem->getBounds().y;
                            }

                            for (int j = 0; j < 50; j++) {
                                std::string playerID = idBase + "_player_" + std::to_string(j);
                                auto playerElem = sideContainer->getElement(playerID);
                                if (playerElem) {
                                    initialYPositions[playerID] = playerElem->getBounds().y;
                                }
                            }
                        }
                    }
                }

                float maxY = containerBounds.y;

                for (const auto& pair : initialYPositions) {
                    auto element = sideContainer->getElement(pair.first);
                    if (element) {
                        Rectangle bounds = element->getBounds();
                        float elemBottom = bounds.y + bounds.height;
                        if (elemBottom > maxY) maxY = elemBottom;
                    }
                }

                float contentHeight = maxY - containerBounds.y;
                float teamCount = 0;
                for (int i = 0; i < 100; i++) {
                    if (sideContainer->getElement("team_display_" + std::to_string(i) + "_title"))
                        teamCount++;
                }

                float extraSpace = teamCount > 10 ? (teamCount - 10) * 20.0f : 100.0f;
                contentHeight += extraSpace;
                float scrollableDistance = contentHeight - containerHeight;

                if (scrollableDistance <= 0.01f)
                    return;

                float scrollFactor = 1.0f + (teamCount / 20.0f);
                float offset = -value * scrollableDistance * scrollFactor;

                for (int i = 0; i < 100; i++) {
                    std::string idBase = "team_display_" + std::to_string(i);

                    auto separatorElem = sideContainer->getElement(idBase + "_separator");
                    if (separatorElem && initialYPositions.find(idBase + "_separator") != initialYPositions.end()) {
                        float initialY = initialYPositions[idBase + "_separator"];
                        separatorElem->setPosition(separatorElem->getBounds().x, initialY + offset);
                    }

                    auto titleElem = sideContainer->getElement(idBase + "_title");
                    if (titleElem && initialYPositions.find(idBase + "_title") != initialYPositions.end()) {
                        float initialY = initialYPositions[idBase + "_title"];
                        titleElem->setPosition(titleElem->getBounds().x, initialY + offset);

                        auto statsElem = sideContainer->getElement(idBase + "_stats");
                        if (statsElem && initialYPositions.find(idBase + "_stats") != initialYPositions.end()) {
                            float initialStatsY = initialYPositions[idBase + "_stats"];
                            statsElem->setPosition(statsElem->getBounds().x, initialStatsY + offset);
                        }
                    }

                    for (int j = 0; j < 50; j++) {
                        std::string playerID = idBase + "_player_" + std::to_string(j);
                        auto playerElem = sideContainer->getElement(playerID);
                        if (playerElem && initialYPositions.find(playerID) != initialYPositions.end()) {
                            float initialY = initialYPositions[playerID];
                            playerElem->setPosition(playerElem->getBounds().x, initialY + offset);
                        }
                    }
                }
            }
        );
    }

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
            _raylib->closeWindow();
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
        []() {
            // Placeholder for help functionality
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
        []() {
            // Placeholder for camera reset functionality
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

    // Clear existing elements
    for (int i = 0; i < 100; i++) {
        std::string idBase = "team_display_" + std::to_string(i);
        sideContainer->removeElement(idBase + "_title");
        sideContainer->removeElement(idBase + "_separator");
        sideContainer->removeElement(idBase + "_stats");

        for (int j = 0; j < 50; j++) {
            sideContainer->removeElement(idBase + "_player_" + std::to_string(j));
        }
    }

    const std::vector<std::string> teams = gameInfos->getTeamNames();
    const std::vector<zappy::structs::Player> players = gameInfos->getPlayers();

    auto scrollbar = std::dynamic_pointer_cast<ScrollBar>(sideContainer->getElement("side_scrollbar"));
    if (scrollbar)
        scrollbar->setValue(0.0f);

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

        std::vector<int> teamPlayerNumbers;
        for (const auto& player : players) {
            if (player.teamName == teamName) {
                teamPlayerNumbers.push_back(player.number);
            }
        }

        if (!teamPlayerNumbers.empty()) {
            std::string playerList = "Players: ";
            for (size_t j = 0; j < teamPlayerNumbers.size(); ++j) {
                playerList += std::to_string(teamPlayerNumbers[j]);
                if (j < teamPlayerNumbers.size() - 1) {
                    playerList += ", ";
                }
            }

            sideContainer->addTextPercent(
                teamId + "_player_0",
                10.0f,
                yPos,
                playerList,
                2.2f,
                {200, 200, 200, 255}
            );
            yPos += 3.0f;
        } else {
            sideContainer->addTextPercent(
                teamId + "_player_0",
                10.0f,
                yPos,
                "No players",
                2.0f,
                {150, 150, 150, 200}
            );
            yPos += 3.0f;
        }

        yPos += 2.0f;
    }

    if (scrollbar) {
        scrollbar->setValue(0.0f);

        float numTeams = static_cast<float>(teams.size());
        float baseRatio = 90.0f / (numTeams * 15.0f);

        if (numTeams > 10) {
            baseRatio *= 0.5f;
        }

        float contentRatio = std::max(0.01f, std::min(1.0f, baseRatio));
        scrollbar->setHandleSize(contentRatio);
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
    auto scrollbar = std::dynamic_pointer_cast<ScrollBar>(sideContainer->getElement("side_scrollbar"));

    if (scrollbar) {
        float maxTeams = static_cast<float>(currentTeamNames.size());
        float baseHeight = 35.0f;
        float totalContentHeight = maxTeams * baseHeight;
        float viewportHeight = sideContainer->getBounds().height;
        float viewportRatio = std::min(1.0f, viewportHeight / totalContentHeight);

        if (maxTeams > 10) {
            viewportRatio *= 0.8f;
        }

        viewportRatio = std::max(0.05f, viewportRatio);
        scrollbar->setHandleSize(viewportRatio);
    }

    float yPos = 5.0f;

    for (int i = static_cast<int>(currentTeamNames.size()) - 1; i >= 0; i--) {
        std::string teamId = "team_display_" + std::to_string(i);
        const std::string& teamName = currentTeamNames[i];

        auto titleElem = std::dynamic_pointer_cast<Text>(sideContainer->getElement(teamId + "_title"));
        if (titleElem) {
            yPos = titleElem->getBounds().y - sideContainer->getBounds().y;
            yPos = yPos / sideContainer->getBounds().height * 100.0f;
            yPos += 4.0f;

            for (int j = 0; j < 50; j++) {
                sideContainer->removeElement(teamId + "_player_" + std::to_string(j));
            }

            std::vector<int> teamPlayerNumbers;
            for (const auto& player : players) {
                if (player.teamName == teamName) {
                    teamPlayerNumbers.push_back(player.number);
                }
            }

            if (!teamPlayerNumbers.empty()) {
                std::string playerList = "Players: ";
                for (size_t j = 0; j < teamPlayerNumbers.size(); ++j) {
                    playerList += std::to_string(teamPlayerNumbers[j]);
                    if (j < teamPlayerNumbers.size() - 1) {
                        playerList += ", ";
                    }
                }

                sideContainer->addTextPercent(
                    teamId + "_player_0",
                    10.0f,
                    yPos,
                    playerList,
                    2.2f,
                    {200, 200, 200, 255}
                );

                yPos += 3.0f;
            } else {
                sideContainer->addTextPercent(
                    teamId + "_player_0",
                    10.0f,
                    yPos,
                    "No players",
                    2.0f,
                    {150, 150, 150, 200}
                );

                yPos += 3.0f;
            }

            yPos += 2.0f;
        }
    }
    if (scrollbar && yPos > 90.0f) {
        float contentRatio = 90.0f / (yPos + 50.0f);
        contentRatio = std::max(0.02f, std::min(0.9f, contentRatio));
        scrollbar->setHandleSize(contentRatio);
    } else if (scrollbar) {
        scrollbar->setHandleSize(1.0f);
    }
}


