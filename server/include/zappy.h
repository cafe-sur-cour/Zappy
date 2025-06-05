/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server :: Zappy header
*/

#include <stdbool.h>
#include <poll.h>
#include "game.h"
#include "my.h"

#ifndef ZAPPY_H_
    #define ZAPPY_H_

/* Cli parameter of the server */
typedef struct params_s {
    int port;
    int x;
    int y;
    int nb_team;
    char **teams;
    int nb_client;
    int freq;
    bool is_debug;
} params_t;

/* Structure to handle the network side of the gui*/
typedef struct graph_net_s {
    int fd;
    struct graph_net_s *next;
} graph_net_t;

/* Server part of the network */
typedef struct server_s {
    int sockfd;
    struct pollfd pollserver;
} server_t;

typedef struct zappy_s {
    server_t *network;
    game_t *game;
    graph_net_t *graph;
    params_t *params;
} zappy_t;

typedef struct command_pf_s {
    char const *flag;
    bool (*checker)(const char *, const char *, params_t *);
} command_pf_t;

/* messages.c */
int helper(void);
void error_message(const char *message);

/* checkers.c */
bool check_port(char const *flag, char const *value, params_t *params);
bool check_width(char const *flag, char const *value, params_t *params);
bool check_height(char const *flag, char const *value, params_t *params);
bool check_client(char const *flag, char const *value, params_t *params);
bool check_freq(char const *flag, char const *value, params_t *params);

/* signal.c */
void setup_signal(void);
int *get_running_state(void);

/* params.c */
params_t *check_args(int argc, char **argv);
void *free_params(params_t *params);

/* server.c */
zappy_t *init_server(int argc, char **argv);
void *free_zappy(zappy_t *server);

/* protocol.c */
int start_protocol(zappy_t *server);

/* client.c */
bool process_new_client(const char *team_name, int fd, zappy_t *server);
team_t *add_client_to_team(const char *team_name, int fd, zappy_t *server);

/* init_map.c */
void init_game(zappy_t *server);

/* accept.c */
int accept_client(zappy_t *server);

/* free server  */
void *free_zappy(zappy_t *server);
void *free_params(params_t *params);
void *free_player(player_t *player);
void free_map(map_t *map);

/* Function to send info to the gui */
void send_map_size(zappy_t *server);
void send_entrie_map(zappy_t *server);
void send_map_tile(inventory_t **tiles, zappy_t *server,
    int posX, int posY);
void send_team_name(zappy_t *server);
void send_egg(zappy_t *zappy, egg_t *egg);
void send_entire_egg_list(zappy_t *zappy);
void send_time_message(zappy_t *zappy);
void send_egg_death(zappy_t *zappy, egg_t *egg);

/* init_egg.c */
void init_egg(zappy_t *zappy);
egg_t *kil_egg_node(egg_t **head, int egg_id);

#endif /* !ZAPPY_H_ */
