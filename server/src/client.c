/*
** EPITECH PROJECT, 2025
** Zapppy
** File description:
** Server :: Client
*/

#include <stdlib.h>
#include <unistd.h>

#include "zappy.h"

void free_clients(client_t *clients)
{
    client_t *current = clients;
    client_t *next;

    while (current) {
        next = current->next;
        close(current->sockfd);
        free(current);
        current = next;
    }
}

int get_nb_clients(client_t *clients)
{
    int result = 0;
    client_t *current = clients;

    if (!current)
        return result;
    while (current) {
        result++;
        current = current->next;
    }
    return result;
}
