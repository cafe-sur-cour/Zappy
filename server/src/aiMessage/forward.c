/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server :: AI Message :: Forward
*/

#include "zappy.h"

static void move_top(player_t *player, int n)
{
    player->posY -= 1;
    if (player->posY < 0)
        player->posY = n - 1;
}

static void move_bottom(player_t *player, int n)
{
    player->posY += 1;
    if (player->posY >= n)
        player->posY = 0;
}

static void move_left(player_t *player, int n)
{
    player->posX -= 1;
    if (player->posX < 0)
        player->posX = n - 1;
}

static void move_right(player_t *player, int n)
{
    player->posX += 1;
    if (player->posX >= n)
        player->posX = 0;
}

int forward_message(player_t *player, params_t *params)
{
    direction_t direction = player->direction;
    direction_t directions[4] = {NORTH, EAST, SOUTH, WEST};
    void (*move_functions[4])(player_t *, int) = {
        &move_top, &move_right, &move_bottom, &move_left};

    for (int i = 0; i < 4; i++) {
        if (direction == directions[i]) {
            move_functions[i](player, params->y);
            break;
        }
    }
    if (write_message(player->fd, "ok\n") == 1)
        return -1;
    return 0;
}
