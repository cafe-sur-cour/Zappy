/*
** EPITECH PROJECT, 2025
** Zappipicaca prout
** File description:
** Server :: Protocol
*/

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <signal.h>
#include <arpa/inet.h>

#include "zappy.h"

static int *get_running_state(void)
{
    static int running = 1;

    return &running;
}

static void handle_sigint(int sig)
{
    (void)sig;
    *get_running_state() = 0;
}

static void setup_signal(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
}

static void diplay_help(int port)
{
    printf("\033[1;33mWelcome to the Zappy server!\033[0m\n");
    printf("\033[1;29mPress 'CTRL+C' to stop service\033[0m\n");
    printf("\033[1;29mServer listening on port: %d\033[0m\n\n", port);
}

static int init_pollfds(server_t *server)
{
    int max_fds = server->params->nb_client * server->params->nb_team + 1;

    server->poll_fds = calloc(max_fds, sizeof(struct pollfd));
    if (!server->poll_fds) {
        error_message("Failed to allocate memory for poll file descriptors.");
        return -1;
    }
    server->nb_poll = 1;
    server->poll_fds[0].fd = server->sockfd;
    server->poll_fds[0].events = POLLIN;
    for (int i = 1; i < server->params->nb_client *
        server->params->nb_team + 1; i++)
        server->poll_fds[i].fd = -1;
    return 0;
}

void realloc_pollfds(server_t *server, int new_fd)
{
    struct pollfd *new_poll_fds = realloc(server->poll_fds,
        (server->nb_poll + 1) * sizeof(struct pollfd));

    if (!new_poll_fds) {
        error_message("Failed to reallocate memory for poll");
        return;
    }
    server->poll_fds = new_poll_fds;
    server->poll_fds[server->nb_poll].fd = new_fd;
    server->poll_fds[server->nb_poll].events = POLLIN;
    server->nb_poll++;
}

int start_protocol(server_t *server)
{
    setup_signal();
    diplay_help(server->params->port);
    if (init_pollfds(server) == -1)
        return -1;
    while (*get_running_state()) {
        if (poll(server->poll_fds, server->nb_poll, 0) == -1
            && *get_running_state()) {
            error_message("Poll failed.");
            return -1;
        }
        if (server->poll_fds[0].revents & POLLIN)
            accept_client(server);
        if (server->graph->fd != -1){
            send_map_size(server);
        }
    }
    printf("\033[1;33mServer stopped.\033[0m\n");
    return 0;
}
