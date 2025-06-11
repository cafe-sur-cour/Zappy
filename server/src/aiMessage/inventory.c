/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** inventory
*/


#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>

static void print_inventory_server(player_t *player, int len)
{
    int n = len + int_str_len(player->id);
    char *debug = calloc(n + 91 + 1, sizeof(char));

    if (!debug) {
        error_message("Memory allocation failed for inventory debug print.");
        return;
    }
    snprintf(debug, 90 + n + 1, "Player (%d) inventory: [food %d, linemate %d, "
        "deraumere %d, sibur %d, mendiane %d, phiras %d, thystame %d].",
        player->id, player->inventory->nbFood, player->inventory->nbLinemate,
        player->inventory->nbDeraumere, player->inventory->nbSibur,
        player->inventory->nbMendiane, player->inventory->nbPhiras,
        player->inventory->nbThystame);
    valid_message(debug);
    free(debug);
}

static int inventory_message(player_t *player)
{
    int n = int_str_len(player->inventory->nbFood) + int_str_len(
        player->inventory->nbLinemate) + int_str_len(
        player->inventory->nbDeraumere) + int_str_len(
        player->inventory->nbSibur) + int_str_len(player->inventory->nbMendiane
        ) + int_str_len(player->inventory->nbPhiras) + int_str_len(
        player->inventory->nbThystame);
    char *message = calloc(70 + n + 1, sizeof(char));

    snprintf(message, 70 + n + 1, "[food %d, linemate %d, deraumere %d, "
        "sibur %d, mendiane %d, phiras %d, thystame %d]\n",
        player->inventory->nbFood, player->inventory->nbLinemate,
        player->inventory->nbDeraumere, player->inventory->nbSibur,
        player->inventory->nbMendiane, player->inventory->nbPhiras,
        player->inventory->nbThystame);
    if (write_message(player->network->fd, message) == -1)
        return -1;
    print_inventory_server(player, n);
    free(message);
    return 0;
}

int handle_inventory(player_t *player, char *command, zappy_t *zappy)
{
    (void)command;
    (void)zappy;
    if (inventory_message(player) == -1)
        return -1;
    if (send_player_inventory(zappy, player) == -1)
        return -1;
    return 0;
}
