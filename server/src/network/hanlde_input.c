/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** hanlde_input
*/

#include "buffer.h"

#include <unistd.h>
#include <poll.h>

char *get_message(int fd)
{
    static buffer_t cb = {0};
    char c = 0;
    struct pollfd pollfd = {.fd = fd, .events = POLLIN};

    while (1) {
        if (poll(&pollfd, 1, 100) == -1)
            return NULL;
        if (!(pollfd.revents & POLLIN))
            return NULL;
        if (read(fd, &c, 1) <= 0)
            return NULL;
        if (c == '\n') {
            cb_write(&cb, '\0');
            break;
        }
        cb_write(&cb, c);
    }
    return cb.data;
}
