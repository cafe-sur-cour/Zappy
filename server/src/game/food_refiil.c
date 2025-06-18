/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** food-refiil
*/

#include "zappy.h"
#include "game.h"


/* This function countes the current state of the ressources */
void count_current_resources(zappy_t *z, int current_count[7])
{
    inventory_t *tile = NULL;

    for (int i = 0; i < 7; i++)
        current_count[i] = 0;
    for (int y = 0; y < z->game->map->height; y++) {
        for (int x = 0; x < z->game->map->width; x++) {
            tile = &z->game->map->tiles[x][y];
            current_count[0] += tile->nbFood;
            current_count[1] += tile->nbLinemate;
            current_count[2] += tile->nbDeraumere;
            current_count[3] += tile->nbSibur;
            current_count[4] += tile->nbMendiane;
            current_count[5] += tile->nbPhiras;
            current_count[6] += tile->nbThystame;
        }
    }
}

/* This function verifys calls the distributin of ressources */
void refill_food(zappy_t *zappy)
{
    int is_updated = 0;

    if (!zappy || !zappy->game || !zappy->game->map ||
        !zappy->game->map->tiles) {
        error_message("Invalid zappy structure for resource refill.");
        return;
    }
    is_updated = distribute_resources(zappy);
    if (is_updated == 0)
        return;
    valid_message("Resources refilled on the map.");
    send_entrie_map(zappy);
}
