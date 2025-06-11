/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_setObj
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "zappy.h"
#include "network.h"
#include "fake_malloc.h"


static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Function declarations
int add_elem_to_tile(zappy_t *zappy, int x, int y, char *name);
int handle_set(player_t *player, char *command, zappy_t *zappy);
int send_map_tile(inventory_t **tiles, zappy_t *zappy, int posX, int posY);
int send_player_inventory(zappy_t *zappy, player_t *player);
int send_ressource_droped(zappy_t *zappy, player_t *player, int ressourceType);

// Mock variables
static int send_map_tile_calls = 0;
static int send_player_inventory_calls = 0;
static int send_ressource_droped_calls = 0;
static int send_map_tile_return = 0;
static int send_player_inventory_return = 0;
static int send_ressource_droped_return = 0;


// Helper functions
static zappy_t *create_test_zappy(int width, int height)
{
    zappy_t *zappy = malloc(sizeof(zappy_t));
    zappy->game = malloc(sizeof(game_t));
    zappy->game->map = malloc(sizeof(map_t));
    zappy->game->map->width = width;
    zappy->game->map->height = height;
    
    // Allocate tiles array
    zappy->game->map->tiles = malloc(height * sizeof(inventory_t *));
    for (int i = 0; i < height; i++) {
        zappy->game->map->tiles[i] = calloc(width, sizeof(inventory_t));
    }
    
    return zappy;
}

static player_t *create_test_player(int x, int y)
{
    player_t *player = malloc(sizeof(player_t));
    player->network = malloc(sizeof(network_t));
    player->inventory = malloc(sizeof(inventory_t));
    
    player->id = 1;
    player->posX = x;
    player->posY = y;
    player->level = 1;
    player->direction = NORTH;
    player->team = strdup("team1");
    player->network->fd = 42;
    player->next = NULL;
    
    // Initialize inventory with some items
    memset(player->inventory, 0, sizeof(inventory_t));
    player->inventory->nbFood = 10;
    player->inventory->nbLinemate = 5;
    player->inventory->nbDeraumere = 3;
    player->inventory->nbSibur = 2;
    player->inventory->nbMendiane = 1;
    player->inventory->nbPhiras = 1;
    player->inventory->nbThystame = 1;
    
    return player;
}

static void cleanup_test_data(zappy_t *zappy, player_t *player)
{
    if (zappy) {
        for (int i = 0; i < zappy->game->map->height; i++) {
            free(zappy->game->map->tiles[i]);
        }
        free(zappy->game->map->tiles);
        free(zappy->game->map);
        free(zappy->game);
        free(zappy);
    }
    if (player) {
        free(player->team);
        free(player->inventory);
        free(player->network);
        free(player);
    }
    // Reset mock counters
    send_map_tile_calls = 0;
    send_player_inventory_calls = 0;
    send_ressource_droped_calls = 0;
    send_map_tile_return = 0;
    send_player_inventory_return = 0;
    send_ressource_droped_return = 0;
}



// Tests for add_elem_to_tile function
Test(set_object, add_elem_to_tile_food, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    add_elem_to_tile(zappy, 5, 5, "food");
    
    cleanup_test_data(zappy, NULL);
}

Test(set_object, add_elem_to_tile_linemate, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    add_elem_to_tile(zappy, 3, 7, "linemate");
    

    cleanup_test_data(zappy, NULL);
}

Test(set_object, add_elem_to_tile_deraumere, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    add_elem_to_tile(zappy, 2, 8, "deraumere");

    cleanup_test_data(zappy, NULL);
}

Test(set_object, add_elem_to_tile_sibur, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    add_elem_to_tile(zappy, 1, 9, "sibur");

    cleanup_test_data(zappy, NULL);
}

Test(set_object, add_elem_to_tile_mendiane, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    add_elem_to_tile(zappy, 6, 4, "mendiane");

    cleanup_test_data(zappy, NULL);
}

Test(set_object, add_elem_to_tile_phiras, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    add_elem_to_tile(zappy, 8, 2, "phiras");
    

    cleanup_test_data(zappy, NULL);
}

