/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Audio
*/

#include <iostream>
#include <string>
#include "Audio.hpp"
#include "../RayLib/RayLib.hpp"
#include "../../Utils/Constants.hpp"

Audio::Audio(std::shared_ptr<RayLib> raylib) : _raylib(raylib),
    _isInitialized(false), _mainMusicId("main_theme")
{
    if (!_isInitialized) {
        _raylib->initAudioDevice();
        _isInitialized = true;
    }

    if (!_raylib->loadMusic(_mainMusicId, "gui/assets/sounds/main_theme.wav"))
        std::cout << colors::T_RED << "[ERROR] Failed to load main music."
                  << colors::RESET << std::endl;
    if (!_raylib->loadSound("click", "gui/assets/sounds/click.wav"))
        std::cout << colors::T_RED << "[ERROR] Failed to load click sound."
                  << colors::RESET << std::endl;
    playMainMusic(0.2f);
}

Audio::~Audio()
{
    if (_isInitialized) {
        unloadAllSounds();
        unloadAllMusics();
        _raylib->closeAudioDevice();
        _isInitialized = false;
    }
}

bool Audio::loadSound(const std::string& id, const std::string& filepath)
{
    if (!_isInitialized) {
        std::cerr << "Audio device not initialized" << std::endl;
        return false;
    }

    return _raylib->loadSound(id, filepath);
}

void Audio::playSound(const std::string& id, float volume)
{
    if (!_isInitialized)
        return;

    _raylib->playSound(id, volume);
}

void Audio::stopSound(const std::string& id)
{
    if (!_isInitialized)
        return;

    _raylib->stopSound(id);
}

bool Audio::isSoundPlaying(const std::string& id) const
{
    if (!_isInitialized)
        return false;

    return _raylib->isSoundPlaying(id);
}

void Audio::unloadSound(const std::string& id)
{
    if (!_isInitialized)
        return;

    _raylib->unloadSound(id);
}

void Audio::unloadAllSounds()
{
    if (!_isInitialized)
        return;

    _raylib->unloadAllSounds();
}

bool Audio::loadMusic(const std::string& id, const std::string& filepath)
{
    if (!_isInitialized)
        return false;

    return _raylib->loadMusic(id, filepath);
}

void Audio::playMusic(const std::string& id, float volume)
{
    if (!_isInitialized)
        return;

    _raylib->playMusic(id, volume);
}

void Audio::updateMusicStream(const std::string& id)
{
    if (!_isInitialized)
        return;

    _raylib->updateMusic(id);
}

void Audio::updateAllMusics()
{
    if (!_isInitialized)
        return;

    _raylib->updateAllMusics();
}

void Audio::stopMusic(const std::string& id)
{
    if (!_isInitialized)
        return;

    _raylib->stopMusic(id);
}

void Audio::pauseMusic(const std::string& id)
{
    if (!_isInitialized)
        return;

    _raylib->pauseMusic(id);
}

void Audio::resumeMusic(const std::string& id)
{
    if (!_isInitialized)
        return;

    _raylib->resumeMusic(id);
}

bool Audio::isMusicPlaying(const std::string& id) const
{
    if (!_isInitialized)
        return false;

    return _raylib->isMusicPlaying(id);
}

void Audio::unloadMusic(const std::string& id)
{
    if (!_isInitialized)
        return;
    _raylib->unloadMusic(id);
}

void Audio::unloadAllMusics()
{
    if (!_isInitialized)
        return;

    _raylib->unloadAllMusics();
}

void Audio::playMainMusic(float volume)
{
    if (!_raylib->isMusicPlaying(_mainMusicId))
        _raylib->setMusicLooping(_mainMusicId, true);

    _raylib->playMusic(_mainMusicId, volume);
}

void Audio::stopMainMusic()
{
    stopMusic(_mainMusicId);
}
