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

static char *get_current_char(buffer_t *cb, char *line)
{
    if (cb_getline(cb, line, BUFFER_SIZE) > 0) {
        if (strchr(line, '\n')) {
            line[strcspn(line, "\n")] = '\0';
            return line;
        }
    }
}

char *get_message(int fd, server_t *server)
{
    static buffer_t cb = {0};
    char c = 0;
    char *line = malloc(BUFFER_SIZE);
    int bytes_read;

    if (!line) {
        error_message("Failed to allocate memory for message line.");
        return NULL;
    }
    while (1) {
        get_current_char(&cb, line);
        bytes_read = read(fd, &c, 1);
        if (bytes_read <= 0) {
            free(line);
            return NULL;
        }
        cb_write(&cb, c);
        print_received_message(c, server);
    }
}