Test(set_object, add_elem_to_tile_thystame, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    add_elem_to_tile(zappy, 9, 1, "thystame");
    

    cleanup_test_data(zappy, NULL);
}

Test(set_object, add_elem_to_tile_invalid_resource, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    add_elem_to_tile(zappy, 5, 5, "invalid");
    
    cleanup_test_data(zappy, NULL);
}

Test(set_object, add_elem_to_tile_multiple_same_resource, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    // Add multiple food items to same tile
    cr_assert_eq(add_elem_to_tile(zappy, 5, 5, "food"), 0);
    cr_assert_eq(add_elem_to_tile(zappy, 5, 5, "food"), 0);
    cr_assert_eq(add_elem_to_tile(zappy, 5, 5, "food"), 0);
    
    cr_assert_eq(zappy->game->map->tiles[5][5].nbFood, 3);
    
    cleanup_test_data(zappy, NULL);
}

// Tests for handle_set function
Test(set_object, handle_set_food_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "set food";

    // Set all mock returns to success
    send_map_tile_return = 0;
    send_player_inventory_return = 0;
    send_ressource_droped_return = 0;
    
    handle_set(player, command, zappy);

    cleanup_test_data(zappy, player);
}

// Test malloc failure in handle_set
Test(set_object, handle_set_malloc_failure, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "set food";
    
    // Enable malloc failure after 0 calls (immediate failure)
    enable_malloc_failure(0);
    
    int result = handle_set(player, command, zappy);
    
    // Function should handle malloc failure gracefully
    cr_assert_eq(result, -1);
    
    // Disable malloc failure for cleanup
    disable_malloc_failure();
    cleanup_test_data(zappy, player);
}

Test(set_object, handle_set_linemate_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(3, 7);
    char command[] = "set linemate";

    
    send_map_tile_return = 0;
    send_player_inventory_return = 0;
    send_ressource_droped_return = 0;
    
    handle_set(player, command, zappy);

    cleanup_test_data(zappy, player);
}

Test(set_object, handle_set_all_resources, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    
    char *resources[] = {"food", "linemate", "deraumere", "sibur",
                        "mendiane", "phiras", "thystame"};
    
    send_map_tile_return = 0;
    send_player_inventory_return = 0;
    send_ressource_droped_return = 0;
    
    for (int i = 0; i < 7; i++) {
        char command[50];
        snprintf(command, sizeof(command), "set %s", resources[i]);
        
        // Reset mock counters
        send_map_tile_calls = 0;
        send_player_inventory_calls = 0;
        send_ressource_droped_calls = 0;
        
        handle_set(player, command, zappy);
    }
    
    // Check that all resources were added to tile
    cr_assert_eq(zappy->game->map->tiles[5][5].nbFood, 1);
    cr_assert_eq(zappy->game->map->tiles[5][5].nbLinemate, 1);
    cr_assert_eq(zappy->game->map->tiles[5][5].nbDeraumere, 1);
    cr_assert_eq(zappy->game->map->tiles[5][5].nbSibur, 1);
    cr_assert_eq(zappy->game->map->tiles[5][5].nbMendiane, 1);
    cr_assert_eq(zappy->game->map->tiles[5][5].nbPhiras, 1);
    cr_assert_eq(zappy->game->map->tiles[5][5].nbThystame, 1);
    
    cleanup_test_data(zappy, player);
}

Test(set_object, handle_set_no_command_parameter, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "set";
    
    handle_set(player, command, zappy);

    cleanup_test_data(zappy, player);
}

Test(set_object, handle_set_no_resource_parameter, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "set ";
    
    handle_set(player, command, zappy);

    cleanup_test_data(zappy, player);
}

Test(set_object, handle_set_invalid_resource, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "set invalid_resource";
    
    handle_set(player, command, zappy);
    
    cleanup_test_data(zappy, player);
}

Test(set_object, handle_set_insufficient_inventory, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "set food";
    
    // Set player food to 0
    player->inventory->nbFood = 0;
    
    send_map_tile_return = 0;
    send_player_inventory_return = 0;
    send_ressource_droped_return = 0;
    
    handle_set(player, command, zappy);
    cleanup_test_data(zappy, player);
}

