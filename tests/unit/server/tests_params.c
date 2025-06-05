/*
** EPITECH PROJECT, 2025
** Unit Tests
** File description:
** Server :: Params
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "zappy.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

/* Server :: Params */

Test(params, check_port_valid) {
    params_t params = {0};
    bool result = check_port("-p", "8080", &params);

    cr_assert_eq(result, true);
    cr_assert_eq(params.port, 8080);
}

Test(params, check_port_invalid) {
    params_t params = {0};
    bool result = check_port("-p", "abc", &params);

    cr_assert_eq(result, false);
}

Test(params, check_width_valid) {
    params_t params = {0};
    bool result = check_width("-x", "10", &params);

    cr_assert_eq(result, true);
    cr_assert_eq(params.x, 10);
}

Test(params, check_height_valid) {
    params_t params = {0};
    bool result = check_height("-y", "15", &params);

    cr_assert_eq(result, true);
    cr_assert_eq(params.y, 15);
}

Test(params, check_client_valid) {
    params_t params = {0};
    bool result = check_client("-c", "5", &params);

    cr_assert_eq(result, true);
    cr_assert_eq(params.nb_client, 5);
}

Test(params, check_freq_valid) {
    params_t params = {0};
    bool result = check_freq("-f", "100", &params);

    cr_assert_eq(result, true);
    cr_assert_eq(params.freq, 100);
}

Test(params, check_args_valid) {
    char *argv[] = {"zappy_server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "3", "-f", "100"};
    int argc = 14;

    params_t *params = check_args(argc, argv);

    cr_assert_not_null(params);
    cr_assert_eq(params->port, 8080);
    cr_assert_eq(params->x, 10);
    cr_assert_eq(params->y, 10);
    cr_assert_eq(params->nb_client, 3);
    cr_assert_eq(params->freq, 100);
    cr_assert_eq(params->nb_team, 2);

    free_params(params);
}

Test(params, check_args_missing_port) {
    char *argv[] = {"zappy_server", "-x", "10", "-y", "10", "-n", "team1", "-c", "3", "-f", "100"};
    int argc = 11;

    params_t *params = check_args(argc, argv);

    cr_assert_null(params);
}

Test(check_args, check_args_invalid, .init = redirect_all_std) {
    char *argv[] = {"./server"};
    int argc = 1;

    cr_assert_null(check_args(argc, argv), "Expected invalid arguments check to pass.");
}

Test(check_args, check_args_invalid_team_name, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "", "", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_null(check_args(argc, argv), "Expected invalid team name check to pass.");
}

Test(check_args, check_name_empty_string, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_null(check_args(argc, argv), "Expected empty team name to be handled gracefully.");
}

/* Server :: Params :: Free params */

Test(free_params, free_params_null, .init = redirect_all_std) {
    void *result = free_params(NULL);
    cr_assert_null(result, "Expected free_params with NULL to return NULL.");
}

Test(free_params, free_params_valid, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));
    params->nb_team = 2;
    params->teams = malloc(3 * sizeof(char *));
    params->teams[0] = strdup("team1");
    params->teams[1] = strdup("team2");
    params->teams[2] = NULL;

    void *result = free_params(params);
    cr_assert_null(result, "Expected free_params to return NULL after freeing.");
}
