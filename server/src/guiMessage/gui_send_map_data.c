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


/* Send the bct message to the gui */
void send_map_tile(inventory_t **tiles, zappy_t *server,
    int posX, int posY)
{
    int xLength = 0;
    char *message = NULL;

    xLength = int_str_len(posX) +
        int_str_len(posY) +
        int_str_len(tiles[posX][posY].nbFood) + int_str_len(tiles[posX][posY]
        .nbLinemate) + int_str_len(tiles[posX][posY].nbDeraumere) +
        int_str_len(tiles[posX][posY].nbSibur) + int_str_len(tiles[posX][posY].
        nbMendiane) + int_str_len(tiles[posX][posY].nbPhiras) +
        int_str_len(tiles[posX][posY].nbThystame) + 14;
    message = malloc(sizeof(char) * xLength);
    snprintf(message, xLength, "bct %d %d %d %d %d %d %d %d %d\n",
        posX, posY,
        tiles[posX][posY].nbFood, tiles[posX][posY].nbLinemate,
        tiles[posX][posY].nbDeraumere, tiles[posX][posY].nbSibur,
        tiles[posX][posY].nbMendiane, tiles[posX][posY].nbPhiras,
        tiles[posX][posY].nbThystame);
    write_message(server->graph->fd, message);
    free(message);
}

void send_entrie_map(zappy_t *zappy)
{
    for (int x = 0; x < zappy->game->map->width; x++) {
        for (int y = 0; y < zappy->game->map->height; y++) {
            send_map_tile(zappy->game->map->tiles, zappy, x, y);
        }
    }
}

/* Send  the msz message to the gui */
void send_map_size(zappy_t *server)
{
    int xLenthth = int_str_len(server->game->map->width) +
        int_str_len(server->game->map->height) + 7;
    char *message = malloc(sizeof(char) * xLenthth);
    char *x = my_itoa(server->game->map->width);
    char *y = my_itoa(server->game->map->height);

    snprintf(message, xLenthth, "msz %s %s\n", x, y);
    write_message(server->graph->fd, message);
    free(message);
    free(x);
    free(y);
}
