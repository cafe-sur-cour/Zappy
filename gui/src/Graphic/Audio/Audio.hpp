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
#include "../RayLib/RayLib.hpp"

class Audio {
    public:
        Audio(std::shared_ptr<RayLib> raylib);
        ~Audio();

        bool loadSound(const std::string& id, const std::string& filepath);
        void playSound(const std::string& id, float volume = 1.0f);
        void stopSound(const std::string& id);
        bool isSoundPlaying(const std::string& id) const;
        void unloadSound(const std::string& id);
        void unloadAllSounds();

        bool loadMusic(const std::string& id, const std::string& filepath);
        void playMusic(const std::string& id, float volume = 1.0f);
        void updateMusicStream(const std::string& id);
        void updateAllMusics();
        void stopMusic(const std::string& id);
        void pauseMusic(const std::string& id);
        void resumeMusic(const std::string& id);
        bool isMusicPlaying(const std::string& id) const;
        void unloadMusic(const std::string& id);
        void unloadAllMusics();

        void playMainMusic(float volume = 1.0f);
        void stopMainMusic();

    private:
        std::shared_ptr<RayLib> _raylib;
        bool _isInitialized;
        std::string _mainMusicId;
};

#endif /* !AUDIO_HPP_ */
