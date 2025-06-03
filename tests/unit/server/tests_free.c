/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_free
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <unistd.h>
#include "zappy.h"
#include "algo.h"

// Test free_params with NULL
Test(free_params, test_null_params)
{
    void *result = free_params(NULL);
    cr_assert_null(result);
}

// Test free_params with valid params but no teams
Test(free_params, test_params_no_teams)
{
    params_t *params = malloc(sizeof(params_t));
    params->teams = NULL;
    params->nb_team = 0;
    
    void *result = free_params(params);
    cr_assert_null(result);
}

// Test free_params with valid params and teams
Test(free_params, test_params_with_teams)
{
    params_t *params = malloc(sizeof(params_t));
    params->nb_team = 2;
    params->teams = malloc(sizeof(char*) * 2);
    params->teams[0] = malloc(10);
    params->teams[1] = malloc(10);
    
    void *result = free_params(params);
    cr_assert_null(result);
}

// Test free_server with NULL
Test(free_server, test_null_server)
{
    void *result = free_server(NULL);
    cr_assert_null(result);
}

// Test free_server with minimal server
Test(free_server, test_minimal_server)
{
    server_t *server = malloc(sizeof(server_t));
    server->params = NULL;
    server->sockfd = -1;
    server->map = NULL;
    server->graph = NULL;
    
    void *result = free_server(server);
    cr_assert_null(result);
}

// Test free_server with valid socket
Test(free_server, test_server_with_socket)
{
    server_t *server = malloc(sizeof(server_t));
    server->params = NULL;
    server->sockfd = 1; // Valid fd
    server->map = NULL;
    server->graph = NULL;
    
    void *result = free_server(server);
    cr_assert_null(result);
}

// Test free_server with all components
Test(free_server, test_complete_server)
{
    server_t *server = malloc(sizeof(server_t));
    
    // Create params
    server->params = malloc(sizeof(params_t));
    server->params->teams = NULL;
    server->params->nb_team = 0;
    
    // Create map
    server->map = malloc(sizeof(map_t));
    server->map->teams = NULL;
    server->map->ressources = NULL;
    
    // Create graph
    server->graph = malloc(sizeof(graph_t));
    
    server->sockfd = -1;
    
    void *result = free_server(server);
    cr_assert_null(result);
}

// Test map with teams
Test(free_server, test_server_with_teams)
{
    server_t *server = malloc(sizeof(server_t));
    server->params = NULL;
    server->sockfd = -1;
    server->graph = NULL;
    
    // Create map with teams
    server->map = malloc(sizeof(map_t));
    server->map->ressources = NULL;
    server->map->teams = malloc(sizeof(team_t));
    server->map->teams->name = malloc(10);
    server->map->teams->players = NULL;
    server->map->teams->next = NULL;
    
    void *result = free_server(server);
    cr_assert_null(result);
}

// Test map with players
Test(free_server, test_server_with_players)
{
    server_t *server = malloc(sizeof(server_t));
    server->params = NULL;
    server->sockfd = -1;
    server->graph = NULL;
    
    // Create map with teams and players
    server->map = malloc(sizeof(map_t));
    server->map->ressources = NULL;
    server->map->teams = malloc(sizeof(team_t));
    server->map->teams->name = malloc(10);
    server->map->teams->next = NULL;
    
    // Create player
    server->map->teams->players = malloc(sizeof(player_t));
    server->map->teams->players->inventory = malloc(sizeof(int) * 7);
    server->map->teams->players->lives = malloc(sizeof(lives_t));
    server->map->teams->players->next = NULL;
    
    void *result = free_server(server);
    cr_assert_null(result);
}

// Test map with ressources
Test(free_server, test_server_with_ressources)
{
    server_t *server = malloc(sizeof(server_t));
    server->params = NULL;
    server->sockfd = -1;
    server->graph = NULL;
    
    // Create map with ressources
    server->map = malloc(sizeof(map_t));
    server->map->teams = NULL;
    server->map->ressources = malloc(sizeof(ressources_t));
    server->map->ressources->next = malloc(sizeof(ressources_t));
    server->map->ressources->next->next = NULL;
    
    void *result = free_server(server);
    cr_assert_null(result);
}

// Test multiple players in chain
Test(free_server, test_server_with_multiple_players)
{
    server_t *server = malloc(sizeof(server_t));
    server->params = NULL;
    server->sockfd = -1;
    server->graph = NULL;
    
    server->map = malloc(sizeof(map_t));
    server->map->ressources = NULL;
    server->map->teams = malloc(sizeof(team_t));
    server->map->teams->name = malloc(10);
    server->map->teams->next = NULL;
    
    // Create multiple players
    server->map->teams->players = malloc(sizeof(player_t));
    server->map->teams->players->inventory = NULL;
    server->map->teams->players->lives = NULL;
    server->map->teams->players->next = malloc(sizeof(player_t));
    server->map->teams->players->next->inventory = malloc(sizeof(int) * 7);
    server->map->teams->players->next->lives = malloc(sizeof(lives_t));
    server->map->teams->players->next->next = NULL;
    
    void *result = free_server(server);
    cr_assert_null(result);
}

// Test multiple teams in chain
Test(free_server, test_server_with_multiple_teams)
{
    server_t *server = malloc(sizeof(server_t));
    server->params = NULL;
    server->sockfd = -1;
    server->graph = NULL;
    
    server->map = malloc(sizeof(map_t));
    server->map->ressources = NULL;
    
    // Create multiple teams
    server->map->teams = malloc(sizeof(team_t));
    server->map->teams->name = malloc(10);
    server->map->teams->players = NULL;
    server->map->teams->next = malloc(sizeof(team_t));
    server->map->teams->next->name = NULL;
    server->map->teams->next->players = NULL;
    server->map->teams->next->next = NULL;
    
    void *result = free_server(server);
    cr_assert_null(result);
}

// Test graph with NULL pollfd
Test(free_server, test_server_with_null_pollfd)
{
    server_t *server = malloc(sizeof(server_t));
    server->params = NULL;
    server->sockfd = -1;
    server->map = NULL;
    
    server->graph = malloc(sizeof(graph_t));
    
    void *result = free_server(server);
    cr_assert_null(result);
}

