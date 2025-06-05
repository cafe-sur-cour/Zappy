/*
** EPITECH PROJECT, 2025
** Zapppy
** File description:
** Server :: Client
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "zappy.h"
#include "algo.h"

/* This function verify the the current team name is an existing one */
static bool valid_team_name(const char *team_name, zappy_t *zappy)
{
    for (int i = 0; i < zappy->params->nb_team; i++) {
        if (strcmp(team_name, zappy->params->teams[i]) == 0) {
            return true;
        }
    }
    error_message("Invalid team name provided.");
    return false;
}

/* This function check if the name is graphic if not it verify the team name */
bool process_new_client(const char *team_name, int fd, zappy_t *zappy)
{
    if (strcmp(team_name, "GRAPHIC") == 0) {
        if (zappy->graph->fd != -1) {
            error_message("A graphic client is already connected.");
            return false;
        }
        zappy->graph->fd = fd;
        return true;
    }
    return valid_team_name(team_name, zappy);
}

/* This function initialize the inventory struct of the current player */
static inventory_t *init_inventory(void)
{
    inventory_t *inventory = malloc(sizeof(inventory_t));

    if (!inventory) {
        error_message("Failed to allocate memory for player inventory.");
        return NULL;
    }
    inventory->nbLinemate = 0;
    inventory->nbDeraumere = 0;
    inventory->nbSibur = 0;
    inventory->nbMendiane = 0;
    inventory->nbPhiras = 0;
    inventory->nbThystame = 0;
    return inventory;
}

/* This function initialize the current player structure */
static player_t *malloc_player(player_t *player)
{
    player->id = -1;
    player->network = malloc(sizeof(network_t));
    if (!player->network) {
        error_message("Failed to allocate memory for player network.");
        free(player);
        return NULL;
    }
    player->network->buffer = malloc(sizeof(buffer_t));
    if (!player->network->buffer) {
        error_message("Failed to allocate memory for player buffer.");
        free(player->network);
        free(player);
        return NULL;
    }
    return player;
}

/* This loops thrue the eggs list to atribute a pos to the player */
static player_t *set_player_pos(player_t *player, zappy_t *zappy)
{
    egg_t *current_egg = NULL;

    if (zappy->game->map->currentEggs == NULL)
        return player;
    current_egg = zappy->game->map->currentEggs;
    while (current_egg != NULL){
        if (current_egg->isHatched == false) {
            player->posX = current_egg->posX;
            player->posY = current_egg->posY;
            current_egg->isHatched = true;
            break;
        }
        current_egg = current_egg->next;
    }
    return player;
}

/* This function initialize the current player structure */
static player_t *init_player(int fd, zappy_t *zappy)
{
    player_t *player = malloc(sizeof(player_t));

    if (!player) {
        error_message("Failed to allocate memory for new player.");
        return NULL;
    }
    player = malloc_player(player);
    if (!player)
        return NULL;
    player->network->fd = fd;
    player->level = 1;
    player->direction = rand() % 4;
    player->inventory = init_inventory();
    if (!player->inventory)
        return free_player(player);
    player = set_player_pos(player, zappy);
    return player;
}

/* This function verify that the team the client wants to join as space */
static int check_team_capacity(zappy_t *server, const char *team_name,
    player_t *new_player)
{
    while (server->game->teams) {
        if (strcmp(server->game->teams->name, team_name) == 0 &&
            server->game->teams->nbPlayers < server->params->nb_client) {
            new_player->next = server->game->teams->players;
            server->game->teams->players = new_player;
            server->game->teams->nbPlayers++;
            server->game->teams->nbPlayerAlive++;
            new_player->id = server->game->teams->nbPlayers;
            return server->params->nb_client - server->game->teams->nbPlayers;
        }
        server->game->teams = server->game->teams->next;
    }
    return -1;
}

static team_t *add_client_team_rest(zappy_t *server, team_t *save,
    const char *team_name, player_t *new_player)
{
    if (strcmp(team_name, "GRAPHIC") == 0) {
        free_player(new_player);
        return NULL;
    }
    if (check_team_capacity(server, team_name, new_player) == -1) {
        server->game->teams = save;
        free_player(new_player);
        return NULL;
    }
    new_player->team = strdup(team_name);
    if (!new_player->team) {
        error_message("Failed to allocate memory for player team name.");
        free_player(new_player);
        return NULL;
    }
    return server->game->teams;
}

team_t *add_client_to_team(const char *team_name, int fd, zappy_t *server)
{
    player_t *new_player = init_player(fd, server);
    team_t *save = server->game->teams;
    team_t *result = NULL;

    if (!new_player) {
        close(fd);
        return NULL;
    }
    if (!add_client_team_rest(server, save, team_name, new_player)) {
        return NULL;
    }
    result = server->game->teams;
    server->game->teams = save;
    return result;
}
