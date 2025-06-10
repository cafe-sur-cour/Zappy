/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** left
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>

static void print_left_server(player_t *player)
{
    int len = int_str_len(player->id) + 22;
    char *debug = calloc(len + 1, sizeof(char));

    if (!debug) {
        error_message("Memory allocation failed for left debug print.");
        return;
    }
    snprintf(debug, len + 1, "Player (%d) turned left.",
        player->id);
    valid_message(debug);
    free(debug);
}

static void left_message(player_t *player)
{
    direction_t direction = player->direction;

    if (direction == NORTH) {
        player->direction = EAST;
        return;
    }
    if (direction == EAST) {
        player->direction = SOUTH;
        return;
    }
    if (direction == SOUTH) {
        player->direction = WEST;
        return;
    }
    if (direction == WEST) {
        player->direction = NORTH;
        return;
    }
}

/* This function is defined in the array of funcion pointers */
int handle_left(player_t *player, char *command, zappy_t *zappy)
{
    (void)command;
    left_message(player);
    print_left_server(player);
    if (send_player_pos(zappy, player) == -1)
        return -1;
    return 0;
}
