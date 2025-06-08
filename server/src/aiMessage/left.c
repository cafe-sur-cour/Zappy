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


int left_message(player_t *player)
{
    direction_t direction = player->direction;

    if (direction == NORTH)
        player->direction = EAST;
    if (direction == EAST)
        player->direction = SOUTH;
    if (direction == SOUTH)
        player->direction = WEST;
    if (direction == WEST)
        player->direction = NORTH;
    else
        return -1;
    return 0;
}

/* This function is defined in the array of funcion pointers */
int handle_left(player_t *player, char *command, zappy_t *zappy)
{
    (void)command;
    if (left_message(player) == -1)
        return -1;
    if (send_player_pos(zappy, player) == -1)
        return -1;
    return 0;
}
