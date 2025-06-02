/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include "../Utils/Constants.hpp"
#include "../Communication/Communication.hpp"
#include "../Game/GameInfos.hpp"
#include "MsgHandler.hpp"

#include <memory>

class Client {
    public:
        Client(int ac, const char *const *av);
        ~Client();

        void run();

    private:
        zappy::structs::Config _config;
        void initialize(int ac, const char * const *av);

        std::shared_ptr<Communication> _communication;
        std::shared_ptr<GameInfos> _gameInfos;
        std::unique_ptr<MsgHandler> _msgHandler;
};

#endif /* !CLIENT_HPP_ */
