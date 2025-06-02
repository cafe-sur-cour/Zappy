/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server tests
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "zappy.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

/* Server :: Server :: Init server */

Test(init_server, init_server_valid, .init = redirect_all_std) {
    char *argv[] = {"./zappy_server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    server_t *server = init_server(argc, argv);
    cr_assert_not_null(server, "Expected server initialization to succeed.");
    free_server(server);
}

Test(init_server, init_server_invalid, .init = redirect_all_std) {
    char *argv[] = {"./zappy_server"};
    int argc = 1;

    cr_assert_null(init_server(argc, argv), "Expected server initialization to fail with insufficient arguments.");
}

/* Server :: Server :: Free server */

Test(free_server, free_server_null, .init = redirect_all_std) {
    cr_assert_null(free_server(NULL), "Expected free_server to handle NULL gracefully.");
}

Test(free_server, free_server_valid, .init = redirect_all_std) {
    char *argv[] = {"./zappy_server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    server_t *server = init_server(argc, argv);
    cr_assert_null(free_server(server), "Expected free_server to return NULL after freeing.");
}
