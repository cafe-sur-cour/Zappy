/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server :: Zappy header
*/

#include <stdbool.h>

#ifndef ZAPPY_H_
    #define ZAPPY_H_

int helper(void);
void error_message(const char *message);
int check_args(int argc, char **argv);
bool check_port(char const *flag, char const *value);
bool check_width(char const *flag, char const *value);
bool check_height(char const *flag, char const *value);
bool check_client(char const *flag, char const *value);
bool check_freq(char const *flag, char const *value);

#endif /* !ZAPPY_H_ */
