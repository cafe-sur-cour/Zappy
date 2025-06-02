/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** map_init
*/

#include "zappy.h"
#include "algo.h"

#include <stdlib.h>
#include <stdio.h>


static map_t *create_map(server_t *server)
{
    map_t *map = malloc(sizeof(map_t));

    if (!map) {
        error_message("Failed to allocate memory for map.");
        exit(84);
    }
    map->width = server->params->x;
    map->heigt = server->params->y;
    map->teams = NULL;
    map->ressources = NULL;
    return map;
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
    ressources_t *current = server->map->ressources;

    printf("Map size: %d x %d\n", server->params->x, server->params->y);
    printf("Total resources: %d\n", total);
    while (current) {
        printf("Resource type: %s at position (%d, %d)\n",
            get_type(current->type), current->posX, current->posY);
        current = current->next;
    }
}

static void malloc_ressource(server_t *server, int j, int i, tiles_t *tiles)
{
    ressources_t *new_ressource = malloc(sizeof(ressources_t));

    if (!new_ressource)
        exit(84);
    new_ressource->type = i;
    new_ressource->posX = tiles[j].x;
    new_ressource->posY = tiles[j].y;
    new_ressource->next = server->map->ressources;
    server->map->ressources = new_ressource;
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
            malloc_ressource(server, j, i, tiles);
        }
    }
    if (server->params->isDebug == true)
        print_map(server);
}

void inti_map(server_t *server)
{
    server->map = create_map(server);
    init_tiles(server);
}
