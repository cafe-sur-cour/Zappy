/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_boradcast
*/

#include "zappy.h"
#include "network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This function sends the broadcast to one player PBC */
void send_broadcast_to_player(zappy_t *zappy, player_t *player,
    const char *message)
{
    int xLength = int_str_len(player->id) + strlen(message) + 8;
    char *formatted_message = malloc(sizeof(char) * xLength);

    if (formatted_message == NULL) {
        error_message("Failed to allocate memory for broadcast message.");
        return;
    }
    snprintf(formatted_message, xLength, "pbc #%d %s\n", player->id, message);
    if (zappy->params->is_debug == true) {
        printf("Sending to GUI: %s", formatted_message);
    }
    write_message(zappy->graph->fd, formatted_message);
    free(formatted_message);
}

/* This loop send the message to all players */
void send_broadcast_to_all(zappy_t *zappy, const char *message)
{
    team_t *currentTeam = NULL;
    player_t *currentPlayer = NULL;

    for (int i = 0; i < zappy->params->nb_team; i++) {
        currentTeam = zappy->game->teams;
        currentPlayer = currentTeam->players;
        while (currentPlayer != NULL) {
            send_broadcast_to_player(zappy, currentPlayer, message);
            currentPlayer = currentPlayer->next;
        }
        currentTeam = currentTeam->next;
    }
}
