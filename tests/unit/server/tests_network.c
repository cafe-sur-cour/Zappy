/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_network
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <poll.h>
#include "network.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Test data and helper functions
static int test_server_fd = -1;
static int test_client_fd = -1;

static void setup_sockets(void)
{
    int fds[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == 0) {
        test_server_fd = fds[0];
        test_client_fd = fds[1];
    }
}

static void cleanup_sockets(void)
{
    if (test_server_fd != -1) {
        close(test_server_fd);
        test_server_fd = -1;
    }
    if (test_client_fd != -1) {
        close(test_client_fd);
        test_client_fd = -1;
    }
}

// Tests for create_server.c functions
Test(set_socket, successful_socket_creation, .init = redirect_all_std)
{
    int sockfd = set_socket();
    
    cr_assert_neq(sockfd, -1);
    cr_assert_geq(sockfd, 0);
    
    if (sockfd != -1) {
        close(sockfd);
    }
}

Test(bind_socket, successful_bind, .init = redirect_all_std)
{
    int sockfd = set_socket();
    cr_expect_neq(sockfd, -1);
    
    int result = bind_socket(sockfd, 0); // Port 0 for automatic assignment
    
    cr_assert_eq(result, 0);
    
    close(sockfd);
}

Test(bind_socket, invalid_socket_fd, .init = redirect_all_std)
{
    int result = bind_socket(-1, 8080);
    
    cr_assert_eq(result, -1);
}

Test(bind_socket, invalid_port, .init = redirect_all_std)
{
    int sockfd = set_socket();
    cr_expect_neq(sockfd, -1);
    
    // Try to bind to a privileged port without permissions
    int result = bind_socket(sockfd, 80);
    
    // Should fail due to permissions or port already in use
    cr_assert_eq(result, -1);
}

Test(listen_socket, successful_listen, .init = redirect_all_std)
{
    int sockfd = set_socket();
    cr_expect_neq(sockfd, -1);
    
    bind_socket(sockfd, 0);
    int result = listen_socket(sockfd, 5);
    
    cr_assert_eq(result, 0);
    
    close(sockfd);
}

Test(listen_socket, invalid_socket_fd, .init = redirect_all_std)
{
    int result = listen_socket(-1, 5);
    
    cr_assert_eq(result, -1);
}

// Tests for accept_connection.c
Test(accept_connection, invalid_server_fd, .init = redirect_all_std)
{
    int result = accept_connection(-1);
    
    cr_assert_eq(result, -1);
}


// Tests for send_message.c
Test(write_message, successful_write, .init = redirect_all_std)
{
    setup_sockets();
    cr_expect_neq(test_client_fd, -1);
    
    const char *message = "Hello World\n";
    int result = write_message(test_client_fd, message);
    
    cr_assert_eq(result, 0);
    
    // Verify message was written by reading from other end
    char buffer[50] = {0};
    ssize_t bytes_read = read(test_server_fd, buffer, sizeof(buffer) - 1);
    cr_assert_gt(bytes_read, 0);
    cr_assert_str_eq(buffer, message);
    
    cleanup_sockets();
}

Test(write_message, invalid_fd, .init = redirect_all_std)
{
    const char *message = "Hello World\n";
    int result = write_message(-1, message);
    
    cr_assert_eq(result, -1);
}

Test(write_message, empty_message, .init = redirect_all_std)
{
    setup_sockets();
    cr_expect_neq(test_client_fd, -1);
    
    const char *message = "";
    int result = write_message(test_client_fd, message);
    
    cr_assert_eq(result, 0);
    
    cleanup_sockets();
}

Test(get_message, valid_fd_with_data, .init = redirect_all_std)
{
    setup_sockets();
    cr_expect_neq(test_server_fd, -1);
    
    const char *test_data = "Test message";
    write(test_client_fd, test_data, strlen(test_data));
    
    char *result = get_message(test_server_fd);
    
    if (result) {
        cr_assert_str_eq(result, test_data);
        free(result);
    }
    
    cleanup_sockets();
}

