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

Test(check_args, check_args_valid, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_not_null(check_args(argc, argv), "Expected valid arguments check to pass.");
}

Test(check_args, check_args_invalid, .init = redirect_all_std) {
    char *argv[] = {"./server"};
    int argc = 1;

    cr_assert_null(check_args(argc, argv), "Expected invalid arguments check to pass.");
}

Test(check_args, check_args_invalid_team_flag, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-z", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_null(check_args(argc, argv), "Expected invalid team flag check to pass.");
}

Test(check_args, check_args_invalid_team_name, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "", "", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_null(check_args(argc, argv), "Expected invalid team name check to pass.");
}

Test(check_args, check_name_invalid_flag, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-z", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_null(check_args(argc, argv), "Expected invalid name flag check to pass.");
}

Test(check_args, check_name_empty_string, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_null(check_args(argc, argv), "Expected empty team name to be handled gracefully.");
}

Test(check_args, check_simple_flag_missing_port_value, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 13;

    cr_assert_null(check_args(argc, argv), "Expected missing port value check to pass.");
}

Test(check_args, check_simple_flag_end_of_argv, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f"};
    int argc = 13;

    cr_assert_null(check_args(argc, argv), "Expected end of argv check to pass.");
}

Test(check_args, check_simple_flag_invalid_port, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "42", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_null(check_args(argc, argv), "Expected invalid port value to be handled.");
}

Test(check_args, check_names_flag_missing_value, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "-c", "5", "-f", "50"};
    int argc = 12;

    cr_assert_null(check_args(argc, argv), "Expected missing names flag value to be handled.");
}

Test(check_args, check_args_insufficient_argc, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5"};
    int argc = 12;

    cr_assert_null(check_args(argc, argv), "Expected insufficient argc check to pass.");
}

Test(check_args, check_args_memory_allocation_null, .init = redirect_all_std) {
    char *argv[] = {"./server"};
    int argc = 1;

    cr_assert_null(check_args(argc, argv), "Expected memory allocation failure to be handled.");
}

Test(check_args, check_names_flag_insufficient_args, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n"};
    int argc = 8;

    cr_assert_null(check_args(argc, argv), "Expected insufficient arguments for names flag to fail.");
}

Test(check_args, count_names_immediate_flag, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "-c", "5", "-f", "50"};
    int argc = 12;

    cr_assert_null(check_args(argc, argv), "Expected count_names with immediate flag to be handled.");
}

Test(check_args, check_args_invalid_flag_placement, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-z", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_null(check_args(argc, argv), "Expected invalid flag placement to be handled.");
}

Test(check_args, check_name_multiple_teams_success, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "team3", "-c", "5", "-f", "50"};
    int argc = 15;

    cr_assert_not_null(check_args(argc, argv), "Expected multiple teams to be handled correctly.");
}

Test(check_args, check_args_is_ok_false, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "42", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_null(check_args(argc, argv), "Expected is_ok false path to return NULL.");
}

Test(check_args, check_args_all_valid, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "4242", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_not_null(check_args(argc, argv), "Expected all valid arguments to pass.");
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
