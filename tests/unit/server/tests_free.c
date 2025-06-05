/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_free
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <unistd.h>
#include "zappy.h"
#include "algo.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Test free_params with NULL
Test(free_params, test_null_params, .init = redirect_all_std)
{
    void *result = free_params(NULL);
    cr_assert_null(result);
}

// Test free_params with valid params but no teams
Test(free_params, test_params_no_teams, .init = redirect_all_std)
{
    params_t *params = malloc(sizeof(params_t));
    params->teams = NULL;
    params->nb_team = 0;
    
    void *result = free_params(params);
    cr_assert_null(result);
}

// Test free_params with valid params and teams
Test(free_params, test_params_with_teams, .init = redirect_all_std)
{
    params_t *params = malloc(sizeof(params_t));
    params->nb_team = 2;
    params->teams = malloc(sizeof(char*) * 2);
    params->teams[0] = malloc(10);
    params->teams[1] = malloc(10);
    
    void *result = free_params(params);
    cr_assert_null(result);
}

// Test free_server with NULL
Test(free_server, test_null_server, .init = redirect_all_std)
{
    void *result = free_zappy(NULL);
    cr_assert_null(result);
}

// // Test free_server with minimal server
// Test(free_server, test_minimal_server)
// {
//     zappy_t *server = malloc(sizeof(zappy_t));
//     memset(server, 0, sizeof(zappy_t));
//     server->params = NULL;
//     server->game = NULL;
//     server->network = NULL;
//     server->graph = NULL;

//     void *result = free_zappy(server);
//     cr_assert_null(result);
// }

// // Test free_server with valid socket
// Test(free_server, test_server_with_socket)
// {
//     zappy_t *server = malloc(sizeof(zappy_t));
//     memset(server, 0, sizeof(zappy_t));
//     server->params = NULL;
//     server->game = NULL;
//     server->network = NULL;
//     server->graph = NULL;
    
//     void *result = free_zappy(server);
//     cr_assert_null(result);
// }

// Test free_server with all components
// Test(free_server, test_complete_server)
// {
//     zappy_t *server = malloc(sizeof(zappy_t));
//     memset(server, 0, sizeof(zappy_t));
    
//     // Create params
//     server->params = malloc(sizeof(params_t));
//     memset(server->params, 0, sizeof(params_t));
//     server->params->teams = NULL;
//     server->params->nb_team = 0;
    
//     // Create game
//     server->game = malloc(sizeof(game_t));
//     memset(server->game, 0, sizeof(game_t));
//     server->game->teams = NULL;
//     server->game->map = malloc(sizeof(map_t));

//     server->network = NULL;
//     server->graph = NULL;


//     void *result = free_zappy(server);
//     cr_assert_null(result);
// }

// // Test game with teams
// Test(free_server, test_server_with_teams)
// {
//     zappy_t *server = malloc(sizeof(zappy_t));
//     memset(server, 0, sizeof(zappy_t));
//     server->params = NULL;
//     server->network = NULL;
//     server->graph = NULL;
    
//     // Create game with teams
//     server->game = malloc(sizeof(game_t));
//     memset(server->game, 0, sizeof(game_t));
//     server->game->map = NULL;

//     server->game->teams = malloc(sizeof(team_t));
//     memset(server->game->teams, 0, sizeof(team_t));
//     server->game->teams->name = strdup("test_team");
//     server->game->teams->players = NULL;
//     server->game->teams->next = NULL;
    
//     void *result = free_zappy(server);
//     cr_assert_null(result);
// }

// Test game with players
// Test(free_server, test_server_with_players)
// {
//     zappy_t *server = malloc(sizeof(zappy_t));
//     memset(server, 0, sizeof(zappy_t));
//     server->params = NULL;
//     server->network = NULL;
//     server->graph = NULL;
    
//     // Create game with teams and players
//     server->game = malloc(sizeof(game_t));
//     memset(server->game, 0, sizeof(game_t));
//     server->game->map = NULL;

//     server->game->teams = malloc(sizeof(team_t));
//     memset(server->game->teams, 0, sizeof(team_t));
//     server->game->teams->name = strdup("test_team");
//     server->game->teams->next = NULL;
    
//     // Create player
//     server->game->teams->players = malloc(sizeof(player_t));
//     memset(server->game->teams->players, 0, sizeof(player_t));
//     server->game->teams->players->inventory = malloc(sizeof(inventory_t));
//     memset(server->game->teams->players->inventory, 0, sizeof(inventory_t));
//     server->game->teams->players->network = NULL;
//     server->game->teams->players->next = NULL;
    
//     void *result = free_zappy(server);
//     cr_assert_null(result);
// }

// // Test game with ressources
// Test(free_server, test_server_with_ressources)
// {
//     zappy_t *server = malloc(sizeof(zappy_t));
//     memset(server, 0, sizeof(zappy_t));
//     server->params = NULL;
//     server->network = NULL;
//     server->graph = NULL;
    
//     // Create game with ressources
//     server->game = malloc(sizeof(game_t));
//     memset(server->game, 0, sizeof(game_t));
//     server->game->teams = NULL;
//     server->game->map = NULL;
    
