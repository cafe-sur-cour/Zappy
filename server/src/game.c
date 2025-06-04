/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** map_init
*/

#include "zappy.h"
#include "algo.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static game_t *create_game(server_t *server)
{
    game_t *game = malloc(sizeof(game_t));

    if (!game) {
        error_message("Failed to allocate memory for game structure.");
        exit(84);
    }
    game->width = server->params->x;
    game->heigt = server->params->y;
    game->teams = NULL;
    game->ressources = NULL;
    return game;
}

static int nb_total(server_t *server)
{
    int mapValue = server->params->x * server->params->y;
    float density[7] = {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};
    int total = ((mapValue * density[0]) + (mapValue * density[1]) + (mapValue
        * density[2]) + (mapValue * density[3]) + (mapValue * density[4]) +
        (mapValue * density[5]) + (mapValue * density[6]));

    return total;
}

static const char *get_type(crystal_t type)
{
    const char *crystal_names[] = {"food", "linemate", "deraumere", "sibur",
        "mendiane", "phiras", "thystame" };

    if (type >= 0 && type < 7)
        return crystal_names[type];
    return "unknown";
}

static void print_map(server_t *server)
{
    int total = nb_total(server);
    ressources_t *current = server->game->ressources;

    printf("Map size: %d x %d\n", server->params->x, server->params->y);
    printf("Total resources: %d\n", total);
    while (current) {
        printf("Resource type: %s at position (%d, %d)\n",
            get_type(current->type), current->posX, current->posY);
        current = current->next;
    }
}

static ressources_t *init_ressource(int j, int i, tiles_t *tiles,
    ressources_t *ressource)
{
    ressources_t *new_ressource = malloc(sizeof(ressources_t));

    if (!new_ressource)
        exit(84);
    new_ressource->type = i;
    new_ressource->posX = tiles[j].x;
    new_ressource->posY = tiles[j].y;
    new_ressource->next = ressource;
    return new_ressource;
}

static void init_tiles(server_t *server)
{
    int mapV = server->params->x * server->params->y;
    float den[7] = {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};
    int nb[7] = {(mapV * den[0]), (mapV * den[1]), (mapV * den[2]), (mapV *
        den[3]), (mapV * den[4]), (mapV * den[5]), (mapV * den[6])};
    tiles_t *tiles = shuffle_fisher(server->params->x, server->params->y);

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < nb[i]; j++) {
            server->game->ressources = init_ressource(j, i, tiles,
                server->game->ressources);
        }
    }
    if (server->params->is_debug == true)
        print_map(server);
}

static void init_teams(server_t *server)
{
    team_t *current = server->game->teams;

    for (int i = 0; i < server->params->nb_team; i++) {
        current = malloc(sizeof(team_t));
        if (!current) {
            error_message("Failed to allocate memory for team structure.");
            exit(84);
        }
        current->name = strdup(server->params->teams[i]);
        current->nbPlayers = 0;
        current->nbPlayerAlive = 0;
        current->players = NULL;
        current->next = server->game->teams;
        server->game->teams = current;
    }
}

void init_game(server_t *server)
{
    server->game = create_game(server);
    init_tiles(server);
    init_teams(server);
}
