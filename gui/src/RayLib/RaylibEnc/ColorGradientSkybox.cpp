/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** ColorGradientSkybox
*/

#include "RayLibEnc.hpp"

void RayLibEnc::drawSimpleSkybox()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    Color topColor = {41, 128, 185, 255};
    Color middleColor = {135, 206, 235, 255};
    Color bottomColor = {240, 248, 255, 255};

    DrawRectangleGradientV(
        0,
        0,
        screenWidth,
        screenHeight/2,
        topColor,
        middleColor);

    DrawRectangleGradientV(
        0,
        screenHeight/2,
        screenWidth,
        screenHeight/2,
        middleColor,
        bottomColor);
}
