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

static bool valid_team_name(const char *team_name, server_t *server)
{
    for (int i = 0; i < server->params->nb_team; i++) {
        if (strcmp(team_name, server->params->teams[i]) == 0) {
            return true;
        }
    }
    error_message("Invalid team name provided.");
    return false;
}

bool process_new_client(const char *team_name, int fd, server_t *server)
{
    if (strcmp(team_name, "GRAPHIC\n") == 0) {
        if (server->graph->fd != -1) {
            error_message("A graphic client is already connected.");
            return false;
        }
        server->graph->fd = fd;
        printfd("GRAPHIC client connected.\n", fd);
        return true;
    }
    return valid_team_name(team_name, server);
}

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

static lives_t *init_lives(int freq)
{
    lives_t *lives = malloc(sizeof(lives_t));

    if (!lives) {
        error_message("Failed to allocate memory for player lives.");
        return NULL;
    }
    lives->freq = freq;
    lives->nbFood = 0;
    lives->startRefresh = time(NULL);
    lives->endRefresh = time(NULL);
    return lives;
}

static player_t *init_player(int fd, int freq, tiles_t tile)
{
    player_t *player = malloc(sizeof(player_t));

    if (!player) {
        error_message("Failed to allocate memory for new player.");
        return NULL;
    }
    player->id = fd;
    player->level = 1;
    player->posX = tile.x;
    player->posY = tile.y;
    player->isAlive = true;
    player->direction = rand() % 4;
    player->inventory = init_inventory();
    if (!player->inventory)
        return free_player(player);
    player->lives = init_lives(freq);
    if (!player->lives)
        return free_player(player);
    return player;
}

static int check_team_capacity(server_t *server, const char *team_name,
    player_t *new_player)
{
    while (server->game->teams) {
        if (strcmp(server->game->teams->name, team_name) == 0 &&
            server->game->teams->nbPlayers < server->params->nb_client) {
            new_player->next = server->game->teams->players;
            server->game->teams->players = new_player;
            server->game->teams->nbPlayers++;
            server->game->teams->nbPlayerAlive++;
            printf("New player added to team %s.\n", team_name);
            return server->params->nb_client - server->game->teams->nbPlayers;
        }
        server->game->teams = server->game->teams->next;
    }
    return -1;
}

team_t *add_client_to_team(const char *team_name, int fd, server_t *server)
{
    tiles_t *tiles = shuffle_fisher(server->game->width, server->game->heigt);
    player_t *new_player = init_player(fd, server->params->freq, tiles[0]);
    team_t *save = server->game->teams;
    team_t *result = NULL;

    free(tiles);
    if (!new_player) {
        close(fd);
        return NULL;
    }
    if (check_team_capacity(server, team_name, new_player) == -1) {
        error_message("Requested team is full.");
        server->game->teams = save;
        return NULL;
    }
    result = server->game->teams;
    server->game->teams = save;
    return result;
}
