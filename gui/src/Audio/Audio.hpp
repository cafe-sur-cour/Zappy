/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Audio
*/

#ifndef AUDIO_HPP_
#define AUDIO_HPP_

#include <string>
#include <map>
#include <memory>
#include <SFML/Audio.hpp>
#include "IAudio.hpp"

class Audio : public IAudio {
    public:
        Audio();
        ~Audio();

        bool loadSound(const std::string& id, const std::string& filepath);

        void playSound(const std::string& id, float volume = 1.0f);
        void stopSound(const std::string& id);
        bool isSoundPlaying(const std::string& id) const;

        void setSoundLooping(const std::string& id, bool looping);
        void setSoundVolume(const std::string& id, float volume);

    private:
        std::map<std::string, std::unique_ptr<sf::Music>> _sounds;
};

#endif /* !AUDIO_HPP_ */
