/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_death_player
*/

#include "zappy.h"
#include "network.h"

#include <stdlib.h>
#include <stdio.h>

/* This function sends the gui a player as died */
void send_player_death(zappy_t *zappy, player_t *player)
{
    int xLength = int_str_len(player->id) + 7;
    char *message = malloc(sizeof(char) * xLength);

    if (message == NULL) {
        error_message("Failed to allocate memory for player death message.");
        return;
    }
    snprintf(message, xLength, "pdi #%d\n", player->id);
    if (zappy->params->is_debug == true) {
        printf("Sending to GUI: %s", message);
    }
    write_message(zappy->graph->fd, message);
    free(message);
}
