/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** signal
*/


#include "zappy.h"

#include <signal.h>


int *get_running_state(void)
{
    static int running = 1;

    return &running;
}

void handle_sigint(int sig)
{
    (void)sig;
    *get_running_state() = 0;
}

void setup_signal(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
}
