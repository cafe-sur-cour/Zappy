/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** Server :: Server
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "network.h"
#include "zappy.h"

static graph_net_t *init_graph(void)
{
    graph_net_t *graph = malloc(sizeof(graph_net_t));

    if (!graph) {
        error_message("Failed to allocate memory for graph.");
        exit(84);
    }
    graph->fd = -1;
    return graph;
}

static void fill_elements(zappy_t *zappy)
{
    zappy->network = malloc(sizeof(network_t));
    if (!zappy->network) {
        error_message("Memory allocation failed for network.");
        exit(84);
    }
    zappy->graph = init_graph();
    zappy->game = NULL;
    zappy->params = NULL;
}

static zappy_t *set_zappy_bas(zappy_t *zappy)
{
    if (bind_socket(zappy->network->sockfd, zappy->params->port) == -1)
        return free_zappy(zappy);
    if (listen_socket(zappy->network->sockfd,
        zappy->params->nb_client) == -1) {
        close(zappy->network->sockfd);
        return free_zappy(zappy);
    }
    return zappy;
}

zappy_t *init_server(int argc, char **argv)
{
    zappy_t *zappy = malloc(sizeof(zappy_t));

    if (!zappy) {
        error_message("Memory allocation failed for server.");
        return NULL;
    }
    fill_elements(zappy);
    zappy->params = check_args(argc, argv);
    if (!zappy->params)
        return free_zappy(zappy);
    zappy->network->sockfd = set_socket();
    if (zappy->network->sockfd == -1)
        return free_zappy(zappy);
    zappy = set_zappy_bas(zappy);
    zappy->network->pollserver.fd = zappy->network->sockfd;
    zappy->network->pollserver.events = POLLIN;
    return zappy;
}
