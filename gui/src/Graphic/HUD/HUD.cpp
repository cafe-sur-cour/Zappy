/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** HUD
*/

#include <string>
#include <memory>
#include "../../Utils/Constants.hpp"
#include "HUD.hpp"

HUD::HUD(std::shared_ptr<RayLib> raylib)
    : _containers(), _raylib(raylib)
{
}

HUD::~HUD()
{
    _containers.clear();
}

void HUD::draw()
{
    for (auto& pair : _containers) {
        pair.second->draw();
    }
}

void HUD::update()
{
    for (auto& pair : _containers) {
        pair.second->update();
    }
}

std::shared_ptr<Containers> HUD::addContainer(
    const std::string& id,
    float x, float y,
    float width, float height,
    Color backgroundColor
)
{
    if (_containers.find(id) != _containers.end())
        return nullptr;

    auto container = std::make_shared<Containers>(
        _raylib,
        x,
        y,
        width,
        height,
        backgroundColor);

    _containers[id] = container;

    return container;
}

std::shared_ptr<Containers> HUD::getContainer(const std::string& id) const
{
    auto it = _containers.find(id);

    if (it != _containers.end())
        return it->second;

    return nullptr;
}

bool HUD::removeContainer(const std::string& id)
{
    auto it = _containers.find(id);

    if (it != _containers.end()) {
        _containers.erase(it);
        return true;
    }
    return false;
}

void HUD::handleResize(int oldWidth, int oldHeight, int newWidth, int newHeight)
{
    for (auto& pair : _containers) {
        pair.second->handleResize(oldWidth, oldHeight, newWidth, newHeight);
    }
}

void HUD::clearAllContainers()
{
    _containers.clear();
}

void HUD::initDefaultLayout(float sideWidth, float bottomHeight)
{
    int screenHeight = _raylib->getScreenHeight();
    int screenWidth = _raylib->getScreenWidth();

    float squareSize = sideWidth;

    addContainer(
        "square_container",
        0, 0,
        squareSize, squareSize,
        {60, 60, 60, 220}
    );

    addContainer(
        "side_container",
        0, 0,
        sideWidth, screenHeight,
        {40, 40, 40, 200}
    );

    addContainer(
        "bottom_container",
        0, screenHeight - bottomHeight,
        screenWidth, bottomHeight,
        {40, 40, 40, 200}
    );
}

std::shared_ptr<Containers> HUD::getSideContainer() const
{
    return getContainer("side_container");
}

std::shared_ptr<Containers> HUD::getBottomContainer() const
{
    return getContainer("bottom_container");
}

std::shared_ptr<Containers> HUD::getSquareContainer() const
{
    return getContainer("square_container");
}

void HUD::initExitButton()
{
    auto squareContainer = getSquareContainer();
    if (!squareContainer)
        return;

    Rectangle bounds = squareContainer->getBounds();

    float buttonWidth = bounds.width * 0.7f;
    float buttonHeight = bounds.height * 0.15f;

    float buttonX = (bounds.width - buttonWidth) / 2;
    float buttonY = bounds.height * 0.1f;

    squareContainer->addButton(
        "exit_button",
        buttonX, buttonY,
        buttonWidth, buttonHeight,
        "EXIT",
        [this]() {
            _raylib->closeWindow();
        },
        {240, 60, 60, 255},
        {255, 100, 100, 255},
        {200, 40, 40, 255},
        {255, 255, 255, 255}
    );
}
