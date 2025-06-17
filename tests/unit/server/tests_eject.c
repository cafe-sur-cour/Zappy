/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_eject
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include "zappy.h"
#include "network.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Setup function to initialize the zappy structure
static zappy_t *new_zappy(void)
{
    zappy_t *zappy = malloc(sizeof(zappy_t));
    if (!zappy)
        return NULL;

    zappy->params = malloc(sizeof(params_t));
    if (!zappy->params) {
        free(zappy);
        return NULL;
    }
    zappy->params->port = 4242;
    zappy->params->x = 10;
    zappy->params->y = 10;
    zappy->params->nb_team = 2;
    zappy->params->nb_client = 3;
    zappy->params->freq = 100;
    zappy->params->teams = malloc(sizeof(char *) * 3);
    if (!zappy->params->teams) {
        free(zappy->params);
        free(zappy);
        return NULL;
    }
    zappy->params->teams[0] = strdup("Team1");
    zappy->params->teams[1] = strdup("Team2");
    zappy->params->teams[2] = NULL;
    zappy->game = malloc(sizeof(game_t));
    if (!zappy->game) {
        free(zappy->params->teams[0]);
        free(zappy->params->teams[1]);
        free(zappy->params->teams);
        free(zappy->params);
        free(zappy);
        return NULL;
    }
    zappy->game->map = malloc(sizeof(map_t));
    if (!zappy->game->map) {
        free(zappy->game);
        free(zappy->params->teams[0]);
        free(zappy->params->teams[1]);
        free(zappy->params->teams);
        free(zappy->params);
        free(zappy);
        return NULL;
    }
    zappy->game->map->width = 10;
    zappy->game->map->height = 10;
    
    // Initialize teams
    zappy->game->teams = malloc(sizeof(team_t));
    zappy->game->teams->name = strdup("Team1");
    zappy->game->teams->nbPlayers = 0;
    zappy->game->teams->nbPlayerAlive = 0;
    zappy->game->teams->players = NULL;
    zappy->game->teams->next = malloc(sizeof(team_t));
    zappy->game->teams->next->name = strdup("Team2");
    zappy->game->teams->next->nbPlayers = 0;
    zappy->game->teams->next->nbPlayerAlive = 0;
    zappy->game->teams->next->players = NULL;
    zappy->game->teams->next->next = NULL;
    
    return zappy;
}

static player_t *create_test_player(int id, int x, int y, direction_t dir, int fd)
{
    player_t *player = malloc(sizeof(player_t));
    if (!player)
        return NULL;
    
    player->id = id;
    player->posX = x;
    player->posY = y;
    player->direction = dir;
    player->level = 1;
    player->team = strdup("Team1");
    player->next = NULL;
    
    player->network = malloc(sizeof(network_t));
    if (!player->network) {
        free(player->team);
        free(player);
        return NULL;
    }
    player->network->fd = fd;
    player->network->buffer = NULL;
    
    player->inventory = malloc(sizeof(inventory_t));
    if (!player->inventory) {
        free(player->network);
        free(player->team);
        free(player);
        return NULL;
    }
    memset(player->inventory, 0, sizeof(inventory_t));
    
    return player;
}

static void free_test_player(player_t *player)
{
    if (!player)
        return;
    free(player->team);
    free(player->network);
    free(player->inventory);
    free(player);
}

static void free_test_zappy(zappy_t *zappy)
{
    if (!zappy)
        return;
    if (zappy->params) {
        if (zappy->params->teams) {
            for (int i = 0; zappy->params->teams[i]; i++)
                free(zappy->params->teams[i]);
            free(zappy->params->teams);
        }
        free(zappy->params);
    }
    if (zappy->game) {
        if (zappy->game->teams) {
            team_t *current = zappy->game->teams;
            while (current) {
                team_t *next = current->next;
                free(current->name);
                // Free players if any
                player_t *player = current->players;
                while (player) {
                    player_t *next_player = player->next;
                    free_test_player(player);
                    player = next_player;
                }
                free(current);
                current = next;
            }
        }
        free(zappy->game->map);
        free(zappy->game);
    }
    free(zappy);
}

Test(eject, invalid_command, .init = redirect_all_std)
{
    zappy_t *zappy = new_zappy();
    player_t *player = create_test_player(1, 5, 5, NORTH, 1);
    
    cr_assert_not_null(zappy);
    cr_assert_not_null(player);
    
    int result = handle_eject(player, "InvalidCommand", zappy);
    cr_assert_eq(result, -1, "Should return -1 for invalid command");
    
    free_test_player(player);
    free_test_zappy(zappy);
}

