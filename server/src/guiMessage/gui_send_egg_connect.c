/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_egg_connect
*/

#include "network.h"
#include "zappy.h"

#include <stdlib.h>
#include <stdio.h>


/* This function notify of an player connectiong to an egg EBO */
int send_egg_connect(zappy_t *zappy, egg_t *currentEgg)
{
    int xLength = int_str_len(currentEgg->id) + 7;
    char *message = malloc(sizeof(char) * xLength);
    graph_net_t *current = zappy->graph;

    if (message == NULL) {
        error_message("Memory allocation failed in send_egg_connect");
        return -1;
    }
    snprintf(message, xLength, "ebo #%d\n", currentEgg->id);
    if (zappy->params->is_debug == true)
        printf("DEBUG: Sending egg connection message: %s", message);
    while (current != NULL) {
        if (write_message(current->fd, message) == -1) {
            free(message);
            return -1;
        }
        current = current->next;
    }
    free(message);
    return 0;
}
