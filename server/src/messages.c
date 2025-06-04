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
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    char time_str[26];
    strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("\033[0;31m%s\033[0m\t\033[1;31m%s\033[0m\n", time_str, message);
}

void printfd(char const *message)
{
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    char time_str[26];
    strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("\033[0;32m%s\033[0m\t\033[1;29m%s\033[0m\n", time_str, message);
}

static void print_received_message(char c, server_t *server)
{
    if (server->params->is_debug == true) {
        printf("Read character: '%c' (0x%02x)\n", c, (unsigned char)c);
    }
}

static char *end_message(buffer_t *cb)
{
    int i = cb->tail;
    char *message = malloc(sizeof(char) * (cb->head - cb->tail));

    for (; i < cb->head - 1; i++)
        message[i - cb->tail] = cb->data[i];
    message[i - cb->tail] = '\0';
    return message;
}

char *get_message(int fd, server_t *server)
{
    static buffer_t cb = {.head = 0, .tail = 0, .full = 0};
    char c = 0;
    struct pollfd pollfd = {.fd = fd, .events = POLLIN};

    cb.tail = cb.head;
    while (1) {
        if (poll(&pollfd, 1, 1000) == -1)
            return NULL;
        if (!(pollfd.revents & POLLIN))
            return NULL;
        if (read(fd, &c, 1) <= 0)
            return NULL;
        cb_write(&cb, c);
        if (c == '\n') {
            break;
        }
        print_received_message(c, server);
    }
    return end_message(&cb);
}

int write_message(int fd, const char *message)
{
    struct pollfd pollfd = {.fd = fd, .events = POLLOUT};

    if (poll(&pollfd, 1, 1000) == -1) {
        error_message("Client socket not ready for writing");
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
    error_message("Client socket not ready for writing");
    close(fd);
    return -1;
}
