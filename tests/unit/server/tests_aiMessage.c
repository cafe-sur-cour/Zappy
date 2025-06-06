/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_aiMessage
*/

#include "zappy.h"
#include "game.h"
#include "network.h"

#include <criterion/criterion.h>
#include <criterion/redirect.h>


static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(Testforward_messages, test_forward_message_north, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.y = 10;
    player.posX = 5;
    player.posY = 5;
    player.direction = NORTH;
    
    int result = forward_message(&player, &params);
    
    cr_assert_eq(result, 0, "Forward message should return 0");
    cr_assert_eq(player.posY, 4, "Player should move north (y-1)");
    cr_assert_eq(player.posX, 5, "Player X position should not change");
}

Test(Testforward_messages, test_forward_message_south, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.y = 10;
    player.posX = 5;
    player.posY = 5;
    player.direction = SOUTH;
    
    int result = forward_message(&player, &params);
    
    cr_assert_eq(result, 0, "Forward message should return 0");
    cr_assert_eq(player.posY, 6, "Player should move south (y+1)");
    cr_assert_eq(player.posX, 5, "Player X position should not change");
}

Test(Testforward_messages, test_forward_message_east, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.x = 10;
    player.posX = 5;
    player.posY = 5;
    player.direction = EAST;
    
    int result = forward_message(&player, &params);
    
    cr_assert_eq(result, 0, "Forward message should return 0");
    
}

Test(Testforward_messages, test_forward_message_west, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.x = 10;
    player.posX = 5;
    player.posY = 5;
    player.direction = WEST;
    
    int result = forward_message(&player, &params);
    
    cr_assert_eq(result, 0, "Forward message should return 0");
    cr_assert_eq(player.posX, 4, "Player should move west (x-1)");
    cr_assert_eq(player.posY, 5, "Player Y position should not change");
}

Test(Testforward_messages, test_forward_message_wrapping_north, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.y = 10;
    player.posX = 5;
    player.posY = 0;  // At top edge
    player.direction = NORTH;
    
    forward_message(&player, &params);
    
    cr_assert_eq(player.posY, 9, "Player should wrap to bottom when moving north from y=0");
}

Test(Testforward_messages, test_forward_message_wrapping_south, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.y = 10;
    player.posX = 5;
    player.posY = 9;  // At bottom edge
    player.direction = SOUTH;
    
    forward_message(&player, &params);
    
    cr_assert_eq(player.posY, 0, "Player should wrap to top when moving south from y=9");
}

Test(Testforward_messages, test_forward_message_wrapping_west, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.x = 10;
    player.posX = 0;  // At left edge
    player.posY = 5;
    player.direction = WEST;
    
    forward_message(&player, &params);
}

Test(Testforward_messages, test_forward_message_wrapping_east, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.x = 10;
    player.posX = 9;  // At right edge
    player.posY = 5;
    player.direction = EAST;
    
    forward_message(&player, &params);
    
    cr_assert_eq(player.posX, 0, "Player should wrap to left when moving east from x=9");
}


Test(Testforward_messages, test_forward_invalid_direction, .init = redirect_all_std)
{
    player_t player = {0};
    params_t params = {0};
    
    params.x = 10;
    params.y = 10;
    player.posX = 5;
    player.posY = 5;
    player.direction = 999;  // Invalid direction
    
    int result = forward_message(&player, &params);
    
    // Should handle invalid direction gracefully
    cr_assert_eq(result, 0, "Forward should handle invalid direction");
    cr_assert_eq(player.posX, 5, "Player X should not change with invalid direction");
    cr_assert_eq(player.posY, 5, "Player Y should not change with invalid direction");
}

