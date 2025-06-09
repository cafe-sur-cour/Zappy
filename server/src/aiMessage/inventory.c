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

int handle_inventory(player_t *player, char *command, zappy_t *zappy)
{
    (void)command;
    if (send_player_inventory(zappy, player) == -1)
        return -1;
    return 0;
}
