/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** close
*/

#include <unistd.h>

void close_fd(int fd)
{
    if (fd < 0)
        return;
    close(fd);
}
