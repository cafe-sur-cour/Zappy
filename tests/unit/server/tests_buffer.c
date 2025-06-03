/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_buffer
*/


#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include<stdio.h>
#include <stdlib.h>

#include "buffer.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(buffer, test_advance_function)
{
    cr_assert_eq(advance(0), 1);
    cr_assert_eq(advance(BUFFER_SIZE - 1), 0);
    cr_assert_eq(advance(512), 513);
}

Test(buffer, test_buffer_initialization)
{
    buffer_t buffer = {0};
    
    cr_assert_eq(buffer.head, 0);
    cr_assert_eq(buffer.tail, 0);
    cr_assert_eq(buffer.full, 0);
}

Test(buffer, test_cb_write_single_char)
{
    buffer_t buffer = {0};
    
    cb_write(&buffer, 'A');
    
    cr_assert_eq(buffer.data[0], 'A');
    cr_assert_eq(buffer.head, 1);
    cr_assert_eq(buffer.tail, 0);
    cr_assert_eq(buffer.full, 0);
}

Test(buffer, test_cb_write_multiple_chars)
{
    buffer_t buffer = {0};
    char test_string[] = "Hello";
    
    for (int i = 0; test_string[i]; i++) {
        cb_write(&buffer, test_string[i]);
    }
    
    cr_assert_eq(buffer.head, 5);
    cr_assert_eq(buffer.tail, 0);
    cr_assert_eq(buffer.full, 0);
    for (int i = 0; i < 5; i++) {
        cr_assert_eq(buffer.data[i], test_string[i]);
    }
}

Test(buffer, test_cb_write_buffer_overflow)
{
    buffer_t buffer = {0};
    
    // Fill the entire buffer
    for (int i = 0; i < BUFFER_SIZE; i++) {
        cb_write(&buffer, 'A' + (i % 26));
    }
    
    cr_assert_eq(buffer.full, 1);
    cr_assert_eq(buffer.head, buffer.tail);
    
    // Write one more character to test overflow
    cb_write(&buffer, 'X');
    cr_assert_eq(buffer.data[0], 'X');
    cr_assert_eq(buffer.tail, 1);
}

Test(buffer, test_cb_getline_simple_line)
{
    buffer_t buffer = {0};
    char line[100];
    char test_data[] = "Hello World\n";
    
    for (int i = 0; test_data[i]; i++) {
        cb_write(&buffer, test_data[i]);
    }
    
    int result = cb_getline(&buffer, line, 100);
    
    cr_assert_eq(result, 12);
    cr_assert_str_eq(line, "Hello World\n");
    cr_assert_eq(buffer.full, 0);
}

Test(buffer, test_cb_getline_empty_buffer)
{
    buffer_t buffer = {0};
    char line[100];
    
    int result = cb_getline(&buffer, line, 100);
    
    cr_assert_eq(result, 0);
}

Test(buffer, test_cb_getline_no_newline)
{
    buffer_t buffer = {0};
    char line[100];
    char test_data[] = "Hello";
    
    for (int i = 0; test_data[i]; i++) {
        cb_write(&buffer, test_data[i]);
    }
    
    int result = cb_getline(&buffer, line, 100);
    
    cr_assert_eq(result, 0);
}

Test(buffer, test_cb_getline_max_length_reached)
{
    buffer_t buffer = {0};
    char line[10];
    char test_data[] = "This is a very long line without newline";
    
    for (int i = 0; test_data[i]; i++) {
        cb_write(&buffer, test_data[i]);
    }
    
    int result = cb_getline(&buffer, line, 10);
    
    cr_assert_eq(result, 9);
    cr_assert_str_eq(line, "This is a");
    cr_assert_eq(line[9], '\0');
}

Test(buffer, test_cb_getline_multiple_lines)
{
    buffer_t buffer = {0};
    char line[100];
    char test_data[] = "First line\nSecond line\n";
    
    for (int i = 0; test_data[i]; i++) {
        cb_write(&buffer, test_data[i]);
    }
    
    int result1 = cb_getline(&buffer, line, 100);
    cr_assert_eq(result1, 11);
    cr_assert_str_eq(line, "First line\n");
    
    int result2 = cb_getline(&buffer, line, 100);
    cr_assert_eq(result2, 12);
    cr_assert_str_eq(line, "Second line\n");
}

Test(buffer, test_buffer_wrap_around)
{
    buffer_t buffer = {0};
    char line[100];
    
    // Fill buffer almost to capacity
    for (int i = 0; i < BUFFER_SIZE - 10; i++) {
        cb_write(&buffer, 'A');
    }
    
    // Read some data to move tail
    buffer.tail = BUFFER_SIZE - 20;
    buffer.full = 0;
    
    // Write data that wraps around
    char test_data[] = "Test\n";
    for (int i = 0; test_data[i]; i++) {
        cb_write(&buffer, test_data[i]);
    }
    
    int result = cb_getline(&buffer, line, 100);
    cr_assert_gt(result, 0);
}

Test(buffer, test_cb_getline_with_full_buffer, .init = redirect_all_std)
{
    buffer_t buffer = {0};
    char line[100];
    
    // Fill the buffer completely
    for (int i = 0; i < BUFFER_SIZE - 1; i++) {
        cb_write(&buffer, 'A' + (i % 26));
    }
    cb_write(&buffer, '\n');
    
    int result = cb_getline(&buffer, line, 100);
    cr_assert_gt(result, 0);
}