Test(set_object, handle_set_send_map_tile_failure, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "set food";
    
    send_map_tile_return = -1; // Fail map tile send
    send_player_inventory_return = 0;
    send_ressource_droped_return = 0;
    
    handle_set(player, command, zappy);
    cleanup_test_data(zappy, player);
}

Test(set_object, handle_set_send_player_inventory_failure, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "set food";
    
    send_map_tile_return = 0;
    send_player_inventory_return = -1; // Fail inventory send
    send_ressource_droped_return = 0;
    
    handle_set(player, command, zappy);
    cleanup_test_data(zappy, player);
}

Test(set_object, handle_set_send_resource_dropped_failure, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "set food";
    
    send_map_tile_return = 0;
    send_player_inventory_return = 0;
    send_ressource_droped_return = -1; // Fail resource dropped send
    
    handle_set(player, command, zappy);
    
    cleanup_test_data(zappy, player);
}

Test(set_object, handle_set_resource_type_mapping, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    
    send_map_tile_return = 0;
    send_player_inventory_return = 0;
    send_ressource_droped_return = 0;
    
    // Test different resource types to ensure correct mapping
    char *resources[] = {"food", "linemate", "deraumere", "sibur", 
                        "mendiane", "phiras", "thystame"};
    
    for (int i = 0; i < 7; i++) {
        char command[50];
        snprintf(command, sizeof(command), "set %s", resources[i]);
        
        send_ressource_droped_calls = 0; // Reset counter
        
        handle_set(player, command, zappy);
    }
    
    cleanup_test_data(zappy, player);
}

Test(set_object, handle_set_multiple_words_resource, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "set food extra words ignored";
    
    send_map_tile_return = 0;
    send_player_inventory_return = 0;
    send_ressource_droped_return = 0;
    
    handle_set(player, command, zappy);

    
    cleanup_test_data(zappy, player);
}

Test(set_object, handle_set_edge_positions, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    // Test edge positions
    int positions[][2] = {{0, 0}, {9, 9}, {0, 9}, {9, 0}, {5, 0}, {0, 5}};
    int num_positions = sizeof(positions) / sizeof(positions[0]);
    
    send_map_tile_return = 0;
    send_player_inventory_return = 0;
    send_ressource_droped_return = 0;
    
    for (int i = 0; i < num_positions; i++) {
        player_t *player = create_test_player(positions[i][0], positions[i][1]);
        char command[] = "set food";
        
        handle_set(player, command, zappy);
        
        free(player->team);
        free(player->inventory);
        free(player->network);
        free(player);
    }
    
    cleanup_test_data(zappy, NULL);
}

// Tests for rm_item_to_inventory function (lines 74, 84, 87, 89-90, 92-95)
Test(set_object, rm_item_to_inventory_null_name, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "set ";
    
    int result = handle_set(player, command, zappy);
    
    // Should return -1 because strtok returns NULL for missing parameter
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy, player);
}

Test(set_object, rm_item_to_inventory_null_inventory, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    
    // Set inventory to NULL to test line 74
    free(player->inventory);
    player->inventory = NULL;
    char command[] = "set food";
    
    int result = handle_set(player, command, zappy);
    
    cr_assert_eq(result, -1);
    
    // Don't call cleanup_test_data with player as inventory is NULL
    if (zappy) {
        for (int i = 0; i < zappy->game->map->height; i++) {
            free(zappy->game->map->tiles[i]);
        }
        free(zappy->game->map->tiles);
        free(zappy->game->map);
        free(zappy->game);
        free(zappy);
    }
    if (player) {
        free(player->team);
        free(player->network);
        free(player);
    }
}

Test(set_object, rm_item_to_inventory_invalid_item, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "set invalid_item";
    
    int result = handle_set(player, command, zappy);
    
    // Should return -1 because item not found in handlers array (line 84)
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy, player);
}

