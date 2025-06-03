/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server :: Zappy header
*/

#include <stdbool.h>
#include <poll.h>
#include "game.h"

#ifndef ZAPPY_H_
    #define ZAPPY_H_

typedef struct params_s {
    int port;
    int x;
    int y;
    int nb_team;
    char **teams;
    int nb_client;
    int freq;
    bool isDebug;
} params_t;

typedef struct server_s {
    int sockfd;
    int nb_poll;
    params_t *params;
    map_t *map;
    struct pollfd *poll_fds;
} server_t;

typedef struct command_pf_s {
    char const *flag;
    bool (*checker)(const char *, const char *, params_t *);
} command_pf_t;

/* messages.c */
int helper(void);
void error_message(const char *message);
void printfd(char const *message, int fd);
char *get_message(int fd);
int write_message(int fd, const char *message);

/* checkers.c */
bool check_port(char const *flag, char const *value, params_t *params);
bool check_width(char const *flag, char const *value, params_t *params);
bool check_height(char const *flag, char const *value, params_t *params);
bool check_client(char const *flag, char const *value, params_t *params);
bool check_freq(char const *flag, char const *value, params_t *params);

/* params.c */
params_t *check_args(int argc, char **argv);
void *free_params(params_t *params);

/* server.c */
server_t *init_server(int argc, char **argv);
void *free_server(server_t *server);

/* protocol.c */
int start_protocol(server_t *server);
void realloc_pollfds(server_t *server, int new_fd);

/* client.c */
bool valid_team_name(const char *team_name, server_t *server);

/* init_map.c */
void init_map(server_t *server);

/* accept.c */
int accept_client(server_t *server);

#endif /* !ZAPPY_H_ */
