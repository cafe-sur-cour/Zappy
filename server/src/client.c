/*
** EPITECH PROJECT, 2025
** Zapppy
** File description:
** Server :: Client
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "zappy.h"

bool valid_team_name(const char *team_name, params_t *params)
{
    printf("Validating team name: '%s'\n", team_name);
    if (team_name == NULL) {
        error_message("Team name cannot be NULL.");
        return false;
    }
    if (strcmp(team_name, "") == 0) {
        error_message("Team name cannot be an empty string.");
        return false;
    }
    for (int i = 0; i < server->params->nb_team; i++) {
        if (strcmp(team_name, server->params->teams[i]) == 0) {
            return true;
        }
    }
    error_message("Invalid team name provided.");
    return false;
}

static graph_t *init_graph(void)
{
    graph_t *graph = malloc(sizeof(graph_t));

    if (!graph) {
        error_message("Failed to allocate memory for graph.");
        exit(84);
    }
    graph->fd = -1;
    graph->pollfd = NULL;
    return graph;
}

bool graphic(const char *team_name, int fd, server_t *server)
{
    if (strcmp(team_name, "GRAPHIC") == 0) {
        server->graph = init_graph();
        if (server->graph->fd != -1) {
            error_message("A graphic client is already connected.");
            return false;
        }
        server->graph->fd = fd;
        server->graph->pollfd = realloc(server->graph->pollfd,
            sizeof(struct pollfd) * (server->params->nb_client *
            server->params->nb_team + 1));
        if (!server->graph->pollfd) {
            error_message("Failed to allocate memory for graph poll.");
            return false;
        }
        server->graph->pollfd[0].fd = fd;
        server->graph->pollfd[0].events = POLLIN;
        printfd("GRAPHIC client connected.\n", fd);
        return true;
    }
    return valid_team_name(team_name, server);
}
