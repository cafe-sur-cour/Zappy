/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include "../Utils/Constants.hpp"

class Client {
    public:
        Client(int ac, const char *const *av);
        ~Client();

    private:
        zappy::structs::Config _config;
        void initialize(int ac, const char * const *av);
};

#endif /* !CLIENT_HPP_ */
