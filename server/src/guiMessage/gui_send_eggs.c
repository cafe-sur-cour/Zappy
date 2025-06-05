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
    printf("Sending egg: %s", message);
    write_message(zappy->graph->fd, message);
    free(message);
}

void send_entire_egg_list(zappy_t *zappy)
{
    egg_t *current = zappy->game->map->currentEggs;

    while (current != NULL) {
        send_egg(zappy, current);
        current = current->next;
    }
}
