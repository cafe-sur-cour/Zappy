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

static bool send_gui_message(server_t *server, bool tmp)
{
    if (server->graph->fd != -1 && tmp == false) {
        send_map_size(server);
        send_entrie_map(server);
        send_team_name(server);
        tmp = true;
    }
    return tmp;
}

int start_protocol(server_t *server)
{
    bool temp = false;

    setup_signal();
    diplay_help(server->params->port);
    while (*get_running_state()) {
        if (poll(&server->pollserver, 1, 100) == -1
            && *get_running_state()) {
            error_message("Poll failed.");
            return -1;
        }
        if (server->pollserver.revents & POLLIN)
            accept_client(server);
        temp = send_gui_message(server, temp);
    }
    printf("\033[1;33mServer stopped.\033[0m\n");
    return 0;
}
