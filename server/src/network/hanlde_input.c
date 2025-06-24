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

static int len_message(int start, int end)
{
    int message_len = 0;

    if (start == end)
        return -1;
    if (end > start)
        message_len = end - start;
    else
        message_len = (BUFFER_SIZE - start) + end;
    if (message_len <= 0)
        return -1;
    return message_len;
}

static char *end_message(buffer_t *cb)
{
    int message_len = -1;
    int start = cb->tail;
    int end = cb->head;
    char *message = NULL;
    int i = 0;

    message_len = len_message(start, end);
    if (message_len == -1)
        return NULL;
    message = malloc(sizeof(char) * message_len);
    if (!message)
        return NULL;
    while (start != end && i < message_len - 1) {
        message[i] = cb->data[start];
        start = (start + 1) % BUFFER_SIZE;
        i++;
    }
    message[i] = '\0';
    return message;
}

static buffer_t *get_message_from_buffer(int fd, buffer_t *cb,
    char c)
{
    while (1) {
        if (read(fd, &c, 1) <= 0)
            return NULL;
        cb_write(cb, c);
        if (c == '\n')
            break;
    }
    return cb;
}

char *get_message(int fd)
{
    static buffer_t cb = {.head = 0, .tail = 0, .full = 0};
    buffer_t *cb_ptr = &cb;
    char c = 0;

    if (fd < 0)
        return NULL;
    cb.tail = cb.head;
    cb_ptr = get_message_from_buffer(fd, cb_ptr, c);
    if (cb_ptr == NULL)
        return NULL;
    return end_message(cb_ptr);
}
