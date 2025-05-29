/*
** EPITECH PROJECT, 2025
** Server
** File description:
** Server :: Parsing
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "zappy.h"

static int check_port(char const *flag, char const *value)
{
    int port = 0;

    if (!flag || strcmp(flag, "-p") != 0 || !value) {
        error_message("Invalid port flag.");
        return -1;
    }
    port = atoi(value);
    if (port < 1024 || port > 65535) {
        error_message("Port must be between 1024 and 65535 (included).");
        return -1;
    }
    return 0;
}

static int check_width(char const *flag, char const *value)
{
    int width = 0;

    if (!flag || strcmp(flag, "-x") != 0 || !value) {
        error_message("Invalid width flag.");
        return -1;
    }
    width = atoi(value);
    if (width <= 0) {
        error_message("Width must be a positive integer.");
        return -1;
    }
    return 0;
}

static int check_height(char const *flag, char const *value)
{
    int height = 0;

    if (!flag || strcmp(flag, "-y") != 0 || !value) {
        error_message("Invalid height flag.");
        return -1;
    }
    height = atoi(value);
    if (height <= 0) {
        error_message("Height must be a positive integer.");
        return -1;
    }
    return 0;
}

static int check_name(char const *flag, char const * const *value, int nb)
{
    if (!flag || strcmp(flag, "-n") != 0 || !value) {
        error_message("Invalid name flag.");
        return -1;
    }
    for (int i = 0; i < nb; i++) {
        if (value[i] == NULL || strlen(value[i]) == 0) {
            error_message("Name cannot be empty.");
            return -1;
        }
    }
    return 0;
}

int check_args(int argc, char **argv)
{
    bool is_ok = true;

    if (argc < 7)
        return helper();
    check_port(argv[1], argv[2]) == -1 ? is_ok = false : 0;
    check_width(argv[3], argv[4]) == -1 ? is_ok = false : 0;
    check_height(argv[5], argv[6]) == -1 ? is_ok = false : 0;
    check_name(argv[7],
        (const char **) &argv[8], argc - 12) == -1 ? is_ok = false : 0;
    return is_ok ? 0 : 84;
}
