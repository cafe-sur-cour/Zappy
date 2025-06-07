/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_sedn_rest_info_player
*/

#include "zappy.h"
#include "network.h"

#include <stdio.h>
#include <stdlib.h>

static int inventory_message_length(player_t *player)
{
    int len = int_str_len(player->id) +
        int_str_len(player->posX) + int_str_len(player->posY) +
        int_str_len(player->inventory->nbFood) +
        int_str_len(player->inventory->nbLinemate) +
        int_str_len(player->inventory->nbDeraumere) +
        int_str_len(player->inventory->nbSibur) +
        int_str_len(player->inventory->nbMendiane) +
        int_str_len(player->inventory->nbPhiras) +
        int_str_len(player->inventory->nbThystame) + 20;

    return len;
}

/* This send the player's inventory PIN */
int send_player_inventory(zappy_t *zappy, player_t *player)
{
    int xLenght = inventory_message_length(player);
    char *message = malloc(sizeof(char) * xLenght);

    if (message == NULL) {
        error_message("Failed to allocate memory for player inventory.");
        return -1;
    }
    snprintf(message, xLenght, "pin #%d %d %d %d %d %d %d %d %d %d\n",
        player->id, player->posX, player->posY,
        player->inventory->nbFood, player->inventory->nbLinemate,
        player->inventory->nbDeraumere, player->inventory->nbSibur,
        player->inventory->nbMendiane, player->inventory->nbPhiras,
        player->inventory->nbThystame);
    if (zappy->params->is_debug == true)
        printf("Sending to GUI: %s", message);
    if (write_message(zappy->graph->fd, message) == -1) {
        free(message);
        return -1;
    }
    free(message);
    return 0;
}

/* This function sends the the expulsion message of a player to the gui PEX */
int send_player_expelled(zappy_t *zappy, player_t *player)
{
    int xLenght = int_str_len(player->id) + 7;
    char *message = malloc(sizeof(char) * xLenght);

    if (message == NULL) {
        error_message("Failed to allocate memory for player expulsion.");
        return -1;
    }
    snprintf(message, xLenght, "pex #%d\n", player->id);
    if (zappy->params->is_debug == true) {
        printf("Sending to GUI: %s", message);
    }
    if (write_message(zappy->graph->fd, message) == -1) {
        free(message);
        return -1;
    }
    free(message);
    return 0;
}
