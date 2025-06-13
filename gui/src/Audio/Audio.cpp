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
    loadSound("main_theme", "gui/assets/sounds/main_theme.wav");
    setSoundLooping("main_theme", true);
    playSound("main_theme", 50.0f);

    loadSound("click", "gui/assets/sounds/click.wav");
    loadSound("clickPlayer", "gui/assets/sounds/playerClick.wav");
}

Audio::~Audio()
{
    for (auto& sound : _sounds) {
        if (sound.second) {
            sound.second->stop();
        }
    }
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
    it->second->setVolume(volume);
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
