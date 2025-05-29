/*
** EPITECH PROJECT, 2025
** Server
** File description:
** Server :: Parsing
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "zappy.h"

static int check_port(char const *flag, char const *value)
{
    int port = 0;

    if (!flag || strcmp(flag, "-p") != 0 || !value) {
        error_message("Invalid port flag.");
        return 84;
    }
    port = atoi(value);
    if (port < 1024 || port > 65535) {
        error_message("Port must be between 1024 and 65535 (included).");
        return 84;
    }
    return 0;
}

int check_args(int argc, char **argv)
{
    if (argc < 3) {
        return helper();
    }
    if (check_port(argv[1], argv[2]) != 0) {
        return 84;
    }
    return 0;
}