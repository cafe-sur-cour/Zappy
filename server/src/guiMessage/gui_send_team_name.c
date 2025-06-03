/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_team_name
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "zappy.h"


/* Use the length name to create the buffer */
static void create_buffer(const char *team_name, server_t *server)
{
    int xLength = strlen(team_name) + 6;
    char *message = malloc(sizeof(char) * (xLength));

    if (message == NULL) {
        return;
    }
    snprintf(message, xLength, "tna %s\n", team_name);
    write_message(server->graph->fd, message);
    free(message);
}

/* This function serves as the cover to send the TNA */
void send_team_name(server_t *server)
{
    for (int i = 0; i < server->params->nb_team; i++) {
        if (server->params->teams[i] == NULL)
            continue;
        create_buffer(server->params->teams[i], server);
    }
}
