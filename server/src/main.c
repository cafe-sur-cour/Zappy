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
    server_t *server = NULL;

    if (argc == 2 && (strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "-help") == 0)) {
        return helper();
    }
    server = init_server(argc, argv);
    if (server == NULL)
        return 84;
    init_game(server);
    start_protocol(server);
    free_server(server);
    return 0;
}
