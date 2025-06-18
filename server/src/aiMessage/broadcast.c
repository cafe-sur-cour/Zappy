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

static int adjust_for_orientation(int direction_number,
    direction_t orientation)
{
    int offset = (2 * (orientation - 1)) % 8;
    int adjusted = ((direction_number - 1 - offset + 8) % 8) + 1;

    return adjusted;
}

static void wrap_coordinates(int *dx, int *dy, int map_width, int map_height)
{
    if (*dx > map_width / 2)
        *dx -= map_width;
    else if (*dx < - map_width / 2)
        *dx += map_width;
    if (*dy > map_height / 2)
        *dy -= map_height;
    else if (*dy < - map_height / 2)
        *dy += map_height;
}

static int get_orientation(int dx, int dy)
{
    if (dx == 0 && dy == 0)
        return 0;
    if (dx == 0 && dy < 0)
        return 1;
    if (dx == 1 && dy < 0)
        return 8;
    if (dx > 0 && dy == 0)
        return 7;
    if (dx > 0 && dy > 0)
        return 6;
    if (dx == 0 && dy > 0)
        return 5;
    if (dx < 0 && dy > 0)
        return 4;
    if (dx < 0 && dy == 0)
        return 3;
    if (dx < 0 && dy < 0)
        return 2;
    return 0;
}

int broadcast_text(player_t *source, player_t *dest, char *text,
    zappy_t *zappy)
{
    int d[2] = {dest->posX - source->posX, dest->posY - source->posY};
    int dir[3] = {0};
    char *message = NULL;

    wrap_coordinates(&d[0], &d[1], zappy->params->x, zappy->params->y);
    dir[0] = get_orientation(d[0], d[1]);
    dir[1] = adjust_for_orientation(dir[0], dest->direction);
    dir[3] = strlen(text) + int_str_len(dir[1]) + 11;
    message = malloc(sizeof(char) * (dir[3] + 1));
    if (!message)
        return -1;
    snprintf(message, dir[3] + 1, "message %d, %s\n", dir[1], text);
    message[dir[3]] = '\0';
    if (write_message(dest->network->fd, message) == -1) {
        free(message);
        return -1;
    }
    print_broadcast_server(source, dest, dir[1]);
    free(message);
    return 0;
}

static int send_broadcast(team_t *team, player_t *source, char *text,
    zappy_t *zappy)
{
    if (team->players != source) {
        if (broadcast_text(source, team->players, text, zappy) == -1) {
            return -1;
        }
    }
    return 0;
}

static int broadcast_to_team(team_t *team, player_t *source, char *text,
    zappy_t *zappy)
{
    player_t *save_player = team->players;

    (void)text;
    while (team->players) {
        if (source->id == team->players->id) {
            team->players = team->players->next;
        }
        if (send_broadcast(team, source, text, zappy) == -1) {
            team->players = save_player;
            return -1;
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
        if (broadcast_to_team(zappy->game->teams, source, text, zappy) == -1)
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
    if (send_broadcast_to_player(zappy, player, text) == -1)
        return -1;
    return 0;
}
