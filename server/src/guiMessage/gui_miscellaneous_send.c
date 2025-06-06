/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_miscellaneous_send
*/

#include "zappy.h"
#include "network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This function sends the end of game message */
void send_end_game(zappy_t *zappy, const char *teamName)
{
    int xLength = strlen(teamName) + 6;
    char *message = malloc(sizeof(char) * xLength);

    if (message == NULL) {
        error_message("Failed to allocate memory for end game message.");
        return;
    }
    snprintf(message, xLength, "seg %s\n", teamName);
    if (zappy->params->is_debug) {
        printf("Sending end game message: %s", message);
    }
    write_message(zappy->graph->fd, message);
    free(message);
}

/* This sends a str Message from the server */
void send_str_message(zappy_t *zappy, const char *message)
{
    int xLength = strlen(message) + 6;
    char *formatted_message = malloc(sizeof(char) * xLength);

    if (formatted_message == NULL) {
        error_message("Failed to allocate memory for string message.");
        return;
    }
    snprintf(formatted_message, xLength, "smg %s\n", message);
    if (zappy->params->is_debug) {
        printf("Sending string message: %s", formatted_message);
    }
    write_message(zappy->graph->fd, formatted_message);
    free(formatted_message);
}

/* This function sends unkown message */
void send_unknown_command(zappy_t *zappy)
{
    const char *message = "suc\n";

    if (zappy->params->is_debug) {
        printf("Sending unknown command message: %s", message);
    }
    write_message(zappy->graph->fd, message);
}

/* This send the command parameter */
void send_command_parameter(zappy_t *zappy)
{
    const char *message = "sbp\n";

    if (zappy->params->is_debug) {
        printf("Sending command parameter message: %s", message);
    }
    write_message(zappy->graph->fd, message);
}
