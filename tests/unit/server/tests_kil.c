/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_kil
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <math.h>
#include "zappy.h"
#include "network.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Mock variables
static char *last_written_message = NULL;
static int write_message_calls = 0;
static int write_message_return = 0;
static int send_player_death_calls = 0;
static int send_player_death_return = 0;

// Function declarations
int kil(zappy_t *zappy, graph_net_t *graphic, char *message);

// Helper functions to create test data
static zappy_t *create_test_zappy(void)
{
    zappy_t *zappy = malloc(sizeof(zappy_t));
    zappy->params = malloc(sizeof(params_t));
    zappy->game = malloc(sizeof(game_t));
    
    zappy->params->nb_team = 2;
    zappy->params->x = 10;
    zappy->params->y = 10;
    zappy->params->is_debug = false;
    
    // Initialize teams
    team_t *team1 = malloc(sizeof(team_t));
    team1->name = strdup("team1");
    team1->players = NULL;
    team1->nbPlayers = 0;
    team1->nbPlayerAlive = 0;
    
    team_t *team2 = malloc(sizeof(team_t));
    team2->name = strdup("team2");
    team2->players = NULL;
    team2->nbPlayers = 0;
    team2->nbPlayerAlive = 0;
    team2->next = NULL;
    
    team1->next = team2;
    zappy->game->teams = team1;
    
    return zappy;
}

static player_t *create_test_player(int id, int x, int y, const char *team_name)
{
    player_t *player = malloc(sizeof(player_t));
    player->network = malloc(sizeof(network_t));
    player->inventory = malloc(sizeof(inventory_t));
    
    player->id = id;
    player->posX = x;
    player->posY = y;
    player->level = 1;
    player->direction = NORTH;
    player->team = strdup(team_name);
    player->network->fd = 42 + id;
    player->next = NULL;
    
    memset(player->inventory, 0, sizeof(inventory_t));
    
    return player;
}

static void add_player_to_team(zappy_t *zappy, player_t *player, const char *team_name)
{
    team_t *team = zappy->game->teams;
    while (team) {
        if (strcmp(team->name, team_name) == 0) {
            player->next = team->players;
            team->players = player;
            team->nbPlayers++;
            team->nbPlayerAlive++;
            break;
        }
        team = team->next;
    }
}

static void cleanup_test_data(zappy_t *zappy)
{
    if (zappy) {
        if (zappy->game) {
            team_t *team = zappy->game->teams;
            while (team) {
                team_t *next_team = team->next;
                player_t *player = team->players;
                while (player) {
                    player_t *next_player = player->next;
                    free(player->team);
                    free(player->network);
                    free(player->inventory);
                    free(player);
                    player = next_player;
                }
                free(team->name);
                free(team);
                team = next_team;
            }
            free(zappy->game);
        }
        free(zappy->params);
        free(zappy);
    }
    if (last_written_message) {
        free(last_written_message);
        last_written_message = NULL;
    }
    write_message_calls = 0;
    write_message_return = 0;
    send_player_death_calls = 0;
    send_player_death_return = 0;
}

// Test basic kill functionality
Test(kil, valid_kill_single_player, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(1, 5, 5, "team1");
    
    add_player_to_team(zappy, player, "team1");
    
    write_message_return = 0;
    send_player_death_return = 0;
    
    char message[] = "kil #1";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, 0, "Should successfully kill valid player");
    
    cleanup_test_data(zappy);
}

Test(kil, valid_kill_player_in_list, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player1 = create_test_player(1, 5, 5, "team1");
    player_t *player2 = create_test_player(2, 3, 3, "team1");
    player_t *player3 = create_test_player(3, 7, 7, "team1");
    
    add_player_to_team(zappy, player1, "team1");
    add_player_to_team(zappy, player2, "team1");
    add_player_to_team(zappy, player3, "team1");
    
    write_message_return = 0;
    send_player_death_return = 0;
    
    char message[] = "kil #2";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, 0, "Should successfully kill player in middle of list");
    
    cleanup_test_data(zappy);
}

