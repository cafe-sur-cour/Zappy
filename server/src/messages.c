/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server :: Errors
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

#include "zappy.h"
#include "buffer.h"

int helper(void)
{
    printf("USAGE: ./zappy_server -p port -x width -y height ");
    printf("-n name1 name2 ... -c clientsNb -f freq\n");
    return 84;
}

void error_message(char const *message)
{
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    char time_str[26];
    strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("\033[0;31m%s\033[0m\t\033[1;31m%s\033[0m\n", time_str, message);
}
