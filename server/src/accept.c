/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** zappy :: Accept
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "network.h"
#include "zappy.h"

static char *check_team_name(zappy_t *zappy, int new_sockfd)
{
    char *message = NULL;

    if (write_message(new_sockfd, "WELCOME\n") == -1)
        return NULL;
    message = get_message(new_sockfd);
    if (!message) {
        error_message("Failed to read team name message from client.");
        close(new_sockfd);
        return NULL;
    }
    if (!process_new_client(message, new_sockfd, zappy)) {
        close(new_sockfd);
        return NULL;
    }
    return message;
}

static int complete_connection(zappy_t *zappy, int fd, const char *message)
{
    char *buffer = calloc(12, sizeof(char));
    team_t *team = add_client_to_team(message, fd, zappy);

    if (strcmp(message, "GRAPHIC") == 0)
        return 0;
    if (!team || !buffer)
        return -1;
    snprintf(buffer, 12, "%d\n", zappy->params->nb_client - team->nbPlayers);
    if (write_message(fd, buffer) == -1)
        return -1;
    free(buffer);
    buffer = calloc(27, sizeof(char));
    if (!buffer)
        return -1;
    snprintf(buffer, 27, "%d %d\n", zappy->params->x, zappy->params->y);
    if (write_message(fd, buffer) == -1)
        return -1;
    free(buffer);
    return 0;
}

int accept_client(zappy_t *zappy)
{
    char *message = NULL;
    int new_sockfd = accept_connection(zappy->network->sockfd);

    message = check_team_name(zappy, new_sockfd);
    if (!message) {
        close(new_sockfd);
        return -1;
    }
    if (complete_connection(zappy, new_sockfd, message) == -1) {
        close(new_sockfd);
        return -1;
    }
    return 0;
}
