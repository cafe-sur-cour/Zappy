/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** player
*/

#include "zappy.h"
#include "game.h"
#include "network.h"

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

/* This function updates player food consumption based on time */
static void update_player_food(player_t *player, zappy_t *zappy)
{
    time_t current_time = time(NULL);
    double time_elapsed = difftime(current_time, player->last_food_check);
    int time_units_passed = (int)(time_elapsed * zappy->params->freq);

    if (time_units_passed > 0) {
        player->food_timer -= time_units_passed;
        player->last_food_check = current_time;
        while (player->food_timer <= 0 && player->inventory->nbFood > 0) {
            player->inventory->nbFood--;
            player->food_timer += 126;
            send_player_inventory(zappy, player);
        }
    }
}

/* This function checks if a player is dead from starvation */
static bool is_player_dead(player_t *player)
{
    return (player->food_timer <= 0 && player->inventory->nbFood <= 0);
}

/* This function handles player death */
static void handle_player_death(zappy_t *zappy, player_t *player, team_t *team)
{
    player_t *current = NULL;

    write_message(player->network->fd, "dead\n");
    send_player_death(zappy, player);
    if (team->players == player) {
        team->players = player->next;
    } else {
        current = team->players;
        while (current && current->next != player) {
            current = current->next;
        }
        if (current) {
            current->next = player->next;
        }
    }
    team->nbPlayerAlive--;
    free_player(player);
}

static void check_player_health_status(zappy_t *zappy,
    team_t *current, player_t *next_player)
{
    player_t *player = current->players;

    while (player != NULL) {
        next_player = player->next;
        verify_player_id(zappy, player);
        update_player_food(player, zappy);
        if (is_player_dead(player))
            handle_player_death(zappy, player, current);
        player = next_player;
    }
}

/* Loop thru the player to check health and connection updates */
void check_player_status(zappy_t *zappy)
{
    team_t *current = NULL;
    player_t *next_player = NULL;

    for (current = zappy->game->teams; current != NULL;
        current = current->next) {
        check_player_health_status(zappy, current, next_player);
    }
}