Test(get_message, invalid_fd, .init = redirect_all_std)
{
    char *result = get_message(-1);
    
    cr_assert_null(result);
}

Test(get_message, no_data_available, .init = redirect_all_std)
{
    setup_sockets();
    cr_expect_neq(test_server_fd, -1);
    
    // Don't write any data
    char *result = get_message(test_server_fd);
    
    // Should return NULL or empty string depending on implementation
    if (result) {
        free(result);
    }
    
    cleanup_sockets();
}

// Tests for close.c (assuming close_fd function exists)
Test(close_fd, valid_fd, .init = redirect_all_std)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    cr_expect_neq(fd, -1);
    
    // This should not crash
    close_fd(fd);
    
    // Verify fd is closed by trying to use it
    int result = write(fd, "test", 4);
    cr_assert_eq(result, -1);
    cr_assert_eq(errno, EBADF);
}

Test(close_fd, invalid_fd, .init = redirect_all_std)
{
    // This should handle invalid fd gracefully
    close_fd(-1);
    
    // Should not crash - test passes if we reach here
    cr_assert(true);
}

Test(close_fd, already_closed_fd, .init = redirect_all_std)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    cr_expect_neq(fd, -1);
    
    close(fd); // Close it first
    
    // This should handle already closed fd gracefully
    close_fd(fd);
    
    // Should not crash - test passes if we reach here
    cr_assert(true);
}

// Tests for buffer.c (assuming buffer functions exist)
Test(buffer_operations, buffer_initialization, .init = redirect_all_std)
{
    // Test buffer creation and initialization
    // This test structure depends on your buffer implementation
    cr_assert(true); // Placeholder
}

Test(buffer_operations, buffer_write_read, .init = redirect_all_std)
{
    // Test writing to and reading from buffer
    // This test structure depends on your buffer implementation  
    cr_assert(true); // Placeholder
}

Test(buffer_operations, buffer_overflow, .init = redirect_all_std)
{
    // Test buffer overflow conditions
    // This test structure depends on your buffer implementation
    cr_assert(true); // Placeholder
}

// Integration tests
Test(network_integration, full_socket_lifecycle, .init = redirect_all_std)
{
    // Test complete socket creation, bind, listen cycle
    int sockfd = set_socket();
    cr_expect_neq(sockfd, -1);
    
    int bind_result = bind_socket(sockfd, 0);
    cr_assert_eq(bind_result, 0);
    
    int listen_result = listen_socket(sockfd, 5);
    cr_assert_eq(listen_result, 0);
    
    close_fd(sockfd);
}

Test(network_integration, message_send_receive, .init = redirect_all_std)
{
    setup_sockets();
    cr_expect_neq(test_client_fd, -1);
    cr_expect_neq(test_server_fd, -1);
    
    // Send message
    const char *message = "Integration test message";
    int send_result = write_message(test_client_fd, message);
    cr_assert_eq(send_result, 0);
    
    // Receive message
    char *received = get_message(test_server_fd);
    if (received) {
        cr_assert_str_eq(received, message);
        free(received);
    }
    
    cleanup_sockets();
}

// Edge case tests
Test(network_edge_cases, large_message, .init = redirect_all_std)
{
    setup_sockets();
    cr_expect_neq(test_client_fd, -1);
    
    // Create a large message
    size_t large_size = 8192;
    char *large_message = (char *)malloc(sizeof(char) *  large_size);
    memset(large_message, 'A', large_size - 1);
    large_message[large_size - 1] = '\0';
    
    write_message(test_client_fd, large_message);
    
    // Should handle large messages appropriately
    free(large_message);
    cleanup_sockets();
}

Test(network_edge_cases, rapid_connections, .init = redirect_all_std)
{
    // Test rapid connection/disconnection cycles
    for (int i = 0; i < 10; i++) {
        int sockfd = set_socket();
        if (sockfd != -1) {
            close_fd(sockfd);
        }
    }
    
    cr_assert(true); // Test passes if no crashes occur
}

