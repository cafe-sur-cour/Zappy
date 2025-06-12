/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** SST handlers
*/

#include <stdio.h>
#include <string.h>
#include "../../../include/zappy.h"
#include "../../../include/network.h"
#include "../../../include/my.h"

static int send_sst_message(graph_net_t *graphic, int time)
{
    int size = 5 + int_str_len(time);
    char buffer[size];

    snprintf(buffer, size, "sst %d\n", time);
    return write_message(graphic->fd, buffer);
}

int sst(zappy_t *zappy, graph_net_t *graphic, char *message)
{
    int new_freq = 0;

    if (strlen(message) < 6) {
        error_message("Invalid plv message format.");
        return -1;
    }
    if (sscanf(message, "plv %d\n", &new_freq) != 1) {
        error_message("Invalid plv pattern.");
        return -1;
    }
    if (send_sst_message(graphic, new_freq) == -1)
        return -1;
    zappy->params->freq = new_freq;
    return 0;
}
