/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Settings
*/

#include "Settings.hpp"
#include <iostream>
#include <string>
#include <memory>
#include "Utils/HelpText.hpp"

void Settings::show()
{
    this->_visible = true;
}

void Settings::hide()
{
    this->_visible = false;
}

bool Settings::isVisible() const
{
    return this->_visible;
}

void Settings::update()
{
    if (!this->_visible || !this->_settingsContainer)
        return;

    this->_settingsContainer->update();
    if (this->_audio->getMusicVolumeLevel() != this->_musicLevel)
        this->_audio->setMusicVolumeLevel(this->_musicLevel);
    if (this->_audio->getSFXVolumeLevel() != this->_sfxLevel)
        this->_audio->setSFXVolumeLevel(this->_sfxLevel);
}

void Settings::draw()
{
    if (!this->_visible || !this->_settingsContainer)
        return;

    this->_settingsContainer->draw();
}

void Settings::handleResize(int oldWidth, int oldHeight, int newWidth, int newHeight)
{
    if (!this->_settingsContainer)
        return;

    this->_settingsContainer->handleResize(oldWidth, oldHeight, newWidth, newHeight);
}


Settings::Settings(std::shared_ptr<IDisplay> display, std::shared_ptr<IAudio> audio
    ): _display(display), _audio(audio)
{
    if (!_display)
        return;

    this->_settingsContainer = std::make_shared<Containers>(
        this->_display,
        this->_audio,
        15.0f, 0.0f,
        30.0f, 30.0f,
        Color32{50, 50, 50, 240}
    );

    this->_settingsContainer->setRelativePosition(
        15.0f, 0.0f,
        30.0f, 15.0f
    );

    this->_settingsContainer->addTextPercent(
        "settings_title",
        10.0f, 5.0f,
        "Settings",
        12.0f,
        {255, 255, 255, 255}
    );

    this->_settingsContainer->addButtonPercent(
        "settings_close_button",
        87.0f, 3.0f,
        12.0f, 12.0f,
        "X",
        [this]() {
            this->hide();
        },
        {200, 60, 60, 255},
        {255, 100, 100, 255},
        {180, 40, 40, 255},
        {255, 255, 255, 255}
    );

    this->_settingsContainer->addTextPercent(
        "settings_separator",
        0.0f, 20.0f,
        std::string(120, '-'),
        1.0f,
        {150, 150, 150, 200}
    );

    float yPos = 30.0f;

    this->_settingsContainer->addSliderPercent(
        "slider_music",
        5.f, yPos, 90.f, 30.f,
        0.f, 100.f, 50.f, "Volume Music:",
        [this](float nbr) {
            this->_musicLevel = nbr;
        }
    );
    yPos += 30.0f;

    this->_settingsContainer->addSliderPercent(
        "slider_click",
        5.f, yPos, 90.f, 30.f,
        0.f, 100.f, 50.f, "Volume SFX:",
        [this](float nbr) {
            this->_sfxLevel = nbr;
        }
    );

    this->_settingsContainer->addImageButtonPercent(
        "test_music_button",
        62.0f, 65.0f,
        30.0f, 30.0f,
        "gui/assets/sprite/hp_logo.png",
        [this]() {
        }
    );

    this->_visible = false;
}

Settings::~Settings()
{
}
