/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server :: Zappy header
*/

#include <stdbool.h>

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
} params_t;

typedef struct server_s {
    params_t *params;
} server_t;

typedef struct command_pf_s {
    char const *flag;
    bool (*checker)(const char *, const char *, params_t *);
} command_pf_t;

/* errors.c */
int helper(void);
void error_message(const char *message);

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

#endif /* !ZAPPY_H_ */
