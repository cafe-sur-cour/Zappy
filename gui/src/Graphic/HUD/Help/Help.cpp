/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Help
*/

#include "Help.hpp"
#include <iostream>
#include <vector>
#include <fstream>

Help::Help(std::shared_ptr<RayLib> raylib, std::shared_ptr<Audio> audio)
    : _raylib(raylib), _audio(audio), _helpContainer(nullptr), _visible(false)
{
    initHelpContainer();
}

void Help::show()
{
    _visible = true;
}

void Help::hide()
{
    _visible = false;
}

bool Help::isVisible() const
{
    return _visible;
}

void Help::update()
{
    if (!_visible || !_helpContainer)
        return;

    _helpContainer->update();
}

void Help::draw()
{
    if (!_visible || !_helpContainer)
        return;

    _helpContainer->draw();
}

void Help::handleResize(int oldWidth, int oldHeight, int newWidth, int newHeight)
{
    if (!_helpContainer)
        return;

    _helpContainer->handleResize(oldWidth, oldHeight, newWidth, newHeight);
}

void Help::initHelpContainer()
{
    if (!_raylib)
        return;

    int screenWidth = _raylib->getScreenWidth();
    int screenHeight = _raylib->getScreenHeight();

    float width = screenWidth * 0.50f;
    float height = screenHeight * 0.30f;
    float x = (screenWidth - width) / 2;
    float y = (screenHeight - height) / 2;

    _helpContainer = std::make_shared<Containers>(
        _raylib,
        _audio,
        x, y,
        width, height,
        Color{50, 50, 50, 240}
    );

    _helpContainer->setRelativePosition(
        17.5f, 20.0f,
        40.0f, 60.0f
    );

    _helpContainer->addTextPercent(
        "help_title",
        5.0f, 2.0f,
        "HELP",
        6.0f,
        {255, 255, 255, 255}
    );

    _helpContainer->addButtonPercent(
        "help_close_button",
        92.0f, 2.0f,
        6.0f, 6.0f,
        "X",
        [this]() {
            this->hide();
            if (_audio) _audio->playSound("click");
        },
        {200, 60, 60, 255},
        {255, 100, 100, 255},
        {180, 40, 40, 255},
        {255, 255, 255, 255}
    );

    _helpContainer->addTextPercent(
        "help_separator",
        2.0f, 8.0f,
        std::string(100, '-'),
        1.0f,
        {150, 150, 150, 200}
    );

    float yPos = 12.0f;

    _helpContainer->addTextPercent(
        "help_section1_title",
        5.0f, yPos,
        "Game Overview",
        3.5f,
        {230, 230, 120, 255}
    );
    yPos += 5.0f;

    _helpContainer->addTextPercent(
        "help_section1_content",
        7.0f, yPos,
        "Zappy is a game where AI-controlled players compete to collect resources\n"
        "and level up on a dynamically changing map. The GUI allows you to visualize\n"
        "the game state, players, and resources in real-time.",
        2.5f,
        {220, 220, 220, 255}
    );
    yPos += 10.0f;

    _helpContainer->addTextPercent(
        "help_section2_title",
        5.0f, yPos,
        "Controls",
        3.5f,
        {230, 230, 120, 255}
    );
    yPos += 5.0f;

    _helpContainer->addTextPercent(
        "help_section2_content",
        7.0f, yPos,
        "Camera Movement:\n"
        "  - WASD: Move camera\n"
        "  - Right mouse button + drag: Rotate camera\n\n"
        "Interface:\n"
        "  - Click on players to see their stats\n"
        "  - Use the RESET CAMERA button to return to default view\n"
        "  - Use the Settings button to adjust game settings",
        2.5f,
        {220, 220, 220, 255}
    );
    yPos += 25.0f;

    _helpContainer->addTextPercent(
        "help_section3_title",
        5.0f, yPos,
        "Teams & Players",
        3.5f,
        {230, 230, 120, 255}
    );
    yPos += 5.0f;

    _helpContainer->addTextPercent(
        "help_section3_content",
        7.0f, yPos,
        "The left panel shows all teams and their player IDs.\n"
        "Players have different levels based on collected resources.\n"
        "The team that first gets a player to level 8 wins the game.",
        2.5f,
        {220, 220, 220, 255}
    );
    yPos += 12.0f;

    _helpContainer->addTextPercent(
        "help_section4_title",
        5.0f, yPos,
        "Resources",
        3.5f,
        {230, 230, 120, 255}
    );
    yPos += 5.0f;

    _helpContainer->addTextPercent(
        "help_section4_content",
        7.0f, yPos,
        "Resources on the map are represented by different colored objects.\n"
        "Players collect these resources to perform rituals and level up.",
        2.5f,
        {220, 220, 220, 255}
    );

    _visible = false;
}
