/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** loop_thru_command
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* This is the array of function pointer that holds all the cmds */
static const command_info_t COMMANDS[] = {
    {"Forward", 7, PRIORITY_MEDIUM, &handle_forward},
    {NULL, 0, 0, NULL}
};

// {"Right", 7, PRIORITY_MEDIUM, &handle_right},
// {"Left", 7, PRIORITY_MEDIUM, &handle_left},
// {"Look", 7, PRIORITY_MEDIUM, &handle_look},
// {"Inventory", 1, PRIORITY_HIGH, &handle_inventory},
// {"Broadcast", 7, PRIORITY_MEDIUM, &handle_broadcast},
// {"Connect_nbr", 1, PRIORITY_HIGH, &handle_connect_nbr},
// {"Fork", 42, PRIORITY_LOW, &handle_fork},
// {"Eject", 7, PRIORITY_MEDIUM, &handle_eject},
// {"Take", 7, PRIORITY_MEDIUM, &handle_take},
// {"Set", 7, PRIORITY_MEDIUM, &handle_set},
// {"Incantation", 300, PRIORITY_LOW, &handle_incantation},


/* This function loops thru the cmds to find the correct func */
const command_info_t *find_command_info(char *command)
{
    for (int i = 0; COMMANDS[i].command != NULL; i++) {
        if (strcmp(COMMANDS[i].command, command) == 0) {
            return &COMMANDS[i];
        }
    }
    return NULL;
}
