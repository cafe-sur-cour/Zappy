/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** game
*/

#include "buffer.h"
#include <time.h>

#ifndef GAME_H_
    #define GAME_H_

/* Definition of the directions */
typedef enum direction_e {
    NORTH,
    EAST,
    SOUTH,
    WEST
} direction_t;

/* definintion od the different element on the map */
typedef enum crystal_e {
    FOOD,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME
} crystal_t;


typedef struct egg_s {
    int id; /* Id of the egg */
    int posX;
    int posY;
    char *teamName;  /* Name of the team that laid it */
    int idLayer;  /* Id of the player that layed it */
    struct egg_s *next;
} egg_t;

/* Struct that "handles" the network element */
typedef struct network_s {
    int fd;
    buffer_t *buffer;
} network_t;

/* Struct defining the inventory of tiles and players */
typedef struct inventory_s {
    int nbFood;
    int nbLinemate;
    int nbDeraumere;
    int nbSibur;
    int nbMendiane;
    int nbPhiras;
    int nbThystame;
} inventory_t;


/* Player struct */
typedef struct player_s {
    int id;
    network_t *network;
    int level;
    int posX;
    int posY;
    direction_t direction;
    inventory_t *inventory;
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


/* Structure that holds the size and array of tiles */
typedef struct map_t {
    int width;
    int height;
    egg_t *currentEggs;  /* List of current eggs */
    inventory_t **tiles;  /* Here we call inv for the tile*/
} map_t;


/* Map struct */
typedef struct game_s {
    team_t *teams;
    map_t *map;
} game_t;

#endif /* !GAME_H_ */
