/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** Server :: Server
*/

#include <stdlib.h>

#include "zappy.h"

void *free_server(server_t *server)
{
    if (!server)
        return NULL;
    if (server->params)
        free_params(server->params);
    free(server);
    return NULL;
}

server_t *init_server(int argc, char **argv)
{
    server_t *server = malloc(sizeof(server_t));

    if (!server) {
        error_message("Memory allocation failed for server.");
        return NULL;
    }
    server->params = check_args(argc, argv);
    if (!server->params)
        return free_server(server);
    return server;
}
