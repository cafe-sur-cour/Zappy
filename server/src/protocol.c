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
#include <arpa/inet.h>

#include "zappy.h"

static void diplay_help(int port)
{
    printf("\033[1;33mWelcome to the Zappy server!\033[0m\n");
    printf("\033[1;29mPress 'CTRL+C' to stop service\033[0m\n");
    printf("\033[1;29mServer listening on port: %d\033[0m\n\n", port);
}

struct pollfd *init_pollfds(server_t *server)
{
    client_t *current = server->clients;
    int i = 1;
    struct pollfd *poll_fds = calloc(
        get_nb_clients(server->clients) + 1, sizeof(struct pollfd));

    if (!poll_fds) {
        error_message("Failed to allocate memory for poll file descriptors.");
        return NULL;
    }
    poll_fds[0].fd = server->sockfd;
    poll_fds[0].events = POLLIN;
    while (current != NULL) {
        poll_fds[i] = current->pollfd;
        current = current->next;
        i++;
    }
    return poll_fds;
};

static int accept_client(server_t *server)
{
    int new_sockfd = accept(server->sockfd, NULL, NULL);

    if (new_sockfd < 0) {
        printf("\033[0;31mFailed to accept new client connection.\033[0m");
        return -1;
    }
    printf("New client connected.\n");
    return 0;
}

int start_protocol(server_t *server)
{
    struct pollfd *pollfds;

    diplay_help(server->params->port);
    while (1) {
        pollfds = init_pollfds(server);
        if (poll(pollfds, get_nb_clients(server->clients) + 1, 0) == -1) {
            error_message("Poll failed.");
            free(pollfds);
            return -1;
        }
        if (pollfds[0].revents & POLLIN)
            accept_client(server);
        free(pollfds);
    }
    printf("\033[1;33mServer stopped.\033[0m\n");
    return 0;
}
