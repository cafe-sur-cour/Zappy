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
    zappy_t *zappy = NULL;

    if (argc == 2 && (strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "-help") == 0)) {
        return helper();
    }
    zappy = init_server(argc, argv);
    if (zappy == NULL)
        return 84;
    init_game(zappy);
    start_protocol(zappy);
    free_zappy(zappy);
    return 0;
}
