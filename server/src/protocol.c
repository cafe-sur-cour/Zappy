/*
** EPITECH PROJECT, 2025
** Zappipicaca prout
** File description:
** Server :: Protocol
*/

#include <stdbool.h>
#include <stdlib.h>
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

static void diplay_help(int port)
{
    printf("\033[1;33mWelcome to the Zappy server!\033[0m\n");
    printf("\033[1;29mPress 'CTRL+C' to stop service\033[0m\n");
    printf("\033[1;29mServer listening on port: %d\033[0m\n\n", port);
}

struct pollfd *init_pollfds(server_t *server)
{
    struct pollfd *poll_fds = calloc(server->params->nb_client *
        server->params->nb_team + 2, sizeof(struct pollfd));

    if (!poll_fds) {
        error_message("Failed to allocate memory for poll file descriptors.");
        return NULL;
    }
    poll_fds[0].fd = server->sockfd;
    poll_fds[0].events = POLLIN;
    for (int i = 1; i < server->params->nb_client *
        server->params->nb_team + 1; i++)
        poll_fds[i].fd = -1;
    return poll_fds;
};

static int accept_client(server_t *server)
{
    int new_sockfd = accept(server->sockfd, NULL, NULL);

    if (new_sockfd == -1) {
        printf("\033[0;31mFailed to accept new client connection.\033[0m");
        return -1;
    }
    printfd("WELCOME", new_sockfd);
    printf("New client connected.\n");
    return 0;
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

int start_protocol(server_t *server)
{
    struct pollfd *pollfds = init_pollfds(server);

    setup_signal();
    diplay_help(server->params->port);
    while (*get_running_state()) {
        if (poll(pollfds, server->params->nb_client *
            server->params->nb_team + 2, 0) == -1 && *get_running_state()) {
            error_message("Poll failed.");
            free(pollfds);
            return -1;
        }
        if (pollfds[0].revents & POLLIN)
            accept_client(server);
    }
    printf("\033[1;33mServer stopped.\033[0m\n");
    return 0;
}
