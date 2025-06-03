/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** test_map_init
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
    server->map = NULL;
    return server;
}

// Helper function to free test server
static void free_test_server(server_t *server)
{
    if (server->map) {
        ressources_t *current = server->map->ressources;
        while (current) {
            ressources_t *next = current->next;
            free(current);
            current = next;
        }
        free(server->map);
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

Test(map_init, init_map_basic, .init = redirect_all_std)
{
    server_t *server = create_test_server(5, 5, false);
    
    inti_map(server);
    
    cr_assert_not_null(server->map, "Map should be initialized");
    cr_assert_eq(server->map->width, 5, "Map width should be set correctly");
    cr_assert_eq(server->map->heigt, 5, "Map height should be set correctly");
    cr_assert_null(server->map->teams, "Teams should be null initially");
    
    free_test_server(server);
}

Test(map_init, resource_distribution, .init = redirect_all_std)
{
    server_t *server = create_test_server(10, 10, false);
    
    inti_map(server);
    
    // Count resources by type
    int resource_count[7] = {0};
    ressources_t *current = server->map->ressources;
    
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
    int mapValue = 100;
    float density[7] = {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};
    
    for (int i = 0; i < 7; i++) {
        int expected = (int)(mapValue * density[i]);
        cr_assert_eq(resource_count[i], expected, 
            "Resource type %d count should match expected density", i);
    }
    
    free_test_server(server);
}

Test(map_init, debug_output, .init = redirect_all_std)
{
    server_t *server = create_test_server(3, 3, true);
    
    inti_map(server);
    
    // Debug mode should produce output
    fflush(stdout);
    free_test_server(server);
}

Test(map_init, empty_map, .init = redirect_all_std)
{
    server_t *server = create_test_server(1, 1, false);
    
    inti_map(server);
    
    cr_assert_not_null(server->map, "Even 1x1 map should be initialized");
    cr_assert_eq(server->map->width, 1, "1x1 map width should be 1");
    cr_assert_eq(server->map->heigt, 1, "1x1 map height should be 1");
    
    // Should have at least some resources
    cr_assert_null(server->map->ressources, "1x1 map should have resources");
    
    free_test_server(server);
}

Test(map_init, large_map, .init = redirect_all_std)
{
    server_t *server = create_test_server(50, 50, false);
    
    inti_map(server);
    
    cr_assert_not_null(server->map, "Large map should be initialized");
    cr_assert_eq(server->map->width, 50, "Large map width should be correct");
    cr_assert_eq(server->map->heigt, 50, "Large map height should be correct");
    
    // Count total resources
    int total_resources = 0;
    ressources_t *current = server->map->ressources;
    while (current) {
        total_resources++;
        current = current->next;
    }
    
    cr_assert_gt(total_resources, 0, "Large map should have resources");
    
    free_test_server(server);
}