Test(eject, no_players_to_eject, .init = redirect_all_std)
{
    zappy_t *zappy = new_zappy();
    player_t *player = create_test_player(1, 5, 5, NORTH, 1);
    
    cr_assert_not_null(zappy);
    cr_assert_not_null(player);
    
    // Add player to team but no other players on same tile
    zappy->game->teams->players = player;
    zappy->game->teams->nbPlayers = 1;
    
    int result = handle_eject(player, "Eject", zappy);
    cr_assert_eq(result, 0, "Should return 0 when no players to eject");
    
    free_test_zappy(zappy);
}

Test(eject, eject_single_player_north, .init = redirect_all_std)
{
    zappy_t *zappy = new_zappy();
    player_t *ejector = create_test_player(1, 5, 5, NORTH, 1);
    player_t *victim = create_test_player(2, 5, 5, EAST, 2);
    
    cr_assert_not_null(zappy);
    cr_assert_not_null(ejector);
    cr_assert_not_null(victim);
    
    // Link players
    ejector->next = victim;
    victim->next = NULL;
    
    // Add players to team
    zappy->game->teams->players = ejector;
    zappy->game->teams->nbPlayers = 2;
    
    int original_y = victim->posY;
    int result = handle_eject(ejector, "Eject", zappy);
    
    cr_assert_eq(result, 0, "Eject should complete successfully");
    cr_assert_eq(victim->posY, original_y - 1, "Victim should move north (y-1)");
    cr_assert_eq(victim->posX, 5, "Victim x position should remain same");
    
    free_test_zappy(zappy);
}

Test(eject, eject_single_player_south, .init = redirect_all_std)
{
    zappy_t *zappy = new_zappy();
    player_t *ejector = create_test_player(1, 5, 5, SOUTH, 1);
    player_t *victim = create_test_player(2, 5, 5, EAST, 2);
    
    cr_assert_not_null(zappy);
    cr_assert_not_null(ejector);
    cr_assert_not_null(victim);
    
    // Link players
    ejector->next = victim;
    victim->next = NULL;
    
    // Add players to team
    zappy->game->teams->players = ejector;
    zappy->game->teams->nbPlayers = 2;
    
    int original_y = victim->posY;
    int result = handle_eject(ejector, "Eject", zappy);
    
    cr_assert_eq(result, 0, "Eject should complete successfully");
    cr_assert_eq(victim->posY, original_y + 1, "Victim should move south (y+1)");
    cr_assert_eq(victim->posX, 5, "Victim x position should remain same");
    
    free_test_zappy(zappy);
}

Test(eject, eject_single_player_east, .init = redirect_all_std)
{
    zappy_t *zappy = new_zappy();
    player_t *ejector = create_test_player(1, 5, 5, EAST, 1);
    player_t *victim = create_test_player(2, 5, 5, NORTH, 2);
    
    cr_assert_not_null(zappy);
    cr_assert_not_null(ejector);
    cr_assert_not_null(victim);
    
    // Link players
    ejector->next = victim;
    victim->next = NULL;
    
    // Add players to team
    zappy->game->teams->players = ejector;
    zappy->game->teams->nbPlayers = 2;
    
    int original_x = victim->posX;
    int result = handle_eject(ejector, "Eject", zappy);
    
    cr_assert_eq(result, 0, "Eject should complete successfully");
    cr_assert_eq(victim->posX, original_x + 1, "Victim should move east (x+1)");
    cr_assert_eq(victim->posY, 5, "Victim y position should remain same");
    
    free_test_zappy(zappy);
}

Test(eject, eject_single_player_west, .init = redirect_all_std)
{
    zappy_t *zappy = new_zappy();
    player_t *ejector = create_test_player(1, 5, 5, WEST, 1);
    player_t *victim = create_test_player(2, 5, 5, NORTH, 2);
    
    cr_assert_not_null(zappy);
    cr_assert_not_null(ejector);
    cr_assert_not_null(victim);
    
    // Link players
    ejector->next = victim;
    victim->next = NULL;
    
    // Add players to team
    zappy->game->teams->players = ejector;
    zappy->game->teams->nbPlayers = 2;
    
    int original_x = victim->posX;
    int result = handle_eject(ejector, "Eject", zappy);
    
    cr_assert_eq(result, 0, "Eject should complete successfully");
    cr_assert_eq(victim->posX, original_x - 1, "Victim should move west (x-1)");
    cr_assert_eq(victim->posY, 5, "Victim y position should remain same");
    
    free_test_zappy(zappy);
}

