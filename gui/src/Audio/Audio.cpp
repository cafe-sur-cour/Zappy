/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Audio
*/

#include "Audio.hpp"
#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

Audio::Audio()
{
    this->Audio::loadSound("main_theme", "gui/assets/sounds/main_theme.wav");
    this->Audio::setSoundLooping("main_theme", true);
    this->Audio::playSound("main_theme", this->Audio::getMusicVolumeLevel());
    this->Audio::loadSound("click", "gui/assets/sounds/click.wav");
}

Audio::~Audio()
{
    for (auto& sound : _sounds) {
        if (sound.second) {
            sound.second->stop();
        }
    }
}

float Audio::getSFXVolumeLevel() {
    return this->_levelSFX;
}

float Audio::getMusicVolumeLevel() {
    return this->_levelMusic;
}

void Audio::setSFXVolumeLevel(float level) {
    this->_levelSFX = level;
    for (const auto &s: this->_sfxId)
        this->setSoundVolume(s, this->_levelSFX);
}

void Audio::setMusicVolumeLevel(float level) {
    this->_levelMusic = level;
    for (const auto &s: this->_musicId)
        this->setSoundVolume(s, this->_levelMusic);
}


bool Audio::loadSound(const std::string& id, const std::string& filepath)
{
    if (_sounds.find(id) != _sounds.end()) {
        std::cerr << "Sound with ID '" << id << "' already exists." << std::endl;
        return false;
    }

    auto music = std::make_unique<sf::Music>();
    if (!music->openFromFile(filepath)) {
        std::cerr << "Failed to load sound from '" << filepath << "'" << std::endl;
        return false;
    }

    _sounds[id] = std::move(music);
    return true;
}

void Audio::playSound(const std::string& id, float volume)
{
    auto it = _sounds.find(id);
    if (it == _sounds.end()) {
        std::cerr << "Sound with ID '" << id << "' not found." << std::endl;
        return;
    }
    it->second->stop();
    auto musicIt = std::find(this->_musicId.begin(), this->_musicId.end(), id);
    if (musicIt != this->_musicId.end())
        it->second->setVolume(this->_levelMusic);
    else {
        auto sfxIt = std::find(this->_sfxId.begin(), this->_sfxId.end(), id);
        if (sfxIt != this->_sfxId.end())
            it->second->setVolume(this->_levelSFX);
        else {
            it->second->setVolume(volume);
        }
    }
    it->second->play();
}

void Audio::stopSound(const std::string& id)
{
    auto it = _sounds.find(id);
    if (it == _sounds.end()) {
        std::cerr << "Sound with ID '" << id << "' not found." << std::endl;
        return;
    }

    it->second->stop();
}

bool Audio::isSoundPlaying(const std::string& id) const
{
    auto it = _sounds.find(id);
    if (it == _sounds.end()) {
        std::cerr << "Sound with ID '" << id << "' not found." << std::endl;
        return false;
    }

    return it->second->getStatus() == sf::Music::Playing;
}

void Audio::setSoundLooping(const std::string& id, bool looping)
{
    auto it = _sounds.find(id);
    if (it == _sounds.end()) {
        std::cerr << "Sound with ID '" << id << "' not found." << std::endl;
        return;
    }

    it->second->setLoop(looping);
}

void Audio::setSoundVolume(const std::string& id, float volume)
{
    auto it = _sounds.find(id);
    if (it == _sounds.end()) {
        std::cerr << "Sound with ID '" << id << "' not found." << std::endl;
        return;
    }

    it->second->setVolume(volume);
}
