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


/* This function initialize the Game structure */
static game_t *create_game(void)
{
    game_t *game = malloc(sizeof(game_t));

    if (!game) {
        error_message("Failed to allocate memory for game structure.");
        exit(84);
    }
    game->teams = NULL;
    game->map = NULL;
    return game;
}

/* This is a function for debug purposses that prints the map tiles*/
static void print_tile(zappy_t *zappy, int x, int y)
{
    inventory_t *tile = NULL;

    if (y < 0 || y >= zappy->game->map->height ||
        x < 0 || x >= zappy->game->map->width) {
        printf("Error: Coordinates (%d,%d) out of bounds\n", x, y);
        return;
    }
    tile = &zappy->game->map->tiles[y][x];
    printf("Tile (%d,%d): F:%d L:%d D:%d S:%d M:%d P:%d T:%d\n",
        x, y, tile->nbFood, tile->nbLinemate, tile->nbDeraumere,
        tile->nbSibur, tile->nbMendiane, tile->nbPhiras, tile->nbThystame);
}

/* This function allows use to pritn the tiles */
static void print_map_tiles(zappy_t *zappy)
{
    printf("Map size: %d x %d\n", zappy->params->x, zappy->params->y);
    for (int y = 0; y < zappy->params->y; y++) {
        for (int x = 0; x < zappy->params->x; x++) {
            print_tile(zappy, x, y);
        }
    }
}

/* This function distribute a rzndom position from the tiles */
static int *distrib_tiles(int *tile_index, tiles_t *shuffled_tiles,
    int mapValue)
{
    int *pos = malloc(sizeof(int) * 2);

    if (pos == NULL) {
        error_message("Failed to allocate memory for tile position.");
        exit(84);
    }
    if (*tile_index >= mapValue) {
        *tile_index = 0;
    }
    pos[0] = shuffled_tiles[*tile_index].x;
    pos[1] = shuffled_tiles[*tile_index].y;
    (*tile_index)++;
    return pos;
}

/* This function initialize the tiles of the map */
static map_t *set_tile(int x, int y, map_t *map, int type)
{
    inventory_t *tile = &map->tiles[y][x];
    int *counters[] = { &tile->nbFood, &tile->nbLinemate, &tile->nbDeraumere,
        &tile->nbSibur, &tile->nbMendiane, &tile->nbPhiras, &tile->nbThystame};

    if (type >= 0 && type < 7)
        (*counters[type])++;
    return map;
}

/* This function distrbute the ressources on the tiles */
static void distribute_resources(zappy_t *server)
{
    int mapValue = server->params->x * server->params->y;
    float density[7] = {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};
    int resources_count[7];
    tiles_t *shuffled_tiles = shuffle_fisher(server->params->x,
        server->params->y);
    int tile_index = 0;
    int *pos = NULL;

    for (int i = 0; i < 7; i++)
        resources_count[i] = (int)(mapValue * density[i]);
    for (int type = 0; type < 7; type++) {
        for (int count = 0; count < resources_count[type]; count++) {
            pos = distrib_tiles(&tile_index, shuffled_tiles, mapValue);
            server->game->map = set_tile(pos[0], pos[1], server->game->map,
                type);
            free(pos);
        }
    }
    free(shuffled_tiles);
}

static map_t *malloc_tiles(int width, int height,
    map_t *map)
{
    map->currentEggs = NULL;
    for (int i = 0; i < height; i++) {
        map->tiles[i] = calloc(width, sizeof(inventory_t));
        if (!map->tiles[i]) {
            error_message("Failed to allocate memory for map row.");
            free_map(map);
            exit(84);
        }
    }
    return map;
}

static map_t *create_map(int width, int height)
{
    map_t *map = malloc(sizeof(map_t));

    if (!map) {
        error_message("Failed to allocate memory for map structure.");
        exit(84);
    }
    map->width = width;
    map->height = height;
    map->tiles = malloc(sizeof(inventory_t *) * height);
    if (!map->tiles) {
        error_message("Failed to allocate memory for map tiles.");
        free_map(map);
        exit(84);
    }
    map = malloc_tiles(width, height, map);
    return map;
}

static void init_teams(zappy_t *server)
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

void init_game(zappy_t *zappy)
{
    zappy->game = create_game();
    zappy->game->map = create_map(zappy->params->x, zappy->params->y);
    distribute_resources(zappy);
    init_teams(zappy);
    if (zappy->params->is_debug == true)
        print_map_tiles(zappy);
    init_egg(zappy);
}
