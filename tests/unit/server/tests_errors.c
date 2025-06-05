/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** Errors.c tests
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <unistd.h>

#include "network.h"

#include "zappy.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

/* Server :: Errors :: Helper */

Test(helper, helper_valid, .init = redirect_all_std) {
    cr_assert_eq(helper(), 84, "Expected helper function to return 84.");
}

/* Server :: Errors :: Error Message */

Test(error_message, error_message_valid, .init = redirect_all_std) {
    FILE *tmp_file = fopen("/tmp/criterion_test_output.txt", "w");
    cr_assert_not_null(tmp_file, "Failed to create temporary file");

    FILE *original_stdout = stdout;
    stdout = tmp_file;

    error_message("This is a test error message.");

    stdout = original_stdout;
    fclose(tmp_file);

    tmp_file = fopen("/tmp/criterion_test_output.txt", "r");
    cr_assert_not_null(tmp_file, "Failed to reopen temporary file");

    char buffer[1000];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, tmp_file);
    buffer[bytes_read] = '\0';
    fclose(tmp_file);

    unlink("/tmp/criterion_test_output.txt");

    const char *expected = "\033[1;31m[SERVER]\033[0m \033[0;31mThis is a test error message.\033[0m\n";
    cr_assert_str_eq(buffer, expected, "Error message output doesn't match expected format");
}

Test(errors, error_message_output, .init = cr_redirect_stderr)
{
    error_message("Test error message");

}

Test(errors, write_message_valid_fd, .init = cr_redirect_stdout)
{
    int result = write_message(1, "Test message");
    
    cr_assert_geq(result, 0);
}

Test(errors, write_message_invalid_fd,  .init = cr_redirect_stderr)
{
    int result = write_message(-1, "Test message");
    
    cr_assert_eq(result, -1);
}

Test(errors, helper_function, .init = cr_redirect_stdout)
{
    int result = helper();
    
    cr_assert_eq(result, 84);
}
