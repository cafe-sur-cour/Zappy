/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** player
*/

#include "zappy.h"
#include "game.h"

#include <stdlib.h>
#include <string.h>

/* This sub-function check the current id agains the other */
static int loop_thru_players(player_t *current_player, int highest_id)
{
    while (current_player != NULL) {
        if (current_player->id > highest_id)
            highest_id = current_player->id;
        current_player = current_player->next;
    }
    return highest_id;
}

/* This function returns the highest free id for a player */
int get_next_free_id(zappy_t *server)
{
    int highest_id = 0;
    team_t *current_team = server->game->teams;
    player_t *current_player = NULL;

    while (current_team != NULL) {
        current_player = current_team->players;
        highest_id = loop_thru_players(current_player, highest_id);
        current_team = current_team->next;
    }
    return highest_id + 1;
}

/* This function call the next id to get player id */
static void verify_player_id(zappy_t *zappy, player_t *player)
{
    if (player->id == -1) {
        player->id = get_next_free_id(zappy);
        send_player_connect(zappy, player);
    }
}

/* Loop thru the player to see connection updates */
void check_player_status(zappy_t *zappy)
{
    team_t *current = NULL;
    player_t *player = NULL;

    for (current = zappy->game->teams; current != NULL;
        current = current->next) {
        player = current->players;
        while (player != NULL) {
            verify_player_id(zappy, player);
            player = player->next;
        }
    }
}

static player_t *loop_tru_players(team_t *team, int player_id)
{
    player_t *current_player = team->players;

    while (current_player) {
        if (current_player->id == player_id) {
            return current_player;
        }
        current_player = current_player->next;
    }
    return NULL;
}

player_t *get_player_by_id(game_t *game, int player_id)
{
    team_t *current_team = game->teams;
    player_t *result = NULL;

    while (current_team) {
        result = loop_tru_players(current_team, player_id);
        if (result)
            return result;
        current_team = current_team->next;
    }
    error_message("Player not found.");
    return NULL;
}
