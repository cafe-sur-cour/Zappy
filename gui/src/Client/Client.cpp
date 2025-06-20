/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Client
*/

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <string>

#include "Client.hpp"
#include "../Communication/Communication.hpp"
#include "../CLI/CLI.hpp"

Client::Client(int ac, const char *const *av)
{
    initialize(ac, av);

    std::cout << colors::T_GREEN << "Client initialized with configuration: "
              << "Port: " << _config.port
              << ", Hostname: " << _config.hostname
              << colors::RESET << std::endl;

    _communication = std::make_shared<Communication>(_config);
    _gameInfos = std::make_shared<GameInfos>(_communication);
    _msgHandler = std::make_unique<MsgHandler>(_gameInfos, _communication);

    if (!_communication->isConnected())
        return;
    this->tryToCreateGuiWithSharedLibInFolder("./gui/lib/");
    if (!this->_gui) {
        std::cerr << "No gui lib found" << std::endl;
        exit(84);
    }
    _guiObserver = std::make_shared<GuiObserver>(_gui);
    _gameInfos->addObserver(_guiObserver);
    _gui->run();
}

Client::~Client()
{
}

void Client::tryToCreateGuiWithSharedLibInFolder(const std::string &libPath)
{
    try {
        for (const auto &entry : std::filesystem::directory_iterator(libPath)) {
            if (entry.path().extension() == ".so") {
                try {
                    this->_gui = std::make_unique<GUI>(this->_gameInfos,
                        entry.path().string());
                    break;
                } catch (Exceptions::ModuleError &e) {
                    this->_gui = nullptr;
                    std::cerr << e.what() << std::endl;
                    continue;
                }
            }
        }
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }
    return;
}

void Client::initialize(int ac, const char *const *av)
{
    CLI cli(ac, av);

    _config = cli.parseArguments(ac, av);
}
