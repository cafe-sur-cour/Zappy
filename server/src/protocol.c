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
#include <time.h>

#include "zappy.h"


static void diplay_help(int port)
{
    printf("\033[1;33mWelcome to the Zappy server!\033[0m\n");
    printf("\033[1;29mPress 'CTRL+C' to stop service\033[0m\n");
    printf("\033[1;29mServer listening on port: %d\033[0m\n\n", port);
}

/* Loop thrue egg list to see updtes */
static void check_eggs_status(zappy_t *zappy)
{
    egg_t *current = NULL;

    if (zappy->game->map->currentEggs == NULL)
        return;
    current = zappy->game->map->currentEggs;
    while (current != NULL) {
        if (current->isHatched == true) {
            send_egg_connect(zappy, current);
            send_egg_death(zappy, current);
            current = kil_egg_node(&zappy->game->map->currentEggs,
                current->id);
            send_entire_egg_list(zappy);
        }
        if (current == NULL)
            return;
        current = current->next;
    }
}

/* This is a temporaryt function that sends element to the gui */
static void send_gui_message(zappy_t *zappy)
{
    check_eggs_status(zappy);
    check_player_status(zappy);
}

// int start_protocol(zappy_t *zappy)
// {
//     int tick_duration_ms = 1000 / zappy->params->freq;
//     double respawn_interval = 0;

//     setup_signal();
//     diplay_help(zappy->params->port);
//     while (*get_running_state()) {
//         tick_duration_ms = 1000 / zappy->params->freq;
//         if (poll(&zappy->network->pollserver, 1, tick_duration_ms) == -1
//             && *get_running_state()) {
//             error_message("Poll failed.");
//             return -1;
//         }
//         if (zappy->network->pollserver.revents & POLLIN)
//             accept_client(zappy);
//         send_gui_message(zappy);
//         smart_poll_players(zappy);
//         poll_graphic_clients(zappy);

//         respawn_interval = 20.0 / zappy->params->freq;
//         if (time_since_last_spawn >= respawn_interval) {
//             refill_food(zappy);
//             time_since_last_spawn = 0;
//         }
//     }
//     printf("\033[1;33mServer stopped.\033[0m\n");
//     return 0;
// }


int start_protocol(zappy_t *zappy)
{
    int tick_duration_ms = 1000 / zappy->params->freq;
    double respawn_interval = 20.0 / zappy->params->freq;
    double time_since_last_spawn = 0.0;

    struct timespec last_time, current_time;
    clock_gettime(CLOCK_MONOTONIC, &last_time);

    setup_signal();
    diplay_help(zappy->params->port);

    while (*get_running_state()) {
        tick_duration_ms = 1000 / zappy->params->freq;

        if (poll(&zappy->network->pollserver, 1, tick_duration_ms) == -1
            && *get_running_state()) {
            error_message("Poll failed.");
            return -1;
        }

        clock_gettime(CLOCK_MONOTONIC, &current_time);
        double delta = (current_time.tv_sec - last_time.tv_sec)
                     + (current_time.tv_nsec - last_time.tv_nsec) / 1e9;
        time_since_last_spawn += delta;
        last_time = current_time;

        if (zappy->network->pollserver.revents & POLLIN)
            accept_client(zappy);
        send_gui_message(zappy);
        smart_poll_players(zappy);
        poll_graphic_clients(zappy);

        if (time_since_last_spawn >= respawn_interval) {
            printf("Time %.2f seconds vs respawn %.2f.\n", time_since_last_spawn, respawn_interval);
            refill_food(zappy);
            time_since_last_spawn = 0.0;
        }
    }

    printf("\033[1;33mServer stopped.\033[0m\n");
    return 0;
}
