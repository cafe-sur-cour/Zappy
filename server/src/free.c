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

static void free_ressources(ressources_t *ressources)
{
    ressources_t *current = ressources;
    ressources_t *next;

    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
}

static void free_players(player_t *player)
{
    player_t *next_player;

    while (player) {
        next_player = player->next;
        if (player->inventory) {
            free(player->inventory);
        }
        if (player->lives) {
            free(player->lives);
        }
        free(player);
        player = next_player;
    }
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

static void free_map(map_t *map)
{
    if (!map)
        return;
    if (map->teams)
        free_teams(map->teams);
    if (map->ressources)
        free_ressources(map->ressources);
    free(map);
}

static void free_graph(graph_t *graph)
{
    if (!graph)
        return;
    free(graph);
}

void *free_server(server_t *server)
{
    if (!server)
        return NULL;
    if (server->params)
        free_params(server->params);
    if (server->sockfd != -1)
        close(server->sockfd);
    if (server->map)
        free_map(server->map);
    if (server->graph)
        free_graph(server->graph);
    free(server);
    return NULL;
}