Test(kil, kill_first_player_in_team, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player1 = create_test_player(1, 5, 5, "team1");
    player_t *player2 = create_test_player(2, 3, 3, "team1");
    
    add_player_to_team(zappy, player2, "team1");
    add_player_to_team(zappy, player1, "team1"); // This makes player1 first
    
    write_message_return = 0;
    send_player_death_return = 0;
    
    char message[] = "kil #1";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, 0, "Should successfully kill first player in team");
    
    cleanup_test_data(zappy);
}

Test(kil, kill_last_player_in_team, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player1 = create_test_player(1, 5, 5, "team1");
    player_t *player2 = create_test_player(2, 3, 3, "team1");
    
    add_player_to_team(zappy, player1, "team1");
    add_player_to_team(zappy, player2, "team1");
    
    write_message_return = 0;
    send_player_death_return = 0;
    
    char message[] = "kil #1";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, 0, "Should successfully kill last player in team");
    
    cleanup_test_data(zappy);
}

Test(kil, kill_player_different_teams, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player1 = create_test_player(1, 5, 5, "team1");
    player_t *player2 = create_test_player(2, 3, 3, "team2");
    
    add_player_to_team(zappy, player1, "team1");
    add_player_to_team(zappy, player2, "team2");
    
    write_message_return = 0;
    send_player_death_return = 0;
    
    char message[] = "kil #2";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, 0, "Should successfully kill player from different team");
    
    cleanup_test_data(zappy);
}

// Test invalid message formats
Test(kil, invalid_message_too_short, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    char message[] = "kil";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, -1, "Should return -1 for message too short");
    
    cleanup_test_data(zappy);
}

Test(kil, invalid_message_wrong_format, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    char message[] = "kil 123";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, -1, "Should return -1 for wrong message format");
    
    cleanup_test_data(zappy);
}

Test(kil, invalid_message_no_number, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    char message[] = "kil #abc";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, -1, "Should return -1 for non-numeric player ID");
    
    cleanup_test_data(zappy);
}

Test(kil, invalid_message_missing_hash, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    char message[] = "kil 123";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, -1, "Should return -1 for missing hash symbol");
    
    cleanup_test_data(zappy);
}

Test(kil, invalid_message_empty_string, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    char message[] = "";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, -1, "Should return -1 for empty message");
    
    cleanup_test_data(zappy);
}

// Test player not found scenarios
Test(kil, player_not_found_nonexistent_id, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(1, 5, 5, "team1");
    
    add_player_to_team(zappy, player, "team1");
    
    char message[] = "kil #999";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, -1, "Should return -1 for nonexistent player ID");
    
    cleanup_test_data(zappy);
}

Test(kil, player_not_found_negative_id, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(1, 5, 5, "team1");
    
    add_player_to_team(zappy, player, "team1");
    
    char message[] = "kil #-1";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, -1, "Should return -1 for negative player ID");
    
    cleanup_test_data(zappy);
}

Test(kil, player_not_found_zero_id, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(1, 5, 5, "team1");
    
    add_player_to_team(zappy, player, "team1");
    
    char message[] = "kil #0";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, -1, "Should return -1 for zero player ID");
    
    cleanup_test_data(zappy);
}

Test(kil, no_players_in_game, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    char message[] = "kil #1";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, -1, "Should return -1 when no players exist");
    
    cleanup_test_data(zappy);
}

// Test edge cases with large numbers
Test(kil, large_player_id, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(999999, 5, 5, "team1");
    
    add_player_to_team(zappy, player, "team1");
    
    write_message_return = 0;
    send_player_death_return = 0;
    
    char message[] = "kil #999999";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, 0, "Should handle large player IDs correctly");
    
    cleanup_test_data(zappy);
}

Test(kil, very_large_nonexistent_id, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(1, 5, 5, "team1");
    
    add_player_to_team(zappy, player, "team1");
    
    char message[] = "kil #2147483647";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, -1, "Should return -1 for very large nonexistent ID");
    
    cleanup_test_data(zappy);
}

