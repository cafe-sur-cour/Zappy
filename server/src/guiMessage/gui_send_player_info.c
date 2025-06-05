/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_player_connect
*/

#include "zappy.h"
#include "network.h"

#include <stdio.h>
#include <stdlib.h>

/* This function sends the Connection player PNW */
void send_player_connect(zappy_t *zappy, player_t *player)
{
    int xLenght = int_str_len(player->id) + int_str_len(player->posX) +
        int_str_len(player->posY) + int_str_len(player->level) +
        int_str_len(player->direction) + strlen(player->team) + 12;
    char *message = malloc(sizeof(char) * xLenght);

    if (!message) {
        error_message("Failed to allocate memory for player connect message.");
        return;
    }
    snprintf(message, xLenght, "pnw #%d %d %d %d %d %s\n",
        player->id, player->posX, player->posY, player->level,
        player->direction, player->team);
    if (zappy->params->is_debug == true) {
        printf("Sending to GUI: %s", message);
    }
    write_message(zappy->graph->fd, message);
    free(message);
}


/* This functio send the position of a player */
void send_player_pos(zappy_t *zappy, player_t *player)
{
    int xLength = int_str_len(player->id) + int_str_len(player->posX) +
        int_str_len(player->posY) + int_str_len(player->direction) + 10;
    char *message = malloc(sizeof(char) * xLength);

    if (message == NULL) {
        error_message("Failed to allocate memory for player position message.");
        return;
    }
    snprintf(message, xLength,"ppo #%d %d %d %d\n",
        player->id, player->posX, player->posY, player->direction);
    if (zappy->params->is_debug == true) {
        printf("Sending to GUI: %s", message);
    }
    write_message(zappy->graph->fd, message);
    free(message);
}
