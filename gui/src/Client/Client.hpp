/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <memory>
#include <filesystem>
#include <string>
#include "../Utils/Constants.hpp"
#include "../Communication/ICommunication.hpp"
#include "../Game/GameInfos.hpp"
#include "../Graphic/GUI.hpp"
#include "MsgHandler.hpp"
#include "../Observer/GuiObserver.hpp"
#include "../Observer/IObserver.hpp"

class Client {
    public:
        Client(int ac, const char *const *av);
        ~Client();

    private:
        void _tryToCreateGuiWithSharedLibInFolder(const std::string &libPath = "./gui/lib/");
        zappy::structs::Config _config;
        void initialize(int ac, const char * const *av);

        std::shared_ptr<ICommunication> _communication;
        std::shared_ptr<GameInfos> _gameInfos;
        std::unique_ptr<MsgHandler> _msgHandler;
        std::shared_ptr<GUI> _gui;
        std::shared_ptr<GuiObserver> _guiObserver;
};

#endif /* !CLIENT_HPP_ */
