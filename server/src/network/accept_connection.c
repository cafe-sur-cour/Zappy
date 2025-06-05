/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** accept_connection
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

int accept_connection(int server_fd)
{
    int new_socket = accept(server_fd, NULL, NULL);

    if (new_socket < 0) {
        perror("Accept failed");
        return -1;
    }
    return new_socket;
}
