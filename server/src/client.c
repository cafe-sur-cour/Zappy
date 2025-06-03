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

static graph_t *init_graph(void)
{
    graph_t *graph = malloc(sizeof(graph_t));

    if (!graph) {
        error_message("Failed to allocate memory for graph.");
        exit(84);
    }
    graph->fd = -1;
    return graph;
}

bool process_new_client(const char *team_name, int fd, server_t *server)
{
    if (strcmp(team_name, "GRAPHIC") == 0) {
        server->graph = init_graph();
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

static player_t *init_player(int fd, int freq)
{
    player_t *player = malloc(sizeof(player_t));

    if (!player) {
        error_message("Failed to allocate memory for new player.");
        return NULL;
    }
    player->id = fd;
    player->level = 1;
    player->posX = 0;
    player->posY = 0;
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

int add_client_to_team(const char *team_name, int fd, server_t *server)
{
    player_t *new_player = init_player(fd, server->params->freq);
    team_t *save = server->game->teams;

    if (!new_player) {
        close(fd);
        return -1;
    }
    while (server->game->teams) {
        if (strcmp(server->game->teams->name, team_name) == 0 &&
            server->game->teams->nbPlayers < server->params->nb_client) {
            new_player->next = server->game->teams->players;
            server->game->teams->players = new_player;
            server->game->teams->nbPlayers++;
            server->game->teams->nbPlayerAlive++;
            printf("New player added to team %s.\n", team_name);
            return 0;
        }
        server->game->teams = server->game->teams->next;
    }
    printf("Team %s is full or does not exist.\n", team_name);
    server->game->teams = save;
    return 0;
}
