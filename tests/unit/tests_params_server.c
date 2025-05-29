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

    cr_assert_eq(check_args(argc, argv), 0, "Expected valid arguments check to pass.");
}

Test(check_args, check_args_invalid, .init = redirect_all_std) {
    char *argv[] = {"./server"};
    int argc = 1;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid arguments check to pass.");
}

/* Server :: Params :: Port */

Test(check_args, check_port_valid_mini, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "1024", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 0, "Expected valid port check to pass.");
}

Test(check_args, check_port_valid_maxi, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "65535", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 0, "Expected valid port check to pass.");
}

Test(check_args, check_port_null_flag, .init = redirect_all_std) {
    char *argv[] = {"./server", NULL, "42", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid port check to pass.");
}

Test(check_args, check_port_null_port, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", NULL, "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid port check to pass.");
}

Test(check_args, check_port_invalid_smaller, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "42", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid port check to pass.");
}

Test(check_args, check_port_invalid_bigger, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "100000", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid port check to pass.");
}

Test(check_args, check_port_invalid_flag, .init = redirect_all_std) {
    char *argv[] = {"./server", "-z", "4242", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid port check to pass.");
}

/* Server :: Params :: Width */

Test(check_args, check_width_valid_hundred, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "100", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 0, "Expected valid width check to pass.");
}

Test(check_args, check_width_valid_thousand, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "1000", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 0, "Expected valid width check to pass.");
}

Test(check_args, check_width_invalid_zero, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "0", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid width check to pass.");
}

Test(check_args, check_width_invalid_negative, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "-10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid width check to pass.");
}

Test(check_args, check_width_null_flag, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", NULL, "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid width check to pass.");
}

Test(check_args, check_width_null_size, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", NULL, "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid width check to pass.");
}

Test(check_args, check_width_invalid_flag, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-z", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid width check to pass.");
}

/* Server :: Params :: Height */

Test(check_args, check_height_valid_hundred, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "100", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 0, "Expected valid height check to pass.");
}

Test(check_args, check_height_valid_thousand, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "1000", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 0, "Expected valid height check to pass.");
}

Test(check_args, check_height_invalid_zero, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "0", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid height check to pass.");
}

Test(check_args, check_height_invalid_negative, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "-10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid height check to pass.");
}

Test(check_args, check_height_null_flag, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", NULL, "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid height check to pass.");
}

Test(check_args, check_height_null_size, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", NULL, "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid height check to pass.");
}

Test(check_args, check_height_invalid_flag, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-z", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid height check to pass.");
}

/* Server :: Params :: Name */

Test(check_args, check_name_valid, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 0, "Expected valid name check to pass.");
}

Test(check_args, check_name_invalid_empty, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "", "", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid name check to pass.");
}

Test(check_args, check_name_invalid_null, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", NULL, NULL, "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid name check to pass.");
}

Test(check_args, check_name_invalid_flag, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-z", "team1", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid name check to pass.");
}

Test(check_args, check_name_many_team, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "team3", "team4", "-c", "5", "-f", "50"};
    int argc = 16;

    cr_assert_eq(check_args(argc, argv), 0, "Expected valid multiple team names check to pass.");
}

Test(check_args, check_name_one_team, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "-c", "5", "-f", "50"};
    int argc = 13;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid single team name check to pass.");
}

/* Server :: Params :: Client */

Test(check_args, check_client_valid_mini, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "1", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 0, "Expected valid client number check to pass.");
}

Test(check_args, check_client_valid_many, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "100", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 0, "Expected valid client number check to pass.");
}

Test(check_args, check_client_invalid_zero, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "0", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid client number check to pass.");
}

Test(check_args, check_client_invalid_negative, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "-5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid client number check to pass.");
}

Test(check_args, check_client_null_flag, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", NULL, "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid client number check to pass.");
}

Test(check_args, check_client_null_value, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", NULL, "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid client number check to pass.");
}

Test(check_args, check_client_invalid_flag, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-z", "5", "-f", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid client number check to pass.");
}

Test(check_args, check_client_many_team, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "team3", "team4", "-c", "5", "-f", "50"};
    int argc = 16;

    cr_assert_eq(check_args(argc, argv), 0, "Expected valid client number check to pass.");
}

/* Server :: Params :: Frequency */

Test(check_args, check_freq_valid_mini, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "1"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 0, "Expected valid frequency check to pass.");
}

Test(check_args, check_freq_valid_maxi, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "200"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 0, "Expected valid frequency check to pass.");
}

Test(check_args, check_freq_invalid_zero, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "0"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid frequency check to pass.");
}

Test(check_args, check_freq_invalid_negative, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "-50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid frequency check to pass.");
}

Test(check_args, check_freq_invalid_bigger, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "201"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid frequency check to pass.");
}

Test(check_args, check_freq_null_flag, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", NULL, "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid frequency check to pass.");
}

Test(check_args, check_freq_null_value, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", NULL};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid frequency check to pass.");
}

Test(check_args, check_freq_invalid_flag, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-z", "50"};
    int argc = 14;

    cr_assert_eq(check_args(argc, argv), 84, "Expected invalid frequency check to pass.");
}

Test(check_args, check_freq_many_team, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "team3", "team4", "-c", "5", "-f", "50"};
    int argc = 16;

    cr_assert_eq(check_args(argc, argv), 0, "Expected valid frequency check to pass.");
}
