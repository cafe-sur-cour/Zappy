/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_egg_death
*/


#include "zappy.h"
#include "network.h"

#include <stdlib.h>
#include <stdio.h>

int send_egg_death(zappy_t *zappy, egg_t *egg)
{
    int xlenght = int_str_len(egg->id) + 7;
    char *message = malloc(sizeof(char) * xlenght);

    if (message == NULL) {
        error_message("Failed to allocate memory for egg death message.");
        return -1;
    }
    snprintf(message, xlenght, "edi #%d\n", egg->id);
    if (zappy->params->is_debug) {
        printf("Sending egg death: %s", message);
    }
    if (write_message(zappy->graph->fd, message) == -1) {
        free(message);
        return -1;
    }
    free(message);
    return 0;
}
