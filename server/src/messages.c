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

static char *allocate_buffer(void)
{
    char *buffer = calloc(1, sizeof(char));

    if (!buffer) {
        error_message("Failed to allocate memory for message buffer.");
        return NULL;
    }
    return buffer;
}

static char *resize_buffer(char *buffer, size_t len, char c)
{
    buffer = realloc(buffer, len + 2);
    if (!buffer) {
        error_message("Failed to reallocate memory for message buffer.");
        return NULL;
    }
    buffer[len] = c;
    return buffer;
}

char *get_message(int fd)
{
    char c = 0;
    char *buffer = allocate_buffer();

    if (!buffer)
        return NULL;
    for (size_t len = 0; c != '\n'; len++) {
        if (read(fd, &c, 1) <= 0) {
            free(buffer);
            return NULL;
        }
        if (c == '\n') {
            buffer[len] = '\0';
            return buffer;
        }
        buffer = resize_buffer(buffer, len, c);
        if (!buffer)
            return NULL;
    }
    return buffer;
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
