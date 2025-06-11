/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Map
*/

#ifndef MAP_HPP_
#define MAP_HPP_

#include <memory>
#include <unordered_map>
#include <string>
#include "../Game/GameInfos.hpp"
#include "../IDisplay.hpp"

enum class DisplayPriority {
    TILE = 0,
    EGG = 1,
    PLAYER = 2,
    FOOD = 3,
    ROCK = 4,
};

class Map {
    public:
        Map(std::shared_ptr<GameInfos> gameInfos, std::shared_ptr<IDisplay> display);
        ~Map();

        void draw();
        void drawTile(int x, int y, const zappy::structs::Tile &tile);
        void drawRock(int x, int y, const zappy::structs::Tile &tile);
        void drawFood(int x, int y, const zappy::structs::Tile &tile);
        void drawPlayers(int x, int y);
        void drawEggs(int x, int y);
        Color32 getTeamColor(const std::string &teamName);

        float getOffset(DisplayPriority priority, int x, int y, size_t stackIndex = 0);

    private:
        std::shared_ptr<GameInfos> _gameInfos;
        std::shared_ptr<IDisplay> _display;
        std::unordered_map<std::string, Color32> _teamColors;

        static constexpr float BASE_HEIGHT_TILE = 0.0f;
        static constexpr float BASE_HEIGHT_FOOD = 0.2f;
        static constexpr float BASE_HEIGHT_ROCK = 0.2f;
        static constexpr float BASE_HEIGHT_EGG = 0.2f;
        static constexpr float BASE_HEIGHT_PLAYER = 0.2f;
        static constexpr float FOOD_HEIGHT = 0.3f;
        static constexpr float ROCK_HEIGHT = 0.3f;
        static constexpr float EGG_HEIGHT = 0.3f;
        static constexpr float PLAYER_HEIGHT = 1.1f;

        void drawOrientationArrow(const Vector3f &position, int orientation,
            float playerHeight);
};

#endif /* !MAP_HPP_ */
