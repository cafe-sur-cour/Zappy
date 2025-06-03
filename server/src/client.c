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

bool valid_team_name(const char *team_name, server_t *server)
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

bool process_new_client(const char *team_name, int fd, server_t *server)
{
    printf("Processing new client with team name: '%s'\n", team_name);
    if (strcmp(team_name, "GRAPHIC\n") == 0) {
        if (server->graph->fd != -1) {
            error_message("A graphic client is already connected.");
            return false;
        }
        server->graph->fd = fd;
        return true;
    }
    return valid_team_name(team_name, server);
}
