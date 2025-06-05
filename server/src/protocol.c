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
#include <arpa/inet.h>

#include "zappy.h"


static void diplay_help(int port)
{
    printf("\033[1;33mWelcome to the Zappy server!\033[0m\n");
    printf("\033[1;29mPress 'CTRL+C' to stop service\033[0m\n");
    printf("\033[1;29mServer listening on port: %d\033[0m\n\n", port);
}

/* Loop thru the player to see connection updates */
// static void check_player(zappy_t *zappy)

/* Loop thrue egg list to see updtes */
static void check_eggs_status(zappy_t *zappy)
{
    egg_t *current = NULL;

    if (zappy->game->map->currentEggs == NULL)
        return;
    current = zappy->game->map->currentEggs;
    while (current != NULL) {
        if (current->isHatched == true) {
            send_egg_death(zappy, current);
            current = kil_egg_node(&zappy->game->map->currentEggs,
                current->id);
        }
        current = current->next;
    }
}

/* This is a temporaryt function that sends element to the gui */
static bool send_gui_message(zappy_t *zappy, bool tmp)
{
    if (zappy->graph->fd != -1 && tmp == false) {
        send_map_size(zappy);
        send_time_message(zappy);
        send_entrie_map(zappy);
        send_team_name(zappy);
        send_entire_egg_list(zappy);
        tmp = true;
    }
    check_eggs_status(zappy);
    return tmp;
}

int start_protocol(zappy_t *zappy)
{
    bool temp = false;

    setup_signal();
    diplay_help(zappy->params->port);
    while (*get_running_state()) {
        if (poll(&zappy->network->pollserver, 1, 100) == -1
            && *get_running_state()) {
            error_message("Poll failed.");
            return -1;
        }
        if (zappy->network->pollserver.revents & POLLIN)
            accept_client(zappy);
        temp = send_gui_message(zappy, temp);
    }
    printf("\033[1;33mServer stopped.\033[0m\n");
    return 0;
}
