/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_message
*/

#include "zappy.h"

#include <stdlib.h>
#include <stdio.h>

static int *calloc_int(void)
{
    int *elem = malloc(sizeof(int) * 7);

    if (elem == NULL)
        return NULL;
    for (int i = 0; i < 7; i++)
        elem[i] = 0;
    return elem;
}

static int *count_cases(ressources_t *current, int *elem)
{
    crystal_t *cryst = malloc(sizeof(crystal_t) * 7);

    if (cryst == NULL)
        return NULL;
    for (int i = 0; i < 7; i++) {
        if (current->type == cryst[i]) {
            elem[i] += 1;
        }
    }
    return elem;
}

static int *nb_elem(ressources_t *ressources, int x, int y)
{
    ressources_t *current = ressources;
    int *elem = calloc_int();

    if (elem == NULL)
        return NULL;
    while (current != NULL) {
        if (current->posX == x && current->posY == y) {
            elem = count_cases(current, elem);
        }
        current = current->next;
    }
    return elem;
}

/* Send the bct message to the gui */
void send_map_tile(ressources_t *ressource, server_t *server,
    int posX, int posY)
{
    int *elem = nb_elem(ressource, posX, posY);
    int xLength = 0;
    char *message = NULL;

    if (elem == NULL)
        return;
    xLength = int_str_len(posX) +
        int_str_len(posY) +
        int_str_len(elem[0]) + int_str_len(elem[1]) +
        int_str_len(elem[2]) + int_str_len(elem[3]) +
        int_str_len(elem[4]) + int_str_len(elem[5]) +
        int_str_len(elem[6]) + 14;
    message = malloc(sizeof(char) * xLength);
    snprintf(message, xLength, "bct %d %d %d %d %d %d %d %d %d\n",
        posX, posY,
        elem[0], elem[1], elem[2], elem[3],
        elem[4], elem[5], elem[6]);
    write_message(server->graph->fd, message);
    free(message);
    free(elem);
}

void send_entrie_map(server_t *server)
{
    for (int x = 0; x < server->map->width; x++) {
        for (int y = 0; y < server->map->heigt; y++) {
            send_map_tile(server->map->ressources, server, x, y);
        }
    }
}

/* Send  the msz message to the gui */
void send_map_size(server_t *server)
{
    int xLenthth = int_str_len(server->map->width) +
        int_str_len(server->map->heigt) + 7;
    char *message = malloc(sizeof(char) * xLenthth);
    char *x = my_itoa(server->map->width);
    char *y = my_itoa(server->map->heigt);

    snprintf(message, xLenthth, "msz %s %s\n", x, y);
    write_message(server->graph->fd, message);
    free(message);
    free(x);
    free(y);
}
