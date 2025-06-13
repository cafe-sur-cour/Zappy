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
#include <math.h>
#include <string.h>

static void print_broadcast_server(player_t *source, player_t *dest,
    int orientation)
{
    int l = int_str_len(source->id) + int_str_len(dest->id) +
        int_str_len(dest->posX) + int_str_len(dest->posY) + 1 + 57;
    char *d = calloc(l + 1, sizeof(char));

    if (!d) {
        error_message("Memory allocation failed for broadcast d print.");
        return;
    }
    snprintf(d, l + 1, "Player (%d) broadcasted to (%d) at (%d, %d). Dir: %d",
        source->id, dest->id, dest->posX, dest->posY, orientation);
    valid_message(d);
    free(d);
}

static int get_orientation(int x, int y)
{
    double angle = atan2(y, x) * 180.0 / M_PI;

    if (angle >= -22.5 && angle < 22.5)
        return 3;
    if (angle >= 22.5 && angle < 67.5)
        return 4;
    if (angle >= 67.5 && angle < 112.5)
        return 5;
    if (angle >= 112.5 && angle < 157.5)
        return 6;
    if ((angle >= 157.5 && angle <= 180.0) ||
        (angle >= -180.0 && angle < -157.5))
        return 7;
    if (angle >= -157.5 && angle < -112.5)
        return 8;
    if (angle >= -112.5 && angle < -67.5)
        return 1;
    if (angle >= -67.5 && angle < -22.5)
        return 2;
    return 0;
}

static int broadcast_text(player_t *source, player_t *dest, char *text)
{
    int dx = dest->posX - source->posX;
    int dy = dest->posY - source->posY;
    int orientation = get_orientation(dx, dy);
    int n = strlen(text) + int_str_len(orientation) + 11;
    char *message = malloc(sizeof(char) * (n + 1));

    if (!message) {
        error_message("Memory allocation failed for broadcast message.");
        return -1;
    }
    snprintf(message, n + 1, "message %d, %s\n", orientation, text);
    message[n] = '\0';
    if (write_message(dest->network->fd, message) == -1) {
        free(message);
        return -1;
    }
    print_broadcast_server(source, dest, orientation);
    free(message);
    return 0;
}

static int broadcast_to_team(team_t *team, player_t *source, char *text)
{
    player_t *save_player = team->players;

    (void)text;
    while (team->players) {
        if (team->players != source) {
            if (broadcast_text(source, team->players, text) == -1) {
                team->players = save_player;
                return -1;
            }
        }
        team->players = team->players->next;
    }
    team->players = save_player;
    return 0;
}

static int loop_thru_teams(player_t *source, zappy_t *zappy, char *text)
{
    team_t *save_team = zappy->game->teams;

    for (int i = 0; i < zappy->params->nb_team; i++) {
        if (broadcast_to_team(zappy->game->teams, source, text) == -1)
            return -1;
        zappy->game->teams = zappy->game->teams->next;
    }
    zappy->game->teams = save_team;
    return 0;
}

int handle_broadcast(player_t *player, char *command, zappy_t *zappy)
{
    char *text = &command[10];

    if (loop_thru_teams(player, zappy, text) == -1)
        return -1;
    if (write_message(player->network->fd, "ok\n") == -1)
        return -1;
    if (send_broadcast_to_all(zappy, command) == -1)
        return -1;
    return 0;
}
