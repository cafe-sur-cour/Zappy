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

static bool check_name(char const *flag, char const * const *value, int nb)
{
    if (!flag || strcmp(flag, "-n") != 0 || !value) {
        error_message("Invalid name flag.");
        return false;
    }
    for (int i = 0; i < nb; i++) {
        if (value[i] == NULL || strlen(value[i]) == 0) {
            error_message("Name cannot be empty.");
            return false;
        }
    }
    return true;
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
    bool (*checker)(const char *, const char *))
{
    int pos = find_flag(argc, argv, flag);

    if (pos == -1 || pos + 1 >= argc) {
        char error_msg[28];
        snprintf(error_msg, sizeof(error_msg),
            "Missing or invalid %s flag.", flag);
        error_message(error_msg);
        return true;
    }
    return checker(argv[pos], argv[pos + 1]) == false;
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
    check_simple_flag(argc, argv, "-p", check_port) ? is_ok = false : 0;
    check_simple_flag(argc, argv, "-x", check_width) ? is_ok = false : 0;
    check_simple_flag(argc, argv, "-y", check_height) ? is_ok = false : 0;
    check_names_flag(argc, argv) ? is_ok = false : 0;
    check_simple_flag(argc, argv, "-c", check_client) ? is_ok = false : 0;
    check_simple_flag(argc, argv, "-f", check_freq) ? is_ok = false : 0;
    return is_ok ? 0 : 84;
}
