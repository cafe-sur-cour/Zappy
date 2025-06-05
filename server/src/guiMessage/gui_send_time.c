/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_time
*/

#include "network.h"
#include "zappy.h"

#include <stdio.h>
#include <stdlib.h>

void send_time_message(zappy_t *zappy)
{
    int xLength = int_str_len(zappy->params->freq) + 6;
    char *message = malloc(xLength * sizeof(char));

    if (message == NULL)
        return;
    snprintf(message, xLength, "sgt %d\n", zappy->params->freq);
    if (zappy->params->is_debug)
        printf("Sending time message: %s", message);
    write_message(zappy->graph->fd, message);
    free(message);
}
