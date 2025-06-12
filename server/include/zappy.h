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

/* items handler */
typedef struct {
    char *name;
    void (*add_func)(inventory_t *);
} item_handler_t;


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
    bool mapSent;
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

typedef struct {
    char *command;
    float base_time;
    action_priority_t priority;
    int (*handler)(player_t *, char *, zappy_t *);
} command_info_t;

typedef struct graphic_pf_s {
    char *command;
    int (*handler)(zappy_t *zappy, graph_net_t *graphic, char *message);
} graphic_pf_t;

/* messages.c */
int helper(void);
void error_message(const char *message);
void valid_message(char const *message);

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

/* params_cherckers.c */
bool validate_no_extra_args(int argc, char **argv);

/* server.c */
zappy_t *init_server(int argc, char **argv);
void *free_zappy(zappy_t *server);

/* protocol.c */
int start_protocol(zappy_t *server);

/* client.c */
bool process_new_client(const char *team_name, int fd, zappy_t *server);
team_t *add_client_to_team(const char *team_name, int fd, zappy_t *server);
int get_next_free_id(zappy_t *server);
void check_player_status(zappy_t *zappy);

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
int send_map_size(zappy_t *server);
int send_entrie_map(zappy_t *server);
int send_map_tile(inventory_t **tiles, zappy_t *server,
    int posX, int posY);
int send_team_name(zappy_t *server);
int send_egg(zappy_t *zappy, egg_t *egg);
int send_entire_egg_list(zappy_t *zappy);
int send_time_message(zappy_t *zappy);
int send_egg_death(zappy_t *zappy, egg_t *egg);
int send_egg_connect(zappy_t *zappy, egg_t *currentEgg);
int send_player_connect(zappy_t *zappy, player_t *player);
int send_player_pos(zappy_t *zappy, player_t *player);
int send_player_level(zappy_t *zappy, player_t *player);
int send_player_inventory(zappy_t *zappy, player_t *player);
int send_player_expelled(zappy_t *zappy, player_t *player);
int send_broadcast_to_all(zappy_t *zappy, const char *message);
int send_broadcast_to_player(zappy_t *zappy, player_t *player,
    const char *message);
int send_player_laying_egg(zappy_t *zappy, player_t *player);
int send_ressource_droped(zappy_t *zappy, player_t *player,
    int ressourceType);
int send_ressource_collected(zappy_t *zappy, player_t *player,
    int ressourceType);
int send_player_death(zappy_t *zappy, player_t *player);
int send_updated_time(zappy_t *zappy, int time);
int send_end_game(zappy_t *zappy, const char *teamName);
int send_str_message(zappy_t *zappy, const char *message);
int send_unknown_command(zappy_t *zappy);
int send_command_parameter(zappy_t *zappy);
int send_start_incantation(zappy_t *zappy, player_t *player, int *player_list,
    int nb_player);
int send_end_incantation(zappy_t *zappy, player_t *player, char *result);

/* init_egg.c */
void init_egg(zappy_t *zappy);
egg_t *add_egg_node(int id, int *pos, char *team_name, int id_layer);
egg_t *kil_egg_node(egg_t **head, int egg_id);

/* AI messages */
int forward_message(player_t *player, params_t *params);

/* Pollin handler */
void smart_poll_players(zappy_t *zappy);
void execute_action(player_t *player, action_request_t *action,
    zappy_t *zappy);
void queue_action(player_t *player, char *command, zappy_t *zappy);
action_queue_t *init_action_queue(void);
void free_action_queue(action_queue_t *queue);
action_request_t *create_action_request(char *command, player_t *player,
    int frequency);
const command_info_t *find_command_info(char *command);
action_request_t *dequeue_highest_priority_action(action_queue_t *queue);
void free_action_request(action_request_t *action);
void insert_action_by_priority(action_queue_t *queue,
    action_request_t *action);

/* This is the definition of the array function of the commands */
int handle_forward(player_t *player, char *command, zappy_t *zappy);

int handle_left(player_t *player, char *command, zappy_t *zappy);
int left_message(player_t *player);
int print_left_server(player_t *player);

int handle_right(player_t *player, char *command, zappy_t *zappy);
int print_right_server(player_t *player);
int right_message(player_t *player);

int handle_connect_nbr(player_t *player, char *command, zappy_t *zappy);
int handle_eject(player_t *player, char *command, zappy_t *zappy);

int handle_fork(player_t *player, char *command, zappy_t *zappy);
int print_look_server(player_t *player);

int handle_incantation(player_t *player, char *command, zappy_t *zappy);
int handle_inventory(player_t *player, char *command, zappy_t *zappy);
int inventory_message(player_t *player);
int print_inventory_server(player_t *player, int len);

int handle_broadcast(player_t *player, char *command, zappy_t *zappy);
int handle_look(player_t *player, char *command, zappy_t *zappy);
int handle_set(player_t *player, char *command, zappy_t *zappy);
int handle_take(player_t *player, char *command, zappy_t *zappy);

/* graphic_clinet.c */
graph_net_t *add_graph_node(graph_net_t **head, int fd);
graph_net_t *remove_graph_node(graph_net_t **head, int fd);
void poll_graphic_clients(zappy_t *zappy);


/* Element hander.c */
void add_food(inventory_t *inventory);
void add_linemate(inventory_t *inventory);
void add_deraumere(inventory_t *inventory);
void add_sibur(inventory_t *inventory);
void add_mendiane(inventory_t *inventory);
void add_phiras(inventory_t *inventory);
void add_thystame(inventory_t *inventory);

void rm_food(inventory_t *inventory);
void rm_linemate(inventory_t *inventory);
void rm_deraumere(inventory_t *inventory);
void rm_sibur(inventory_t *inventory);
void rm_mendiane(inventory_t *inventory);
void rm_phiras(inventory_t *inventory);
void rm_thystame(inventory_t *inventory);

/* Element handler.c */
int msz(zappy_t *zappy, graph_net_t *graphic, char *message);
int bct(zappy_t *zappy, graph_net_t *graphic, char *message);
int mct(zappy_t *zappy, graph_net_t *graphic, char *message);
int tna(zappy_t *zappy, graph_net_t *graphic, char *message);
int ppo(zappy_t *zappy, graph_net_t *graphic, char *message);
int send_bct_message(graph_net_t *graphic, int x, int y,
    inventory_t *inventory);

#endif /* !ZAPPY_H_ */
