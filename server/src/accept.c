/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** Server :: Accept
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#include "zappy.h"

static int check_team_name(server_t *server, int new_sockfd)
{
    char *message = NULL;

    if (write_message(new_sockfd, "WELCOME\n") == -1)
        return -1;
    message = get_message(new_sockfd, server);
    if (!message) {
        error_message("Failed to read team name message from client.");
        close(new_sockfd);
        return -1;
    }
    if (!process_new_client(message, new_sockfd, server)) {
        free(message);
        close(new_sockfd);
        return -1;
    }
    return 0;
}

int accept_client(server_t *server)
{
    int new_sockfd = accept(server->sockfd, NULL, NULL);

    if (new_sockfd == -1) {
        error_message("Failed to accept new client connection.");
        return -1;
    }
    if (check_team_name(server, new_sockfd) == -1) {
        close(new_sockfd);
        return -1;
    }
    realloc_pollfds(server, new_sockfd);
    return 0;
}
