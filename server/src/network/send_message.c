/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** send_message
*/

#include <string.h>
#include <unistd.h>
#include <poll.h>
#include "network.h"

int write_message(int fd, const char *message)
{
    struct pollfd pollfd = {.fd = fd, .events = POLLOUT};

    if (poll(&pollfd, 1, 1000) == -1) {
        error_print("Client socket not ready for writing.");
        close(fd);
        return -1;
    }
    if (pollfd.revents & POLLOUT) {
        if (write(fd, message, strlen(message)) == -1) {
            error_print("Failed to write message to client.");
            close(fd);
            return -1;
        }
        return 0;
    }
    error_print("Client socket not ready for writing.");
    close(fd);
    return -1;
}
