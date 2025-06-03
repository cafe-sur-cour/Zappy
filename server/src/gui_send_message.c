/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_message
*/

#include "zappy.h"

#include <stdlib.h>
#include <stdio.h>
// void send_entrie_map(server_t *server)
// {

// }

// void send_map_tile(server_t *sever)
// {

// }


void send_map_size(server_t *server)
{
    int xLenthth = int_str_len(server->map->width) +
        int_str_len(server->map->heigt) + 7;
    char *message = malloc(sizeof(char) * xLenthth);
    char *x = my_itoa(server->map->width);
    char *y = my_itoa(server->map->heigt);

    snprintf(message, xLenthth, "msz %s %s\n", x, y);
    write_message(server->graph->fd, message);
}
