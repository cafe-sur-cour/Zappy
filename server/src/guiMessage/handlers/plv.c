/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** PLV handlers
*/

#include <stdio.h>
#include <string.h>
#include "../../../include/zappy.h"
#include "../../../include/network.h"
#include "../../../include/my.h"

static int send_plv_message(graph_net_t *graphic, player_t *player)
{
    int size = 8 + int_str_len(player->id) + int_str_len(player->level);
    char buffer[size];

    snprintf(buffer, size, "plv #%d %d\n", player->id, player->level);
    return write_message(graphic->fd, buffer);
}

int plv(zappy_t *zappy, graph_net_t *graphic, char *message)
{
    player_t *player = NULL;
    int player_id = 0;

    if (strlen(message) < 7) {
        error_message("Invalid plv message format.");
        return -1;
    }
    if (sscanf(message, "plv #%d\n", &player_id) != 1) {
        error_message("Invalid plv pattern.");
        return -1;
    }
    player = get_player_by_id(zappy->game, player_id);
    return send_plv_message(graphic, player);
}
