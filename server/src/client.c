/*
** EPITECH PROJECT, 2025
** Zapppy
** File description:
** Server :: Client
*/

#include "zappy.h"

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
