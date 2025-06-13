/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** TNA handlers
*/

#include <stdio.h>
#include <string.h>
#include "../../../include/zappy.h"
#include "../../../include/network.h"
#include "../../../include/my.h"

static int send_tna_message(graph_net_t *graphic, char const *team_name)
{
    char buffer[6 + strlen(team_name)];

    snprintf(buffer, 6 + strlen(team_name), "tna %s\n", team_name);
    if (write_message(graphic->fd, buffer) == -1)
        return -1;
    return 0;
}

int tna(zappy_t *zappy, graph_net_t *graphic, char *message)
{
    if (strcmp(message, "tna\n") != 0) {
        error_message("Invalid mct message format.");
        return -1;
    }
    for (int i = 0; i < zappy->params->nb_team; i++) {
        if (send_tna_message(graphic, zappy->params->teams[i]) == -1)
            return -1;
    }
    return 0;
}
