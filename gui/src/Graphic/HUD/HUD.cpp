/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** HUD
*/

#include <string>
#include <memory>
#include "../../Utils/Constants.hpp"
#include "HUD.hpp"

HUD::HUD(std::shared_ptr<RayLib> raylib)
    : _containers(), _raylib(raylib)
{
    initDefaultLayout(15.0f, 20.0f);
    initExitButton();
    initSettingsButton();
    initHelpButton();
    initCameraResetButton();
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

    auto sideContainer = addContainer(
        "side_container",
        0, 0,
        sideWidth, screenHeight,
        {40, 40, 40, 200}
    );

    if (sideContainer) {
        sideContainer->setRelativePosition(0, 0, sideWidthPercent, 100.0f);
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

    for (int i = 0; i < 100; i++) {
        std::string idBase = "team_display_" + std::to_string(i);
        sideContainer->removeElement(idBase + "_title");

        for (int j = 0; j < 50; j++) {
            sideContainer->removeElement(idBase + "_player_" + std::to_string(j));
        }
    }

    const std::vector<std::string> teams = gameInfos->getTeamNames();
    const std::vector<zappy::structs::Player> players = gameInfos->getPlayers();

    float yPos = 15.0f;

    for (size_t i = 0; i < teams.size(); i++) {
        std::string teamId = "team_display_" + std::to_string(i);
        const std::string& teamName = teams[i];

        sideContainer->addTextPercent(
            teamId + "_title",
            5.0f,
            yPos,
            "TEAM: " + teamName,
            6.0f,
            {255, 255, 255, 255}
        );

        yPos += 8.0f;

        int playerCount = 0;
        for (const auto& player : players) {
            if (player.teamName == teamName) {
                sideContainer->addTextPercent(
                    teamId + "_player_" + std::to_string(playerCount),
                    10.0f,
                    yPos,
                    "Player " + std::to_string(player.number) + " (Level " + std::to_string(player.level) + ")",
                    5.0f,
                    {200, 200, 200, 255}
                );

                yPos += 6.0f;
                playerCount++;
            }
        }

        if (playerCount == 0) {
            sideContainer->addTextPercent(
                teamId + "_player_0",
                10.0f,
                yPos,
                "No players",
                5.0f,
                {150, 150, 150, 255}
            );

            yPos += 6.0f;
        }

        yPos += 4.0f;
    }
}

void HUD::updateTeamPlayersDisplay(std::shared_ptr<GameInfos> gameInfos)
{
    initTeamPlayersDisplay(gameInfos);
}