//     void *result = free_zappy(server);
//     cr_assert_null(result);
// }

// // Test multiple players in chain
// Test(free_server, test_server_with_multiple_players)
// {
//     zappy_t *server = malloc(sizeof(zappy_t));
//     memset(server, 0, sizeof(zappy_t));
//     server->params = NULL;
//     server->network = NULL;
//     server->graph = NULL;
    
//     server->game = malloc(sizeof(game_t));
//     memset(server->game, 0, sizeof(game_t));
//     server->game->map = NULL;

//     server->game->teams = malloc(sizeof(team_t));
//     memset(server->game->teams, 0, sizeof(team_t));
//     server->game->teams->name = strdup("test_team");
//     server->game->teams->next = NULL;
    
//     // Create multiple players
//     server->game->teams->players = malloc(sizeof(player_t));
//     memset(server->game->teams->players, 0, sizeof(player_t));
//     server->game->teams->players->inventory = NULL;
//     server->game->teams->players->network = NULL;

//     server->game->teams->players->next = malloc(sizeof(player_t));
//     memset(server->game->teams->players->next, 0, sizeof(player_t));
//     server->game->teams->players->next->inventory = malloc(sizeof(inventory_t));
//     memset(server->game->teams->players->next->inventory, 0, sizeof(inventory_t));
//     server->game->teams->players->next->network = NULL;
//     server->game->teams->players->next->next = NULL;
    
//     void *result = free_zappy(server);
//     cr_assert_null(result);
// }

// // Test multiple teams in chain
// Test(free_server, test_server_with_multiple_teams)
// {
//     zappy_t *server = malloc(sizeof(zappy_t));
//     memset(server, 0, sizeof(zappy_t));
//     server->params = NULL;
//     server->network = NULL;
//     server->graph = NULL;
    
//     server->game = malloc(sizeof(game_t));
//     memset(server->game, 0, sizeof(game_t));
//     server->game->map = NULL;

//     // Create multiple teams
//     server->game->teams = malloc(sizeof(team_t));
//     memset(server->game->teams, 0, sizeof(team_t));
//     server->game->teams->name = strdup("team1");
//     server->game->teams->players = NULL;
//     server->game->teams->next = malloc(sizeof(team_t));
//     memset(server->game->teams->next, 0, sizeof(team_t));
//     server->game->teams->next->name = strdup("team2");
//     server->game->teams->next->players = NULL;
//     server->game->teams->next->next = NULL;
    
//     void *result = free_zappy(server);
//     cr_assert_null(result);
// }

// // Test graph with NULL pollfd
// Test(free_server, test_server_with_null_pollfd)
// {
//     zappy_t *server = malloc(sizeof(zappy_t));
//     memset(server, 0, sizeof(zappy_t));
//     server->params = NULL;
//     server->network = NULL;
//     server->game = NULL;
//     server->graph = NULL;

//     void *result = free_zappy(server);
//     cr_assert_null(result);
// }

Test(free_functions, free_params_with_teams)
{
    params_t *params = malloc(sizeof(params_t));
    params->teams = malloc(sizeof(char*) * 3);
    params->teams[0] = strdup("team1");
    params->teams[1] = strdup("team2");
    params->teams[2] = NULL;
    params->nb_team = 2;
    
    void *result = free_params(params);
    
    cr_assert_null(result);
}

Test(free_functions, free_params_null)
{
    void *result = free_params(NULL);
    
    cr_assert_null(result);
}

Test(free_functions, free_player_basic)
{
    player_t *player = malloc(sizeof(player_t));
    player->id = 1;
    player->level = 1;
    player->posX = 0;
    player->posY = 0;
    player->direction = NORTH;
    player->inventory = malloc(sizeof(inventory_t));
    player->network = malloc(sizeof(network_t));
    player->network->fd = -1;
    player->network->buffer = NULL;
    player->next = NULL;
    
    void *result = free_player(player);
    
    cr_assert_null(result);
}

Test(free_functions, free_player_null)
{
    void *result = free_player(NULL);
    
    cr_assert_null(result);
}

Test(free_functions, free_map_basic)
{
    map_t *map = malloc(sizeof(map_t));
    map->width = 3;
    map->height = 3;
    map->tiles = malloc(sizeof(inventory_t*) * 3);
    
    for (int i = 0; i < 3; i++) {
        map->tiles[i] = malloc(sizeof(inventory_t) * 3);
    }
    
    free_map(map);
    // Test passes if no segfault occurs
    cr_assert(true);
}

// Test(free_functions, free_zappy_complete)
// {
//     zappy_t *server = malloc(sizeof(zappy_t));
//     memset(server, 0, sizeof(zappy_t));
//     server->params = malloc(sizeof(params_t));
//     memset(server->params, 0, sizeof(params_t));
//     server->params->teams = NULL;
//     server->params->nb_team = 0;
//     server->game = malloc(sizeof(game_t));
//     memset(server->game, 0, sizeof(game_t));
//     server->game->teams = NULL;
//     server->game->map = NULL;
//     server->network = NULL;
//     server->graph = NULL;
    
//     void *result = free_zappy(server);
    
//     cr_assert_null(result);
// }

