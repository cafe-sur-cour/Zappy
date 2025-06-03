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
    for (int i = 0; i < params->nb_team; i++) {
        if (strcmp(team_name, params->teams[i]) == 0) {
            return true;
        }
    }
    error_message("Invalid team name provided.");
    return false;
}
