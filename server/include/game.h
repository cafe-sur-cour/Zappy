/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** game
*/

#include <time.h>

#ifndef GAME_H_
    #define GAME_H_

/* Definition of the directions */
typedef enum direction_e {
    NORTH,
    SOUTH,
    EAST,
    WEST
} direction_e;

/* definintion od the different element on the map */
typedef enum crystal_e {
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME
} crystal_e;

/* Struct that "counts" the current stat of the player */
typedef struct lives_s {
    int freq;
    int nbFood;
    time_t startRefresh;
    time_t endRefresh;
} lives_t;

/* Struct defining the invetory of the player */
typedef struct inventory_s {
    int nbLinemate;
    int nbDeraumere;
    int nbSibur;
    int nbMendiane;
    int nbPhiras;
    int nbThystame;
} inventory_t;

/* Player struct */
typedef struct player_s {
    int id; /* This is equal to the current FD */
    int level;
    int posX;
    int posY;
    bool isAlive;
    direction_e direction;
    inventory_t *inventory;
    lives_t *lives;
    struct player_s *next;
} player_t;

/* Team Strcut */
typedef struct team_s {
    char *name;
    int nbPlayers;
    int nbPlayerAlive;
    player_t *players;
    struct team_s *next;
} team_t;

/* Ressources, and there pos on the map */
typedef struct ressources_s {
    crystal_e type;
    int posX;
    int posY;
    struct ressources_s *next;
} ressources_t;

/* Map struct */
typedef struct map_s {
    int width;
    int heigt;
    team_t *teams;
    ressources_t *ressources;
} map_t;

#endif /* !GAME_H_ */
