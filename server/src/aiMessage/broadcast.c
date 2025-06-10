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

static int broadcast_text(player_t *player, char *command, char *text)
{
    char *orientation = get_orientation();
    int n = strlen(text) + 12;
    char *message = malloc(sizeof(char) * (n + 1));

    if (!message) {
        error_message("Memory allocation failed for broadcast message.");
        return -1;
    }
    snprintf(message, n + 1, "message %d, %s\n", player->id, text);
    message[n] = '\0';
    if (write_message(player->network->fd, message) == -1) {
        free(message);
        return -1;
    }
    free(message);
    valid_message("Broadcast sent to all Trantorians.");
    return 0;
}

int handle_broadcast(player_t *player, char *command, zappy_t *zappy)
{
    char *text = &command[10];

    (void)player;
    if (broadcast_text(player, command, text) == -1)
        return -1;
    if (write_message(player->network->fd, "ok\n") == -1)
        return -1;
    if (send_broadcast_to_all(zappy, command) == -1)
        return -1;
    return 0;
}
