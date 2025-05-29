/*
** EPITECH PROJECT, 2025
** Server
** File description:
** Server :: Main
*/

#include <string.h>

#include "zappy.h"

int main(int argc, char **argv)
{
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "-help") == 0)) {
        return helper();
    }
    if (check_args(argc, argv) == 84)
        return 84;
    return 0;
}
