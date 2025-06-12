/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** PPO handlers
*/

#include <stdio.h>
#include <string.h>
#include "../../../include/zappy.h"
#include "../../../include/network.h"
#include "../../../include/my.h"

static int send_ppo_message(graph_net_t *graphic, player_t *player)
{
    int size = 12 + int_str_len(player->id) + int_str_len(player->posX) +
        int_str_len(player->posY);
    char buffer[size];

    snprintf(buffer, size, "ppo #%d %d %d %d\n",
        player->id, player->posX, player->posY, player->direction);
    return write_message(graphic->fd, buffer);
}

int ppo(zappy_t *zappy, graph_net_t *graphic, char *message)
{
    player_t *player = NULL;
    int player_id = 0;

    if (strlen(message) < 7) {
        error_message("Invalid ppo message format.");
        return -1;
    }
    if (sscanf(message, "ppo #%d\n", &player_id) != 1) {
        error_message("Invalid ppo pattern.");
        return -1;
    }
    player = get_player_by_id(zappy->game, player_id);
    return send_ppo_message(graphic, player);
}
