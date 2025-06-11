/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_end_incantation
*/

#include "zappy.h"
#include "network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Here in this function we send the pie message result is ok or ko*/
int send_end_incantation(zappy_t *zappy, player_t *player, char *result)
{
    int xLength = int_str_len(player->posX) + int_str_len(player->posY)
        + 9;
    char *message = malloc(sizeof(char) * xLength);
    graph_net_t *current = zappy->graph;

    if (message == NULL)
        return -1;
    snprintf(message, xLength, "pie %d %d %s\n",
        player->posX, player->posY, result);
    if (zappy->params->is_debug == true)
        printf("Sending to GUI: %s", message);
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
