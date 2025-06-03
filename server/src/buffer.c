/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** buffer
*/

#include "buffer.h"
#include "zappy.h"


/* Utiliser cette fonction pour wrap */
int advance(int idx)
{
    return (idx + 1) % BUFFER_SIZE;
}

/* Ajouter un élément dans le buffer*/
void cb_write(buffer_t *cb, char c)
{
    cb->data[cb->head] = c;
    cb->head = advance(cb->head);
    if (cb->full)
        cb->tail = advance(cb->tail);
    cb->full = (cb->head == cb->tail);
}

/* Utiliser la line lu pour remplir le bufer */
int cb_getline(buffer_t *cb, char *line, int max_len)
{
    int i = 0;
    int idx = cb->tail;
    char c = 0;

    while (idx != cb->head || cb->full) {
        c = cb->data[idx];
        line[i++] = c;
        idx = advance(idx);
        if (c == '\n' || i == max_len - 1) {
            line[i] = '\0';
            cb->tail = idx;
            cb->full = 0;
            return i;
        }
    }
    return 0;
}
