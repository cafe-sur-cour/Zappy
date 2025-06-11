/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Help
*/

#include "Help.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include "../../../Utils/Constants.hpp"

Help::Help(std::shared_ptr<IDisplay> display, std::shared_ptr<IAudio> audio)
    : _display(display), _audio(audio), _helpContainer(nullptr), _visible(false)
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
    if (!_display)
        return;

    std::pair<int, int> screenSize = this->_display->getScreenSize();
    int screenWidth = screenSize.first;
    int screenHeight = screenSize.second;

    float width = screenWidth * 0.50f;
    float height = screenHeight * 0.30f;
    float x = (screenWidth - width) / 2;
    float y = (screenHeight - height) / 2;

    _helpContainer = std::make_shared<Containers>(
        _display,
        _audio,
        x, y,
        width, height,
        Color32{50, 50, 50, 240}
    );

    _helpContainer->setRelativePosition(
        17.5f, 20.0f,
        40.0f, 60.0f
    );

    _helpContainer->addTextPercent(
        "help_title",
        5.0f, 2.0f,
        zappy::constants::HELP_TITLE,
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
        zappy::constants::HELP_SECTION_1,
        3.5f,
        {230, 230, 120, 255}
    );
    yPos += 5.0f;

    _helpContainer->addTextPercent(
        "help_section1_content",
        7.0f, yPos,
        zappy::constants::HELP_SECTION_1_CONTENT,
        2.5f,
        {220, 220, 220, 255}
    );
    yPos += 10.0f;

    _helpContainer->addTextPercent(
        "help_section2_title",
        5.0f, yPos,
        zappy::constants::HELP_SECTION_2,
        3.5f,
        {230, 230, 120, 255}
    );
    yPos += 5.0f;

    _helpContainer->addTextPercent(
        "help_section2_content",
        7.0f, yPos,
        zappy::constants::HELP_SECTION_2_CONTENT,
        2.5f,
        {220, 220, 220, 255}
    );
    yPos += 28.0f;

    _helpContainer->addTextPercent(
        "help_section3_title",
        5.0f, yPos,
        zappy::constants::HELP_SECTION_3,
        3.5f,
        {230, 230, 120, 255}
    );
    yPos += 5.0f;

    _helpContainer->addTextPercent(
        "help_section3_content",
        7.0f, yPos,
        zappy::constants::HELP_SECTION_3_CONTENT,
        2.5f,
        {220, 220, 220, 255}
    );
    yPos += 12.0f;

    _helpContainer->addTextPercent(
        "help_section4_title",
        5.0f, yPos,
        zappy::constants::HELP_SECTION_4,
        3.5f,
        {230, 230, 120, 255}
    );
    yPos += 5.0f;

    _helpContainer->addTextPercent(
        "help_section4_content",
        7.0f, yPos,
        zappy::constants::HELP_SECTION_4_CONTENT,
        2.5f,
        {220, 220, 220, 255}
    );

    _visible = false;
}
