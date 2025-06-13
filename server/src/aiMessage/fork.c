/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** fork
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int get_nb_eggs(zappy_t *zappy)
{
    egg_t *current = zappy->game->map->currentEggs;
    int count = 0;

    while (current) {
        count++;
        current = current->next;
    }
    return count;
}

static void push_back_egg(zappy_t *zappy, egg_t *new)
{
    egg_t *save = zappy->game->map->currentEggs;

    if (!save) {
        zappy->game->map->currentEggs = new;
        return;
    }
    while (zappy->game->map->currentEggs->next) {
        zappy->game->map->currentEggs = zappy->game->map->currentEggs->next;
    }
    zappy->game->map->currentEggs->next = new;
    zappy->game->map->currentEggs = save;
}

static int team_allows_connection(player_t *player, zappy_t *zappy)
{
    team_t *current_team = zappy->game->teams;
    char *team_name = player->team;

    while (current_team) {
        if (strcmp(current_team->name, team_name) == 0) {
            break;
        }
        current_team = current_team->next;
    }
    if (current_team->nbPlayerAlive < zappy->params->nb_client)
        return 0;
    return -1;
}

int handle_fork(player_t *player, char *command, zappy_t *zappy)
{
    int id = get_nb_eggs(zappy);
    int pos[2] = {player->posX, player->posY};
    egg_t *new = NULL;

    if (strcmp(command, "Fork") != 0) {
        error_message("Invalid command for fork handling.");
        return -1;
    }
    if (team_allows_connection(player, zappy) == -1)
        return -1;
    new = add_egg_node(id, pos, player->team, player->id);
    if (!new) {
        error_message("Failed to create a new egg.");
        return -1;
    }
    push_back_egg(zappy, new);
    send_player_laying_egg(zappy, player);
    return 0;
}
