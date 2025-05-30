/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Checkers tests
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include<stdio.h>
#include <stdlib.h>

#include "zappy.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

/* Server :: Checkers :: Port */

Test(check_port, check_port_valid, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_port("-p", "4242", params), 1, "Expected valid port check to pass.");
    free(params);
}

Test(check_port, check_port_valid_mini, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_port("-p", "1024", params), 1, "Expected valid port check to pass.");
    free(params);
}

Test(check_port, check_port_valid_maxi, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_port("-p", "65535", params), 1, "Expected valid port check to pass.");
    free(params);
}

Test(check_port, check_port_null_flag, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_port(NULL, "42", params), 0, "Expected invalid port check to pass.");
    free(params);
}

Test(check_port, check_port_null_port, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_port("-p", NULL, params), 0, "Expected invalid port check to pass.");
    free(params);
}

Test(check_port, check_port_invalid_smaller, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_port("-p", "42", params), 0, "Expected invalid port check to pass.");
    free(params);
}

Test(check_port, check_port_invalid_bigger, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_port("-p", "100000", params), 0, "Expected invalid port check to pass.");
    free(params);
}

Test(check_port, check_port_invalid_flag, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_port("-z", "4242", params), 0, "Expected invalid port check to pass.");
    free(params);
}

Test(check_port, check_port_invalid_no_params, .init = redirect_all_std) {
    params_t *params = NULL;

    cr_assert_eq(check_port("-p", "4242", params), 0, "Expected invalid port check to pass.");
}

/* Server :: Checkers :: Width */

Test(check_width, check_width_valid, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_width("-x", "10", params), 1, "Expected valid width check to pass.");
    free(params);
}

Test(check_width, check_width_valid_thousand, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_width("-x", "1000", params), 1, "Expected valid width check to pass.");
    free(params);
}

Test(check_width, check_width_invalid_zero, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_width("-x", "0", params), 0, "Expected invalid width check to pass.");
    free(params);
}

Test(check_width, check_width_invalid_negative, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_width("-x", "-10", params), 0, "Expected invalid width check to pass.");
    free(params);
}

Test(check_width, check_width_null_flag, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_width(NULL, "10", params), 0, "Expected invalid width check to pass.");
    free(params);
}

Test(check_width, check_width_null_size, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_width("-x", NULL, params), 0, "Expected invalid width check to pass.");
    free(params);
}

Test(check_width, check_width_invalid_flag, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_width("-z", "10", params), 0, "Expected invalid width check to pass.");
    free(params);
}

Test(check_width, check_width_invalid_no_params, .init = redirect_all_std) {
    params_t *params = NULL;

    cr_assert_eq(check_width("-x", "10", params), 0, "Expected invalid width check to pass.");
}

/* Server :: Checkers :: Height */

Test(check_height, check_height_valid, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_height("-y", "10", params), 1, "Expected valid height check to pass.");
    free(params);
}

Test(check_height, check_height_valid_thousand, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_height("-y", "1000", params), 1, "Expected valid height check to pass.");
    free(params);
}

Test(check_height, check_height_invalid_zero, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_height("-y", "0", params), 0, "Expected invalid height check to pass.");
    free(params);
}

Test(check_height, check_height_invalid_negative, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_height("-y", "-10", params), 0, "Expected invalid height check to pass.");
    free(params);
}

Test(check_height, check_height_null_flag, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_height(NULL, "10", params), 0, "Expected invalid height check to pass.");
    free(params);
}

Test(check_height, check_height_null_size, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_height("-y", NULL, params), 0, "Expected invalid height check to pass.");
    free(params);
}

Test(check_height, check_height_invalid_flag, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_height("-z", "10", params), 0, "Expected invalid height check to pass.");
    free(params);
}

Test(check_height, check_height_invalid_no_params, .init = redirect_all_std) {
    params_t *params = NULL;

    cr_assert_eq(check_height("-y", "10", params), 0, "Expected invalid height check to pass.");
}

/* Server :: Checkers :: Client */

Test(check_client, check_client_valid, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_client("-c", "5", params), 1, "Expected valid client check to pass.");
    free(params);
}

Test(check_client, check_client_valid_many, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_client("-c", "100", params), 1, "Expected valid client check to pass.");
    free(params);
}

Test(check_client, check_client_invalid_zero, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_client("-c", "0", params), 0, "Expected invalid client check to pass.");
    free(params);
}

Test(check_client, check_client_invalid_negative, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_client("-c", "-5", params), 0, "Expected invalid client check to pass.");
    free(params);
}

Test(check_client, check_client_null_flag, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_client(NULL, "5", params), 0, "Expected invalid client check to pass.");
    free(params);
}

Test(check_client, check_client_null_value, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_client("-c", NULL, params), 0, "Expected invalid client check to pass.");
    free(params);
}

Test(check_client, check_client_invalid_flag, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_client("-z", "5", params), 0, "Expected invalid client check to pass.");
    free(params);
}

Test(check_client, check_client_invalid_no_params, .init = redirect_all_std) {
    params_t *params = NULL;

    cr_assert_eq(check_client("-c", "5", params), 0, "Expected invalid client check to pass.");
}

/* Server :: Checkers :: Frequency */

Test(check_freq, check_freq_valid, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_freq("-f", "50", params), 1, "Expected valid frequency check to pass.");
    free(params);
}

Test(check_freq, check_freq_valid_mini, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_freq("-f", "1", params), 1, "Expected valid frequency check to pass.");
    free(params);
}

Test(check_freq, check_freq_valid_maxi, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_freq("-f", "200", params), 1, "Expected valid frequency check to pass.");
    free(params);
}

Test(check_freq, check_freq_invalid_zero, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_freq("-f", "0", params), 0, "Expected invalid frequency check to pass.");
    free(params);
}

Test(check_freq, check_freq_invalid_negative, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_freq("-f", "-50", params), 0, "Expected invalid frequency check to pass.");
    free(params);
}

Test(check_freq, check_freq_invalid_bigger, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_freq("-f", "201", params), 0, "Expected invalid frequency check to pass.");
    free(params);
}

Test(check_freq, check_freq_null_flag, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_freq(NULL, "50", params), 0, "Expected invalid frequency check to pass.");
    free(params);
}

Test(check_freq, check_freq_null_value, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_freq("-f", NULL, params), 0, "Expected invalid frequency check to pass.");
    free(params);
}

Test(check_freq, check_freq_invalid_flag, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));

    cr_assert_eq(check_freq("-z", "50", params), 0, "Expected invalid frequency check to pass.");
    free(params);
}

Test(check_freq, check_freq_invalid_no_params, .init = redirect_all_std) {
    params_t *params = NULL;

    cr_assert_eq(check_freq("-f", "50", params), 0, "Expected invalid frequency check to pass.");
}
