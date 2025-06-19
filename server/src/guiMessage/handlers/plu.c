/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** PLU handlers - Player Level Up
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

int plu(zappy_t *zappy, graph_net_t *graphic, char *message)
{
    player_t *player = NULL;
    int player_id = 0;

    if (strlen(message) < 6) {
        error_message("Invalid plu message format.");
        return -1;
    }
    if (sscanf(message, "plu #%d\n", &player_id) != 1) {
        error_message("Invalid plu pattern.");
        return -1;
    }
    player = get_player_by_id(zappy->game, player_id);
    if (!player)
        return -1;
    if (player->level < 8) {
        player->level += 1;
        send_player_level(zappy, player);
    }
    return send_plv_message(graphic, player);
}
