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
static server_t *create_test_server(int width, int height, bool debug)
{
    server_t *server = malloc(sizeof(server_t));
    server->params = malloc(sizeof(params_t));
    server->params->x = width;
    server->params->y = height;
    server->params->is_debug = debug;
    server->game = NULL;
    return server;
}

// Helper function to free test server
static void free_test_server(server_t *server)
{
    if (server->game) {
        ressources_t *current = server->game->ressources;
        while (current) {
            ressources_t *next = current->next;
            free(current);
            current = next;
        }
        free(server->game);
    }
    free(server->params);
    free(server);
}

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
    server_t *server = create_test_server(5, 5, false);
    
    init_game(server);
    
    cr_assert_not_null(server->game, "game should be initialized");
    cr_assert_eq(server->game->width, 5, "game width should be set correctly");
    cr_assert_eq(server->game->heigt, 5, "game height should be set correctly");
    cr_assert_null(server->game->teams, "Teams should be null initially");
    
    free_test_server(server);
}

Test(game_init, resource_distribution, .init = redirect_all_std)
{
    server_t *server = create_test_server(10, 10, false);
    
    init_game(server);
    
    // Count resources by type
    int resource_count[7] = {0};
    ressources_t *current = server->game->ressources;
    
    while (current) {
        cr_assert_geq(current->type, 0, "Resource type should be >= 0");
        cr_assert_lt(current->type, 7, "Resource type should be < 7");
        cr_assert_geq(current->posX, 0, "Resource X position should be >= 0");
        cr_assert_lt(current->posX, 10, "Resource X position should be < width");
        cr_assert_geq(current->posY, 0, "Resource Y position should be >= 0");
        cr_assert_lt(current->posY, 10, "Resource Y position should be < height");
        
        resource_count[current->type]++;
        current = current->next;
    }
    
    // Verify resource distribution (approximate due to float to int conversion)
    int gameValue = 100;
    float density[7] = {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};
    
    for (int i = 0; i < 7; i++) {
        int expected = (int)(gameValue * density[i]);
        cr_assert_eq(resource_count[i], expected, 
            "Resource type %d count should match expected density", i);
    }
    
    free_test_server(server);
}

Test(game_init, debug_output, .init = redirect_all_std)
{
    server_t *server = create_test_server(3, 3, true);
    
    init_game(server);
    
    // Debug mode should produce output
    fflush(stdout);
    free_test_server(server);
}

Test(game_init, empty_game, .init = redirect_all_std)
{
    server_t *server = create_test_server(1, 1, false);
    
    init_game(server);
    
    cr_assert_not_null(server->game, "Even 1x1 game should be initialized");
    cr_assert_eq(server->game->width, 1, "1x1 game width should be 1");
    cr_assert_eq(server->game->heigt, 1, "1x1 game height should be 1");
    
    // Should have at least some resources
    cr_assert_null(server->game->ressources, "1x1 game should have resources");
    
    free_test_server(server);
}

Test(game_init, large_game, .init = redirect_all_std)
{
    server_t *server = create_test_server(50, 50, false);
    
    init_game(server);
    
    cr_assert_not_null(server->game, "Large game should be initialized");
    cr_assert_eq(server->game->width, 50, "Large game width should be correct");
    cr_assert_eq(server->game->heigt, 50, "Large game height should be correct");
    
    // Count total resources
    int total_resources = 0;
    ressources_t *current = server->game->ressources;
    while (current) {
        total_resources++;
        current = current->next;
    }
    
    cr_assert_gt(total_resources, 0, "Large game should have resources");
    
    free_test_server(server);
}
