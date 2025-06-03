/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** Server :: Server
*/

#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "zappy.h"

static int set_socket(server_t *server)
{
    int y = 1;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        error_message("Failed to create socket.");
        return -1;
    }
    server->sockfd = sockfd;
    if (setsockopt(server->sockfd, SOL_SOCKET, SO_REUSEADDR, &y,
        sizeof(y)) < 0) {
        error_message("Failed to set socket options.");
        close(server->sockfd);
        return -1;
    }
    return 0;
}

static int bind_socket(server_t *server)
{
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(server->params->port);
    if (bind(server->sockfd, (struct sockaddr *)&server_addr,
        sizeof(server_addr)) < 0) {
        error_message("Failed to bind socket.");
        return -1;
    }
    return 0;
}

static int listen_socket(server_t *server)
{
    if (listen(server->sockfd, 10) < 0) {
        error_message("Failed to listen on socket.");
        return -1;
    }
    return 0;
}

void *free_server(server_t *server)
{
    if (!server)
        return NULL;
    if (server->poll_fds)
        free(server->poll_fds);
    if (server->params)
        free_params(server->params);
    if (server->sockfd != -1)
        close(server->sockfd);
    free(server);
    return NULL;
}

server_t *init_server(int argc, char **argv)
{
    server_t *server = malloc(sizeof(server_t));

    if (!server) {
        error_message("Memory allocation failed for server.");
        return NULL;
    }
    server->nb_poll = 0;
    server->map = NULL;
    server->poll_fds = NULL;
    server->params = check_args(argc, argv);
    if (!server->params)
        return free_server(server);
    if (set_socket(server) == -1)
        return free_server(server);
    if (bind_socket(server) == -1)
        return free_server(server);
    if (listen_socket(server) == -1)
        return free_server(server);
    return server;
}