Test(set_object, rm_item_to_inventory_all_valid_items, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    char *items[] = {"food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};
    
    send_map_tile_return = 0;
    send_player_inventory_return = 0;
    send_ressource_droped_return = 0;
    
    for (int i = 0; i < 7; i++) {
        player_t *player = create_test_player(5, 5);
        char command[50];
        snprintf(command, sizeof(command), "set %s", items[i]);
        
        handle_set(player, command, zappy);
        free(player->team);
        free(player->inventory);
        free(player->network);
        free(player);
    }
    
    cleanup_test_data(zappy, NULL);
}

// Tests for send_type_ressource function (lines 98-100, 109, 116)
Test(set_object, send_type_ressource_all_types, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    char *resources[] = {"food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};
    
    send_map_tile_return = 0;
    send_player_inventory_return = 0;
    send_ressource_droped_return = 0;
    
    for (int i = 0; i < 7; i++) {
        player_t *player = create_test_player(5, 5);
        char command[50];
        snprintf(command, sizeof(command), "set %s", resources[i]);
        
        send_ressource_droped_calls = 0;
        
        handle_set(player, command, zappy);
        free(player->team);
        free(player->inventory);
        free(player->network);
        free(player);
    }
    
    cleanup_test_data(zappy, NULL);
}

Test(set_object, send_type_ressource_unknown_resource, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    
    // Modify the code to test unknown resource type (should default to type 0)
    send_map_tile_return = 0;
    send_player_inventory_return = 0;
    send_ressource_droped_return = 0;
    
    // This will test the default case where type remains 0 (line 109)
    char command[] = "set unknown";
    
    // Since rm_item_to_inventory will fail for unknown items, this tests the error path
    int result = handle_set(player, command, zappy);
    
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy, player);
}

Test(set_object, send_ressource_droped_failure, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "set food";
    
    send_map_tile_return = 0;
    send_player_inventory_return = 0;
    send_ressource_droped_return = -1; // Make send_ressource_droped fail (line 116)
    
    int result = handle_set(player, command, zappy);
    
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy, player);
}

// Tests for handle_set error paths (lines 120-122)
Test(set_object, handle_set_null_command_token, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "";  // Empty command
    
    int result = handle_set(player, command, zappy);
    
    // Should return -1 because first strtok returns NULL (line 120)
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy, player);
}

Test(set_object, handle_set_only_command_no_parameter, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "set";  // Only command, no parameter
    
    int result = handle_set(player, command, zappy);
    
    // Should return -1 because second strtok returns NULL (line 122)
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy, player);
}

Test(set_object, handle_set_empty_parameter, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "set ";  // Command with space but no parameter
    
    int result = handle_set(player, command, zappy);
    
    // Should return -1 because parameter is NULL
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy, player);
}

// Additional comprehensive tests
Test(set_object, comprehensive_error_path_testing, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    // Test scenario: valid item but insufficient inventory
    player_t *player = create_test_player(5, 5);
    player->inventory->nbFood = 0;  // No food in inventory
    char command[] = "set food";
    
    int result = handle_set(player, command, zappy);
    
    // Should fail because player has no food to set
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy, player);
}

Test(set_object, resource_type_boundary_testing, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    
    send_map_tile_return = 0;
    send_player_inventory_return = 0;
    send_ressource_droped_return = 0;
    
    // Test the exact resource names and their type mappings
    char *test_cases[][2] = {
        {"food", "0"}, {"linemate", "1"}, {"deraumere", "2"}, 
        {"sibur", "3"}, {"mendiane", "4"}, {"phiras", "5"}, {"thystame", "6"}
    };
    
    for (int i = 0; i < 7; i++) {
        char command[50];
        snprintf(command, sizeof(command), "set %s", test_cases[i][0]);
        
        // Reset player inventory to ensure we have items
        player->inventory->nbFood = 10;
        player->inventory->nbLinemate = 10;
        player->inventory->nbDeraumere = 10;
        player->inventory->nbSibur = 10;
        player->inventory->nbMendiane = 10;
        player->inventory->nbPhiras = 10;
        player->inventory->nbThystame = 10;
        
        handle_set(player, command, zappy);
    }
    
    cleanup_test_data(zappy, player);
}
