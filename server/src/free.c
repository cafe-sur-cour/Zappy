/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** free
*/
#include "algo.h"
#include "zappy.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void *free_params(params_t *params)
{
    if (!params)
        return NULL;
    if (params->teams) {
        for (int i = 0; i < params->nb_team; i++) {
            free(params->teams[i]);
        }
        free(params->teams);
    }
    free(params);
    return NULL;
}

static void free_players(player_t *player)
{
    if (!player)
        return;
    if (player->inventory)
        free(player->inventory);
    if (player->network) {
        if (player->network->buffer)
            free(player->network->buffer);
        free(player->network);
    }
    free(player);
    return;
}

static void free_teams(team_t *teams)
{
    team_t *current = teams;
    team_t *next;

    while (current) {
        next = current->next;
        if (current->name) {
            free(current->name);
        }
        if (current->players) {
            free_players(current->players);
        }
        if (current)
            free(current);
        current = next;
    }
}

static void free_game(game_t *game)
{
    if (!game)
        return;
    if (game->teams)
        free_teams(game->teams);
    if (game->map)
        free_map(game->map);
    free(game);
}

static void free_graph(graph_net_t *graph)
{
    if (!graph)
        return;
    free(graph);
}

static void free_eggs(egg_t *currentEggs)
{
    egg_t *current = currentEggs;
    egg_t *next;

    while (current) {
        next = current->next;
        if (current->teamName)
            free(current->teamName);
        free(current);
        current = next;
    }
}

void free_map(map_t *map)
{
    for (int i = 0; i < map->height; i++) {
        if (map->tiles[i]) {
            free(map->tiles[i]);
        }
    }
    free_eggs(map->currentEggs);
    free(map->tiles);
    free(map);
}

void *free_zappy(zappy_t *zappy)
{
    if (!zappy)
        return NULL;
    if (zappy->params)
        free_params(zappy->params);
    if (zappy->network->sockfd != -1)
        close(zappy->network->sockfd);
    if (zappy->game)
        free_game(zappy->game);
    if (zappy->graph)
        free_graph(zappy->graph);
    if (zappy->network)
        free(zappy->network);
    free(zappy);
    return NULL;
}

void *free_player(player_t *player)
{
    if (!player)
        return NULL;
    if (player->inventory)
        free(player->inventory);
    free(player);
    return NULL;
}
