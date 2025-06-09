/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** broadcast
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>

int handle_broadcast(player_t *player, char *command, zappy_t *zappy)
{
    (void)player;
    if (send_broadcast_to_all(zappy, command) == -1)
        return -1;
    return 0;
}
