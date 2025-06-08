/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** rigth
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>

int right_message(player_t *player)
{
    direction_t direction = player->direction;

    if (direction == NORTH)
        player->direction = WEST;
    if (direction == WEST)
        player->direction = SOUTH;
    if (direction == SOUTH)
        player->direction = EAST;
    if (direction == EAST)
        player->direction = NORTH;
    else
        return -1;
    return 0;
}

int handle_right(player_t *player, char *command, zappy_t *zappy)
{
    (void)command;
    if (right_message(player) == -1)
        return -1;
    if (send_player_pos(zappy, player) == -1)
        return -1;
    return 0;
}
