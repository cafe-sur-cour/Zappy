/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** queu_hanlder
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* This sends the end message "ok" or "ko" and sets the cooldown of player */
void execute_action(player_t *player, action_request_t *action, zappy_t *zappy)
{
    const command_info_t *cmd_info = find_command_info(action->command);
    int result = 0;

    if (!cmd_info) {
        write_message(player->network->fd, "ko\n");
        return;
    }
    player->remaining_cooldown = action->time_limit;
    if (cmd_info->base_time >= 42)
        player->is_busy = true;
    result = cmd_info->handler(player, action->command, zappy);
    if (result == 0)
        write_message(player->network->fd, "ok\n");
    else if (result == -2)
        write_message(player->network->fd, "Elevation underway\n");
    else
        write_message(player->network->fd, "ko\n");
}

/* This functions queue the actions and "lock" it tosafely acces memory */
void queue_action(player_t *player, char *command, zappy_t *zappy)
{
    action_request_t *action = NULL;

    (void)zappy;
    if (!player->pending_actions)
        return;
    action = create_action_request(command, player, zappy->params->freq);
    if (!action)
        return;
    pthread_mutex_lock(&player->pending_actions->mutex);
    if (!player->pending_actions->head ||
        action->priority < player->pending_actions->head->priority) {
        action->next = player->pending_actions->head;
        player->pending_actions->head = action;
        if (!player->pending_actions->tail)
            player->pending_actions->tail = action;
    } else
        insert_action_by_priority(player->pending_actions, action);
    player->pending_actions->count++;
    pthread_mutex_unlock(&player->pending_actions->mutex);
}

/* This function free's the queue */
void free_action_queue(action_queue_t *queue)
{
    action_request_t *current = NULL;
    action_request_t *next = NULL;

    if (!queue)
        return;
    pthread_mutex_lock(&queue->mutex);
    current = queue->head;
    while (current) {
        next = current->next;
        free_action_request(current);
        current = next;
    }
    pthread_mutex_unlock(&queue->mutex);
    pthread_mutex_destroy(&queue->mutex);
    free(queue);
}