Test(eject, torus_world_wrapping_north, .init = redirect_all_std)
{
    zappy_t *zappy = new_zappy();
    player_t *ejector = create_test_player(1, 5, 0, NORTH, 1);
    player_t *victim = create_test_player(2, 5, 0, EAST, 2);
    
    cr_assert_not_null(zappy);
    cr_assert_not_null(ejector);
    cr_assert_not_null(victim);
    
    // Link players
    ejector->next = victim;
    victim->next = NULL;
    
    // Add players to team
    zappy->game->teams->players = ejector;
    zappy->game->teams->nbPlayers = 2;
    
    int result = handle_eject(ejector, "Eject", zappy);
    
    cr_assert_eq(result, 0, "Eject should complete successfully");
    cr_assert_eq(victim->posY, zappy->params->y - 1, "Victim should wrap to bottom of map");
    cr_assert_eq(victim->posX, 5, "Victim x position should remain same");
    
    free_test_zappy(zappy);
}

Test(eject, torus_world_wrapping_south, .init = redirect_all_std)
{
    zappy_t *zappy = new_zappy();
    player_t *ejector = create_test_player(1, 5, 9, SOUTH, 1);
    player_t *victim = create_test_player(2, 5, 9, EAST, 2);
    
    cr_assert_not_null(zappy);
    cr_assert_not_null(ejector);
    cr_assert_not_null(victim);
    
    // Link players
    ejector->next = victim;
    victim->next = NULL;
    
    // Add players to team
    zappy->game->teams->players = ejector;
    zappy->game->teams->nbPlayers = 2;
    
    int result = handle_eject(ejector, "Eject", zappy);
    
    cr_assert_eq(result, 0, "Eject should complete successfully");
    cr_assert_eq(victim->posY, 0, "Victim should wrap to top of map");
    cr_assert_eq(victim->posX, 5, "Victim x position should remain same");
    
    free_test_zappy(zappy);
}

Test(eject, torus_world_wrapping_east, .init = redirect_all_std)
{
    zappy_t *zappy = new_zappy();
    player_t *ejector = create_test_player(1, 9, 5, EAST, 1);
    player_t *victim = create_test_player(2, 9, 5, NORTH, 2);
    
    cr_assert_not_null(zappy);
    cr_assert_not_null(ejector);
    cr_assert_not_null(victim);
    
    // Link players
    ejector->next = victim;
    victim->next = NULL;
    
    // Add players to team
    zappy->game->teams->players = ejector;
    zappy->game->teams->nbPlayers = 2;
    
    int result = handle_eject(ejector, "Eject", zappy);
    
    cr_assert_eq(result, 0, "Eject should complete successfully");
    cr_assert_eq(victim->posX, 0, "Victim should wrap to left side of map");
    cr_assert_eq(victim->posY, 5, "Victim y position should remain same");
    
    free_test_zappy(zappy);
}

Test(eject, torus_world_wrapping_west, .init = redirect_all_std)
{
    zappy_t *zappy = new_zappy();
    player_t *ejector = create_test_player(1, 0, 5, WEST, 1);
    player_t *victim = create_test_player(2, 0, 5, NORTH, 2);
    
    cr_assert_not_null(zappy);
    cr_assert_not_null(ejector);
    cr_assert_not_null(victim);
    
    // Link players
    ejector->next = victim;
    victim->next = NULL;
    
    // Add players to team
    zappy->game->teams->players = ejector;
    zappy->game->teams->nbPlayers = 2;
    
    int result = handle_eject(ejector, "Eject", zappy);
    
    cr_assert_eq(result, 0, "Eject should complete successfully");
    cr_assert_eq(victim->posX, zappy->params->x - 1, "Victim should wrap to right side of map");
    cr_assert_eq(victim->posY, 5, "Victim y position should remain same");
    
    free_test_zappy(zappy);
}


Test(eject, players_on_different_tiles, .init = redirect_all_std)
{
    zappy_t *zappy = new_zappy();
    player_t *ejector = create_test_player(1, 5, 5, NORTH, 1);
    player_t *other_player = create_test_player(2, 7, 7, EAST, 2);
    
    cr_assert_not_null(zappy);
    cr_assert_not_null(ejector);
    cr_assert_not_null(other_player);
    
    // Link players
    ejector->next = other_player;
    other_player->next = NULL;
    
    // Add players to team
    zappy->game->teams->players = ejector;
    zappy->game->teams->nbPlayers = 2;
    
    int original_x = other_player->posX;
    int original_y = other_player->posY;
    int result = handle_eject(ejector, "Eject", zappy);
    
    cr_assert_eq(result, 0, "Eject should complete successfully");
    cr_assert_eq(other_player->posX, original_x, "Other player should not move");
    cr_assert_eq(other_player->posY, original_y, "Other player should not move");
    
    free_test_zappy(zappy);
}
