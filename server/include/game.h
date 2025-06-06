/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** game
*/

#include "buffer.h"
#include <time.h>
#include <pthread.h>

#ifndef GAME_H_
    #define GAME_H_

typedef struct action_request_s action_request_t;
typedef struct action_queue_s action_queue_t;
typedef struct player_s player_t;

/* Definition of the directions */
typedef enum direction_e {
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4
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


/* This enum defines the priority of the action in the queue */
typedef enum action_priority_e {
    PRIORITY_CRITICAL = 0,
    PRIORITY_HIGH = 1,
    PRIORITY_MEDIUM = 2,
    PRIORITY_LOW = 3
} action_priority_t;

/* This strucuture allows use to define a 'queue' of the requests */
typedef struct action_queue_s {
    action_request_t *head;
    action_request_t *tail;
    int count;
    pthread_mutex_t mutex;
} action_queue_t;


typedef struct egg_s {
    int id; /* Id of the egg */
    int posX;
    int posY;
    char *teamName;  /* Name of the team that laid it */
    int idLayer;  /* Id of the player that layed it */
    bool isHatched;
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
    char *team;
    /* New aditions for the smart pollin */
    action_queue_t *pending_actions;
    time_t last_action_time;
    bool is_busy;
    int remaining_cooldown;

    struct player_s *next;
} player_t;

/* This structure define the request strut */
typedef struct action_request_s {
    char *command;
    time_t timestamp;
    int time_limit;  // in game ticks (7/f, 42/f, etc.)
    action_priority_t priority;
    player_t *player;
    struct action_request_s *next;
} action_request_t;

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
