/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Exceptions
*/

#ifndef EXCEPTIONS_HPP_
#define EXCEPTIONS_HPP_

#include <exception>
#include <string>

namespace Exceptions {

    class CLIParsingException : public std::exception {
        public:
            explicit CLIParsingException(const std::string &message)
                : _message("CLI Parsing Error: " + message) {}

            virtual const char *what() const noexcept override {
                return _message.c_str();
            }

        private:
            std::string _message;
    };

}

#endif /* !EXCEPTIONS_HPP_ */
