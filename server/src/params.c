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

static int check_client(char const *flag, char const *value)
{
    int clients_nb = 0;

    if (!flag || strcmp(flag, "-c") != 0 || !value) {
        error_message("Invalid client flag.");
        return -1;
    }
    clients_nb = atoi(value);
    if (clients_nb <= 0) {
        error_message("Number of clients must be a positive integer.");
        return -1;
    }
    return 0;
}

static int check_freq(char const *flag, char const *value)
{
    int freq = 0;

    if (!flag || strcmp(flag, "-f") != 0 || !value) {
        error_message("Invalid frequency flag.");
        return -1;
    }
    freq = atoi(value);
    if (freq <= 0  || freq > 200) {
        error_message("Frequency must be 1 and 200 (included).");
        return -1;
    }
    return 0;
}

static int find_flag(int argc, char **argv, const char *flag)
{
    for (int i = 1; i < argc; i++) {
        if (argv[i] && strcmp(argv[i], flag) == 0)
            return i;
    }
    return -1;
}

static int count_names(int argc, char **argv, int start_pos)
{
    int count = 0;

    for (int i = start_pos; i < argc; i++) {
        if (argv[i][0] == '-')
            break;
        count++;
    }
    return count;
}

static bool check_simple_flag(int argc, char **argv, const char *flag,
    int (*checker)(const char *, const char *))
{
    int pos = find_flag(argc, argv, flag);

    if (pos == -1 || pos + 1 >= argc) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg),
            "Missing or invalid %s flag.", flag);
        error_message(error_msg);
        return false;
    }
    return checker(argv[pos], argv[pos + 1]) == 0;
}

static bool check_names_flag(int argc, char **argv)
{
    int pos = find_flag(argc, argv, "-n");
    int names_count;

    if (pos == -1 || pos + 1 >= argc) {
        error_message("Missing or invalid name flag.");
        return false;
    }
    names_count = count_names(argc, argv, pos + 1);
    if (names_count == 0) {
        error_message("No team names provided after -n flag.");
        return false;
    }
    return check_name(argv[pos],
        (const char **) &argv[pos + 1], names_count) == 0;
}

int check_args(int argc, char **argv)
{
    bool is_ok = true;

    if (argc < 14)
        return helper();
    check_simple_flag(argc, argv, "-p", check_port) ? 0 : (is_ok = false);
    check_simple_flag(argc, argv, "-x", check_width) ? 0 : (is_ok = false);
    check_simple_flag(argc, argv, "-y", check_height) ? 0 : (is_ok = false);
    check_names_flag(argc, argv) ? 0 : (is_ok = false);
    check_simple_flag(argc, argv, "-c", check_client) ? 0 : (is_ok = false);
    check_simple_flag(argc, argv, "-f", check_freq) ? 0 : (is_ok = false);
    return is_ok ? 0 : 84;
}
