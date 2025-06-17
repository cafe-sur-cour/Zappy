/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** graphic_client
*/

#include "zappy.h"
#include "network.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const graphic_pf_t GRAPHIC_COMMAND[] = {
    {"msz", &msz},
    {"bct", &bct},
    {"mct", &mct},
    {"tna", &tna},
    {"ppo", &ppo},
    {"plv", &plv},
    {"pin", &pin},
    {"sgt", &sgt},
    {"sst", &sst},
    {"kil", &kil},
    {NULL, NULL}
};

graph_net_t *add_graph_node(graph_net_t **head, int fd)
{
    graph_net_t *new_node = malloc(sizeof(graph_net_t));

    if (!new_node)
        return NULL;
    new_node->fd = fd;
    new_node->mapSent = false;
    new_node->next = *head;
    *head = new_node;
    return new_node;
}

static graph_net_t *find_node(graph_net_t *current, graph_net_t *prev,
    graph_net_t **head, int fd)
{
    if (current->fd == fd) {
        if (prev)
            prev->next = current->next;
        else
            *head = current->next;
        current->next = NULL;
        return current;
    }
    return current;
}

graph_net_t *remove_graph_node(graph_net_t **head, int fd)
{
    graph_net_t *current = *head;
    graph_net_t *prev = NULL;

    while (current) {
        current = find_node(current, prev, head, fd);
        prev = current;
        current = current->next;
    }
    return NULL;
}

static void disconnect_and_remove_client(zappy_t *zappy, int fd)
{
    graph_net_t *removed = remove_graph_node(&zappy->graph, fd);

    valid_message("Graphic client disconnected");
    close(fd);
    if (removed) {
        free(removed);
    }
}

static int poll_graphic_commands(zappy_t *zappy, graph_net_t *current,
    char *buffer)
{
    for (int i = 0; GRAPHIC_COMMAND[i].command != NULL; i++) {
        if (strncmp(GRAPHIC_COMMAND[i].command, buffer,
            strlen(GRAPHIC_COMMAND[i].command)) == 0)
            return GRAPHIC_COMMAND[i].handler(zappy, current, buffer);
    }
    return -1;
}

static int poll_graphic(zappy_t *zappy, int poll_result,
    struct pollfd *pfd, graph_net_t *current)
{
    char *buffer = NULL;

    pfd->events = POLLIN;
    poll_result = poll(pfd, 1, 0);
    if (poll_result == -1 || (pfd->revents & (POLLHUP | POLLERR | POLLNVAL))) {
        disconnect_and_remove_client(zappy, current->fd);
        return -1;
    }
    if (pfd->revents & POLLIN) {
        buffer = get_message(pfd->fd, 50);
        if (buffer == NULL) {
            disconnect_and_remove_client(zappy, current->fd);
            return -1;
        }
        return poll_graphic_commands(zappy, current, buffer);
    }
    return 0;
}

void poll_graphic_clients(zappy_t *zappy)
{
    graph_net_t *current = zappy->graph;
    graph_net_t *next = NULL;
    struct pollfd pfd = {0};
    int poll_result = -1;

    while (current) {
        next = current->next;
        pfd.fd = current->fd;
        poll_graphic(zappy, poll_result, &pfd, current);
        current = next;
    }
}
