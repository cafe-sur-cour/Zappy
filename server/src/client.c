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
    for (int i = 0; i < server->params->nb_team; i++) {
        if (strcmp(team_name, server->params->teams[i]) == 0) {
            printf(" New client connected with team name: %s\n", team_name);
            return true;
        }
    }
    error_message("Invalid team name provided.");
    return false;
}
