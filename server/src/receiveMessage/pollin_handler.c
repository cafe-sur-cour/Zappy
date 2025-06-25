/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** pollin_handler
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* This functions free the memory of the action */
void free_action_request(action_request_t *action)
{
    if (action) {
        free(action->command);
        free(action);
    }
}

static void write_end_incantation(player_t *player, zappy_t *zappy)
{
    if (handle_end_incantation(player, zappy) != 0) {
        write_in_buffer(player->network->writingBuffer, "ko\n");
        write_message(player->network);
    }
}

static int handle_cooldown(player_t *player, zappy_t *zappy)
{
    if (player->remaining_cooldown > 0) {
        player->remaining_cooldown--;
        if (player->remaining_cooldown == 0 && player->current_action != NULL
            && strcmp(player->current_action, "Incantation") == 0) {
            write_end_incantation(player, zappy);
        }
        if (player->remaining_cooldown == 0 && player->current_action != NULL
            && strcmp(player->current_action, "Fork") == 0) {
            handle_fork_end(player, zappy);
        }
        if (player->remaining_cooldown == 0 && player->is_busy)
            player->is_busy = false;
        return 1;
    }
    return 0;
}

/* This function defines wether the player is occupied or not */
void process_player_actions(player_t *player, zappy_t *zappy)
{
    action_request_t *action = NULL;

    if (!player->pending_actions)
        return;
    if (handle_cooldown(player, zappy) == 1)
        return;
    if (player->is_busy)
        return;
    action = dequeue_highest_priority_action(player->pending_actions);
    if (action) {
        execute_action(player, action, zappy);
        free_action_request(action);
    }
}

/* This function processes player actions without polling */
void process_player_actions_tick(zappy_t *zappy)
{
    team_t *team = NULL;
    player_t *player = NULL;

    if (!zappy || !zappy->game || !zappy->game->teams)
        return;
    team = zappy->game->teams;
    while (team) {
        player = team->players;
        while (player) {
            process_player_actions(player, zappy);
            player = player->next;
        }
        team = team->next;
    }
}
