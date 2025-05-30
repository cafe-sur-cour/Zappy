/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** GameInfos
*/

#include "GameInfos.hpp"

GameInfos::GameInfos() :
    _mapWidth(0),
    _mapHeight(0)
{
}

GameInfos::~GameInfos()
{
}

void GameInfos::setMapSize(int width, int height)
{
    _mapWidth = width;
    _mapHeight = height;
}

std::pair<int, int> GameInfos::getMapSize() const
{
    return std::make_pair(_mapWidth, _mapHeight);
}

void GameInfos::setTimeUnit(int timeUnit)
{
    _timeUnit = timeUnit;
}

int GameInfos::getTimeUnit() const
{
    return _timeUnit;
}
