/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** send_message
*/

#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <stdio.h>

#include "network.h"

int write_message(int fd, const char *message)
{
    int write_value = -1;

    write_value = write(fd, message, strlen(message));
    if (write_value == -1) {
        error_print("Failed to write message to client.");
        close(fd);
        return -1;
    }
    if (write_value != strlen(message)) {
        error_print("Partial write sent to client.");
    }
    return 0;
}
