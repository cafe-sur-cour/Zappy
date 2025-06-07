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

/* THhis function sends the time unit message SGT */
int send_time_message(zappy_t *zappy)
{
    int xLength = int_str_len(zappy->params->freq) + 6;
    char *message = malloc(xLength * sizeof(char));

    if (message == NULL)
        return -1;
    snprintf(message, xLength, "sgt %d\n", zappy->params->freq);
    if (zappy->params->is_debug)
        printf("Sending time message: %s", message);
    if (write_message(zappy->graph->fd, message) == -1) {
        free(message);
        return -1;
    }
    free(message);
    return 0;
}

/* This function send the updated time after the gui upt */
int send_updated_time(zappy_t *zappy, int time)
{
    int xLength = int_str_len(time) + 6;
    char *message = malloc(sizeof(char) * xLength);

    if (message == NULL) {
        error_message("Failed to allocate memory for updated time message.");
        return -1;
    }
    snprintf(message, xLength, "sst %d\n", time);
    if (zappy->params->is_debug)
        printf("Sending updated time: %s", message);
    if (write_message(zappy->graph->fd, message) == -1) {
        free(message);
        return -1;
    }
    free(message);
    return 0;
}
