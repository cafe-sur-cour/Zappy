/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** MSZ handlers
*/

#include <stdio.h>
#include <string.h>
#include "../../../include/zappy.h"
#include "../../../include/network.h"
#include "../../../include/my.h"

static int send_bct_message(graph_net_t *graphic, int x, int y,
    inventory_t *inventory)
{
    int size = 20 + int_str_len(x) + int_str_len(y);
    char buffer[size];

    snprintf(buffer, size, "bct %d %d %d %d %d %d %d %d %d\n",
        x, y, inventory->nbFood, inventory->nbLinemate, inventory->nbDeraumere,
        inventory->nbSibur, inventory->nbMendiane, inventory->nbPhiras,
        inventory->nbThystame);
    return write_message(graphic->fd, buffer);
}

int bct(zappy_t *zappy, graph_net_t *graphic, char *message)
{
    char *pos = NULL;
    int x = 0;
    int y = 0;

    if (strlen(message) < 8) {
        error_message("Invalid bct message format.");
        return -1;
    }
    if (sscanf(pos, "bct %d %d\n", &x, &y) != 2) {
        error_message("Invalid bct pattern.");
        return -1;
    }
    if (x >= zappy->game->map->width || y >= zappy->game->map->height) {
        error_message("Coordinates out of bounds.");
        return -1;
    }
    if (send_bct_message(graphic, x, y, &zappy->game->map->tiles[x][y]) == -1)
        return -1;
    return 0;
}