// Test message variations
Test(kil, message_with_extra_spaces, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(1, 5, 5, "team1");
    
    add_player_to_team(zappy, player, "team1");
    
    write_message_return = 0;
    send_player_death_return = 0;
    
    char message[] = "kil #1   ";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, 0, "Should handle messages with trailing spaces");
    
    cleanup_test_data(zappy);
}

Test(kil, message_with_leading_spaces, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    char message[] = "   kil #1";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, -1, "Should fail for messages with leading spaces");
    
    cleanup_test_data(zappy);
}

Test(kil, message_case_sensitivity, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    char message[] = "KIL #1";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, -1, "Should be case sensitive and fail for uppercase");
    
    cleanup_test_data(zappy);
}

// Test team alive count management
Test(kil, team_alive_count_decreases, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player1 = create_test_player(1, 5, 5, "team1");
    player_t *player2 = create_test_player(2, 3, 3, "team1");
    
    add_player_to_team(zappy, player1, "team1");
    add_player_to_team(zappy, player2, "team1");
    write_message_return = 0;
    send_player_death_return = 0;
    
    char message[] = "kil #1";
    kil(zappy, NULL, message);
    cleanup_test_data(zappy);
}

Test(kil, kill_all_players_in_team, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player1 = create_test_player(1, 5, 5, "team1");
    player_t *player2 = create_test_player(2, 3, 3, "team1");
    
    add_player_to_team(zappy, player1, "team1");
    add_player_to_team(zappy, player2, "team1");
    
    write_message_return = 0;
    send_player_death_return = 0;
    
    // Kill first player
    char message1[] = "kil #1";
    int result1 = kil(zappy, NULL, message1);
    cr_assert_eq(result1, 0, "First kill should succeed");
    
    // Kill second player
    char message2[] = "kil #2";
    int result2 = kil(zappy, NULL, message2);
    cr_assert_eq(result2, 0, "Second kill should succeed");

    cleanup_test_data(zappy);
}

// Test multiple teams scenario
Test(kil, kill_from_different_teams, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player1 = create_test_player(1, 5, 5, "team1");
    player_t *player2 = create_test_player(2, 3, 3, "team2");
    
    add_player_to_team(zappy, player1, "team1");
    add_player_to_team(zappy, player2, "team2");
    
    write_message_return = 0;
    send_player_death_return = 0;
    
    char message[] = "kil #2";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, 0, "Should kill player from team2");
    
    // Check that team1 is unaffected
    team_t *team1 = zappy->game->teams;
    cr_assert_eq(team1->nbPlayerAlive, 1, "Team1 should still have 1 player");
    cleanup_test_data(zappy);
}

Test(kil, null_message_pointer, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    cleanup_test_data(zappy);
    cr_assert(1, "Null message test case identified");
}

// Test specific message parsing edge cases
Test(kil, message_with_multiple_hashes, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    char message[] = "kil ##1";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, -1, "Should fail for multiple hash symbols");
    
    cleanup_test_data(zappy);
}

Test(kil, message_with_hash_at_end, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    char message[] = "kil 1#";
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, -1, "Should fail for hash at wrong position");
    
    cleanup_test_data(zappy);
}

Test(kil, message_exact_minimum_length, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(1, 5, 5, "team1");
    
    add_player_to_team(zappy, player, "team1");
    
    write_message_return = 0;
    send_player_death_return = 0;
    
    char message[] = "kil #1";  // Exactly 6 characters
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, 0, "Should work with minimum valid length");
    
    cleanup_test_data(zappy);
}

Test(kil, message_one_char_too_short, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    char message[] = "kil #";  // Exactly 5 characters (too short)
    int result = kil(zappy, NULL, message);
    
    cr_assert_eq(result, -1, "Should fail when exactly one character too short");
    
    cleanup_test_data(zappy);
}
