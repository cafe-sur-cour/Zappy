/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** buffer
*/

#ifndef BUFFER_H_
    #define BUFFER_H_

    #define BUFFER_SIZE 1024

#include <stddef.h>

typedef struct buffer_s {
    char data[BUFFER_SIZE];
    int head;
    int tail;
    int full;
} buffer_t;

/* buffer.c */
int advance(int idx);
void cb_write(buffer_t *cb, char c);
int cb_getline(buffer_t *cb, char *line, int max_len);

#endif /* !BUFFER_H_ */
