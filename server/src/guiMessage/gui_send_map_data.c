/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_message
*/

#include "network.h"
#include "zappy.h"

#include <stdlib.h>
#include <stdio.h>

int write_map_message(int xLength, zappy_t *zappy,
    inventory_t **tiles, int *pos)
{
    char *message = malloc(sizeof(char) * xLength);
    graph_net_t *current = zappy->graph;

    if (message == NULL)
        return -1;
    snprintf(message, xLength, "bct %d %d %d %d %d %d %d %d %d\n",
        pos[0], pos[1],
        tiles[pos[0]][pos[1]].nbFood, tiles[pos[0]][pos[1]].nbLinemate,
        tiles[pos[0]][pos[1]].nbDeraumere, tiles[pos[0]][pos[1]].nbSibur,
        tiles[pos[0]][pos[1]].nbMendiane, tiles[pos[0]][pos[1]].nbPhiras,
        tiles[pos[0]][pos[1]].nbThystame);
    while (current != NULL) {
        if (write_message(current->fd, message) == -1) {
            free(message);
            return -1;
        }
        current = current->next;
    }
    free(message);
    return 0;
}

/* Send the bct message to the gui */
int send_map_tile(inventory_t **tiles, zappy_t *server,
    int posX, int posY)
{
    int xLength = 0;
    int *pos = malloc(sizeof(int) * 2);

    if (pos == NULL)
        return -1;
    xLength = int_str_len(posX) +
        int_str_len(posY) +
        int_str_len(tiles[posX][posY].nbFood) + int_str_len(tiles[posX][posY]
        .nbLinemate) + int_str_len(tiles[posX][posY].nbDeraumere) +
        int_str_len(tiles[posX][posY].nbSibur) + int_str_len(tiles[posX][posY].
        nbMendiane) + int_str_len(tiles[posX][posY].nbPhiras) +
        int_str_len(tiles[posX][posY].nbThystame) + 14;
    pos[0] = posX;
    pos[1] = posY;
    if (write_map_message(xLength, server, tiles, pos) == -1)
        return -1;
    return 0;
}

static int send_current_tiles(zappy_t *zappy, int x)
{
    for (int y = 0; y < zappy->game->map->height; y++) {
        if (send_map_tile(zappy->game->map->tiles, zappy, x, y) == -1) {
            error_message("Failed to send map tile.");
            return -1;
        }
    }
    return 0;
}

int send_entrie_map(zappy_t *zappy)
{
    for (int x = 0; x < zappy->game->map->width; x++) {
        if (send_current_tiles(zappy, x) == -1) {
            error_message("Failed to send current tiles.");
            return -1;
        }
    }
    return 0;
}

static void free_elems(char *message, char *x, char *y)
{
    if (message)
        free(message);
    if (x)
        free(x);
    if (y)
        free(y);
}

/* Send  the msz message to the gui */
int send_map_size(zappy_t *server)
{
    int xLenthth = int_str_len(server->game->map->width) +
        int_str_len(server->game->map->height) + 7;
    char *message = malloc(sizeof(char) * xLenthth);
    char *x = my_itoa(server->game->map->width);
    char *y = my_itoa(server->game->map->height);
    graph_net_t *current = server->graph;

    snprintf(message, xLenthth, "msz %s %s\n", x, y);
    while (current != NULL) {
        if (write_message(current->fd, message) == -1) {
            free_elems(message, x, y);
            return -1;
        }
        current = current->next;
    }
    free_elems(message, x, y);
    return 0;
}
