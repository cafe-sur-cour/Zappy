/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** MsgHandler
*/

#ifndef MSGHANDLER_HPP_
#define MSGHANDLER_HPP_

#include "../Game/GameInfos.hpp"
#include "../Communication/Communication.hpp"
#include "../Utils/Constants.hpp"

#include <memory>
#include <map>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <string>

class MsgHandler {
    public:
        MsgHandler(std::shared_ptr<GameInfos> gameInfos, std::shared_ptr<Communication> communication);
        ~MsgHandler();

        void start();
        void stop();

    protected:
        void messageLoop();

        void handleMessage(const std::string& message);
        bool handleWelcomeMessage(const std::string& message);

    private:
        std::thread _thread;
        std::atomic<bool> _running;
        std::mutex _mutex;
        std::condition_variable _condition;

        std::shared_ptr<GameInfos> _gameInfos;
        std::shared_ptr<Communication> _communication;
        std::mutex _gameInfosMutex;

        std::map<std::string, std::function<bool(const std::string&)>> _messageHandlers;
};

#endif /* !MSGHANDLER_HPP_ */
