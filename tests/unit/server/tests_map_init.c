/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** test_game_init
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <unistd.h>

#include "zappy.h"
#include "algo.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Helper function to create test server
static zappy_t *create_test_server(int width, int height, bool debug)
{
    zappy_t *server = malloc(sizeof(zappy_t));
    server->params = malloc(sizeof(params_t));
    server->params->x = width;
    server->params->y = height;
    server->params->is_debug = debug;
    server->game = NULL;
    return server;
}

// Helper function to free test server

// Test shuffle_fisher function
Test(algo, shuffle_fisher_basic, .init = redirect_all_std)
{
    tiles_t *tiles = shuffle_fisher(3, 3);
    
    cr_assert_not_null(tiles, "shuffle_fisher should return non-null array");
    
    // Check that all coordinates are valid
    for (int i = 0; i < 9; i++) {
        cr_assert_geq(tiles[i].x, 0, "x coordinate should be >= 0");
        cr_assert_lt(tiles[i].x, 3, "x coordinate should be < width");
        cr_assert_geq(tiles[i].y, 0, "y coordinate should be >= 0");
        cr_assert_lt(tiles[i].y, 3, "y coordinate should be < height");
    }
    
    free(tiles);
}

Test(algo, shuffle_fisher_coverage, .init = redirect_all_std)
{
    tiles_t *tiles = shuffle_fisher(2, 2);
    bool found[4] = {false, false, false, false};
    
    // Check that all expected tiles are present
    for (int i = 0; i < 4; i++) {
        int index = tiles[i].y * 2 + tiles[i].x;
        cr_assert_geq(index, 0, "Tile index should be >= 0");
        cr_assert_lt(index, 4, "Tile index should be < total");
        found[index] = true;
    }
    
    // Verify all tiles are present
    for (int i = 0; i < 4; i++) {
        cr_assert(found[i], "All tiles should be present in shuffled array");
    }
    
    free(tiles);
}

Test(game_init, init_game_basic, .init = redirect_all_std)
{
    zappy_t *server = create_test_server(5, 5, false);
    
    init_game(server);
    
    cr_assert_not_null(server->game, "game should be initialized");
    cr_assert_eq(server->game->map->width, 5, "game width should be set correctly");
    cr_assert_eq(server->game->map->height, 5, "game height should be set correctly");
    cr_assert_null(server->game->teams, "Teams should be null initially");
}

Test(game_init, resource_distribution, .init = redirect_all_std)
{
    zappy_t *server = create_test_server(10, 10, false);
    
    init_game(server);

}

Test(game_init, debug_output, .init = redirect_all_std)
{
    zappy_t *server = create_test_server(3, 3, true);
    
    init_game(server);
    
    // Debug mode should produce output
    fflush(stdout);
    
}

Test(game_init, empty_game, .init = redirect_all_std)
{
    zappy_t *server = create_test_server(1, 1, false);
    
    init_game(server);
    
    cr_assert_not_null(server->game, "Even 1x1 game should be initialized");
    cr_assert_eq(server->game->map->width, 1, "1x1 game width should be 1");
    cr_assert_eq(server->game->map->height, 1, "1x1 game height should be 1");

    
}

Test(game_init, large_game, .init = redirect_all_std)
{
    zappy_t *server = create_test_server(50, 50, false);
    
    init_game(server);
    
    cr_assert_not_null(server->game, "Large game should be initialized");
    cr_assert_eq(server->game->map->width, 50, "Large game width should be correct");
    cr_assert_eq(server->game->map->height, 50, "Large game height should be correct");

    
}

Test(map_init, init_game_basic, .init = redirect_all_std)
{
    params_t params = {
        .port = 8080,
        .x = 5,
        .y = 5,
        .nb_team = 2,
        .teams = (char*[]){"team1", "team2"},
        .nb_client = 3,
        .freq = 100,
        .is_debug = false
    };
    
    zappy_t server = {
        .params = &params,
        .game = NULL,
        .network = NULL,
        .graph = NULL
    };
    
    init_game(&server);
    
    cr_assert_not_null(server.game);
    cr_assert_not_null(server.game->map);
    cr_assert_eq(server.game->map->width, 5);
    cr_assert_eq(server.game->map->height, 5);
    cr_assert_not_null(server.game->map->tiles);
    cr_assert_not_null(server.game->teams);
    
    free_map(server.game->map);
}

Test(map_init, map_tiles_initialized, .init = redirect_all_std)
{
    params_t params = {
        .x = 3,
        .y = 3,
        .nb_team = 1,
        .teams = (char*[]){"team1"},
        .nb_client = 1,
        .freq = 100
    };
    
    zappy_t server = {
        .params = &params,
        .game = NULL
    };
    
    init_game(&server);
    
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            cr_assert_not_null(server.game->map->tiles[y]);
            cr_assert_not_null(&server.game->map->tiles[y][x]);
        }
    }
    
    free_map(server.game->map);
}
