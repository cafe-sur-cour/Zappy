/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** unified_polling
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <unistd.h>

static void get_message_client(int fd, player_t *player, zappy_t *zappy)
{
    char *message = get_message(fd);

    if (message) {
        queue_action(player, message, zappy);
        free(message);
    }
}

static int handle_ai_client_message(zappy_t *zappy, int fd, player_t *player,
    short revents)
{
    while (player) {
        if (player->network && player->network->fd != fd) {
            player = player->next;
            continue;
        }
        if (revents & (POLLHUP | POLLERR | POLLNVAL)) {
            valid_message("Player disconnected");
            remove_player_by_fd(zappy, fd);
            close(fd);
            return 0;
        }
        if (revents & POLLIN) {
            get_message_client(fd, player, zappy);
        }
        return 0;
        player = player->next;
    }
    return -1;
}

static void handle_player_fd(zappy_t *zappy, int fd, short revents)
{
    team_t *team = zappy->game->teams;
    player_t *player = NULL;

    while (team) {
        player = team->players;
        if (handle_ai_client_message(zappy, fd, player, revents) == 0)
            return;
        team = team->next;
    }
}

static void get_graphic_buffer(int fd, graph_net_t *current,
    zappy_t *zappy)
{
    char *buffer = get_message(fd);

    if (buffer) {
        poll_graphic_commands(zappy, current, buffer);
        free(buffer);
    }
}

static void handle_graphic_fd(zappy_t *zappy, int fd, short revents)
{
    graph_net_t *current = zappy->graph;

    while (current) {
        if (current->fd != fd) {
            current = current->next;
            continue;
        }
        if (revents & (POLLHUP | POLLERR | POLLNVAL)) {
            remove_graph_node(&zappy->graph, fd);
            valid_message("Graphic client disconnected");
            close(fd);
            return;
        }
        if (revents & POLLIN) {
            get_graphic_buffer(fd, current, zappy);
        }
        return;
        current = current->next;
    }
}

static void poll_graphics_clients(zappy_t *zappy)
{
    graph_net_t *graphic = zappy->graph;

    while (graphic) {
        if (graphic->fd >= 0) {
            add_fd_to_poll(zappy->unified_poll, graphic->fd, POLLIN);
        }
        graphic = graphic->next;
    }
}

static void accept_client_fd_to_list(zappy_t *zappy, player_t *player)
{
    while (player) {
        if (player->network && player->network->fd >= 0) {
            add_fd_to_poll(zappy->unified_poll, player->network->fd, POLLIN);
        }
        player = player->next;
    }
}

void rebuild_poll_fds(zappy_t *zappy)
{
    team_t *team = NULL;
    player_t *player = NULL;

    if (!zappy || !zappy->unified_poll)
        return;
    zappy->unified_poll->count = 0;
    add_fd_to_poll(zappy->unified_poll, zappy->network->sockfd, POLLIN);
    team = zappy->game->teams;
    while (team) {
        player = team->players;
        accept_client_fd_to_list(zappy, player);
        team = team->next;
    }
    poll_graphics_clients(zappy);
}

static int verify_value_polls(zappy_t *zappy)
{
    int tick_duration_ms = 0;
    int poll_result = 0;

    if (!zappy || !zappy->unified_poll)
        return -1;
    rebuild_poll_fds(zappy);
    if (zappy->unified_poll->count == 0)
        return -1;
    tick_duration_ms = 1000 / zappy->params->freq;
    poll_result = poll(zappy->unified_poll->fds, zappy->unified_poll->count,
        tick_duration_ms);
    if (poll_result == -1)
        return -1;
    if (poll_result == 0)
        return -1;
    return tick_duration_ms;
}

void poll_all_clients(zappy_t *zappy)
{
    int tick_duration_ms = verify_value_polls(zappy);
    int fd = -1;
    short revents = 0;

    if (tick_duration_ms == -1)
        return;
    for (int i = 0; i < zappy->unified_poll->count; i++) {
        if (zappy->unified_poll->fds[i].revents == 0)
            continue;
        fd = zappy->unified_poll->fds[i].fd;
        revents = zappy->unified_poll->fds[i].revents;
        if (fd == zappy->network->sockfd && revents & POLLIN)
            accept_client(zappy);
        handle_player_fd(zappy, fd, revents);
        handle_graphic_fd(zappy, fd, revents);
    }
}
