/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_eggs
*/


#include "network.h"
#include "zappy.h"

#include <stdlib.h>
#include <stdio.h>

/* This send the information of one egg */
void send_egg(zappy_t *zappy, egg_t *egg)
{
    egg_t *current = egg;
    int xLength = int_str_len(current->id) +
        int_str_len(current->posX) + int_str_len(current->posY) +
        int_str_len(current->idLayer) + 20;
    char *message = malloc(sizeof(char) * xLength);

    if (message == NULL)
        return;
    snprintf(message, xLength, "enw #%d #%d %d %d %s\n",
        current->id, current->idLayer, current->posX, current->posY,
        current->teamName);
    if (zappy->params->is_debug == true)
        printf("Sending egg: %s", message);
    write_message(zappy->graph->fd, message);
    free(message);
}

/* This function sends the list of the entire list of the egg of the map */
void send_entire_egg_list(zappy_t *zappy)
{
    egg_t *current = zappy->game->map->currentEggs;

    while (current != NULL) {
        send_egg(zappy, current);
        current = current->next;
    }
}

/* This function sends the message that a playr is laying an egg */
void send_player_laying_egg(zappy_t *zappy, player_t *player)
{
    int xLenght = int_str_len(player->id) + 7;
    char *message = malloc(sizeof(char) * xLenght);

    if (message == NULL) {
        error_message("Failed to allocate memory for player laying egg.");
        return;
    }
    snprintf(message, xLenght, "pfk #%d\n", player->id);
    if (zappy->params->is_debug == true) {
        printf("Sending to GUI: %s", message);
    }
    write_message(zappy->graph->fd, message);
    free(message);
}
