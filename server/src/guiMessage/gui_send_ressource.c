/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_ressource
*/

#include "zappy.h"
#include "network.h"

#include <stdio.h>
#include <stdlib.h>

/* This function sends the ressoruces dropping */
void send_ressource_droped(zappy_t *zappy, player_t *player,
    int ressourceType)
{
    int xLength = int_str_len(player->id) + 9;
    char *message = malloc(sizeof(char) * xLength);

    if (message == NULL) {
        error_message("Failed to allocate memory for resource drop message.");
        return;
    }
    snprintf(message, xLength, "pdr #%d %d\n", player->id, ressourceType);
    if (zappy->params->is_debug == true) {
        printf("Sending to GUI: %s", message);
    }
    write_message(zappy->graph->fd, message);
    free(message);
}

/* This function notify gui that the player collected a ressource  */
void send_ressource_collected(zappy_t *zappy, player_t *player,
    int ressourceType)
{
    int xLength = int_str_len(player->id) + 9;
    char *message = malloc(sizeof(char) * xLength);

    if (message == NULL) {
        error_message("Failed to allocate memory for resource collected.");
        return;
    }
    snprintf(message, xLength, "pgt #%d %d\n", player->id, ressourceType);
    if (zappy->params->is_debug == true) {
        printf("Sending to GUI: %s", message);
    }
    write_message(zappy->graph->fd, message);
    free(message);
}
