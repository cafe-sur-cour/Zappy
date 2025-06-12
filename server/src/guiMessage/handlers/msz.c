/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** MSZ handlers
*/

#include <stdio.h>
#include "../../../include/zappy.h"
#include "../../../include/network.h"
#include "../../../include/my.h"

int msz(zappy_t *zappy, graph_net_t *graphic)
{
    int x = zappy->game->map->width;
    int y = zappy->game->map->height;
    int size = 6 + int_str_len(x) + int_str_len(y);
    char buffer[size];

    snprintf(buffer, size, "msz %d %d\n", x, y);
    if (write_message(graphic->fd, buffer) == -1)
        return -1;
    return 0;
}
