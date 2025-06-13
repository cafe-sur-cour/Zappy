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
    char msg[19];

    if (handle_end_incantation(player, zappy) == 0) {
        snprintf(msg, 19, "Current level: %d\n", player->level);
        write_message(player->network->fd, msg);
    } else {
        write_message(player->network->fd, "ko\n");
    }
    if (player->current_action)
        free(player->current_action);
    player->current_action = NULL;
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
static void process_player_actions(player_t *player, zappy_t *zappy)
{
    action_request_t *action = NULL;

    if (!player->pending_actions)
        return;
    if (handle_cooldown(player, zappy) == 1)
        return;
    if (player->is_busy)
        return;
    pthread_mutex_lock(&player->pending_actions->mutex);
    action = dequeue_highest_priority_action(player->pending_actions);
    if (action) {
        execute_action(player, action, zappy);
        free_action_request(action);
    }
    pthread_mutex_unlock(&player->pending_actions->mutex);
}

/* This function allows the smart poll to have different timeout */
static int calculate_poll_timeout(player_t *player, int frequency)
{
    int base_timeout = 1000 / frequency;

    if (player->is_busy) {
        return base_timeout * 2;
    }
    if (player->pending_actions && player->pending_actions->count > 0) {
        return base_timeout / 2;
    }
    return base_timeout;
}

/* This function "polls" the message and the queue it */
static void poll_player_input(player_t *player, zappy_t *zappy)
{
    int timeout = 0;
    char *message = NULL;

    if (!player || !player->network || player->network->fd < 0)
        return;
    timeout = calculate_poll_timeout(player, zappy->params->freq);
    message = get_message(player->network->fd, timeout);
    if (message) {
        queue_action(player, message, zappy);
        free(message);
    }
}

/* This functions loops thrue all player in all team to handle cmd */
void smart_poll_players(zappy_t *zappy)
{
    team_t *team = NULL;
    player_t *player = NULL;

    if (!zappy || !zappy->game || !zappy->game->teams)
        return;
    team = zappy->game->teams;
    while (team) {
        player = team->players;
        while (player) {
            poll_player_input(player, zappy);
            process_player_actions(player, zappy);
            player = player->next;
        }
        team = team->next;
    }
}
