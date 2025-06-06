/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** hanlde_input
*/

#include "buffer.h"

#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static char *end_message(buffer_t *cb)
{
    int i = cb->tail;
    char *message = malloc(sizeof(char) * (cb->head - cb->tail));

    for (; i < cb->head - 1; i++)
        message[i - cb->tail] = cb->data[i];
    message[i - cb->tail] = '\0';
    return message;
}

char *get_message(int fd, int timeout)
{
    static buffer_t cb = {.head = 0, .tail = 0, .full = 0};
    char c = 0;
    struct pollfd pollfd = {.fd = fd, .events = POLLIN};

    cb.tail = cb.head;
    while (1) {
        if (poll(&pollfd, 1, timeout) == -1 ||
            poll(&pollfd, 1, timeout) == 0)
            return NULL;
        if (!(pollfd.revents & POLLIN))
            return NULL;
        if (read(fd, &c, 1) <= 0)
            return NULL;
        cb_write(&cb, c);
        if (c == '\n')
            break;
    }
    return end_message(&cb);
}
