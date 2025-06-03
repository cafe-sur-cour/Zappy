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

char *get_message(int fd, server_t *server)
{
    static buffer_t cb = {0};
    char c = 0;
    char *line = NULL;
    int bytes_read;

    while (1) {
        bytes_read = read(fd, &c, 1);
        if (bytes_read <= 0) {
            free(line);
            return NULL;
        }
        cb_write(&cb, c);
        print_received_message(c, server);
        if (c == '\n')
            break;
    }
    return cb.data;
}

int write_message(int fd, const char *message)
{
    struct pollfd pollfd = {.fd = fd, .events = POLLOUT};

    if (poll(&pollfd, 1, 10) == -1) {
        error_message("Client socket not ready for writing.");
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
    error_message("Client socket not ready for writing.");
    close(fd);
    return -1;
}
