/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server :: Errors
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

#include "zappy.h"
#include "buffer.h"

int helper(void)
{
    printf("USAGE: ./zappy_server -p port -x width -y height ");
    printf("-n name1 name2 ... -c clientsNb -f freq\n");
    return 84;
}

void error_message(char const *message)
{
    printf("\033[1;31m[SERVER]\033[0m \033[0;31m%s\033[0m\n", message);
}

void printfd(char const *message, int fd)
{
    dprintf(fd, "%s\n", message);
}

static void print_received_message(char c, server_t *server)
{
    if (server->params->is_debug == true) {
        printf("Read character: '%c' (0x%02x)\n", c, (unsigned char)c);
    }
}

char *get_message(int fd, server_t *server)
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
        print_received_message(c, server);
    }
    return cb.data;
}

int write_message(int fd, const char *message)
{
    struct pollfd pollfd = {.fd = fd, .events = POLLOUT};

    if (poll(&pollfd, 1, 1000) == -1) {
        error_message("Client socket not ready for writing 1 .");
        close(fd);
        return -1;
    }
    if (pollfd.revents & POLLOUT) {
        if (write(fd, message, strlen(message)) == -1) {
            error_message("Failed to write message to client.");
            close(fd);
            return -1;
        }
        return 0;
    }
    error_message("Client socket not ready for writing 2 .");
    close(fd);
    return -1;
}
