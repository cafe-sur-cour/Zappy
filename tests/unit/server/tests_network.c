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
#include <fcntl.h>
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
    server_t *server = (server_t *)malloc(sizeof(server_t));
    server->sockfd = sockfd;
    server->port = 8080; // Example port
    server->backlog = 5;
    server->pollserver.fd = sockfd;
    int result = bind_socket(server); // Port 0 for automatic assignment
    
    cr_assert_eq(result, 0);
    
    close(sockfd);
}

Test(bind_socket, invalid_socket_fd, .init = redirect_all_std)
{
    server_t *server = (server_t *)malloc(sizeof(server_t));
    server->sockfd = -1;
    server->port = 8080; // Example port
    server->backlog = 5;
    server->pollserver.fd = -1;
    int result = bind_socket(server);
    
    cr_assert_eq(result, -1);
}

Test(bind_socket, invalid_port, .init = redirect_all_std)
{
    int sockfd = set_socket();
    cr_expect_neq(sockfd, -1);
    server_t *server = (server_t *)malloc(sizeof(server_t));
    server->sockfd = sockfd;
    server->port = 80; // Example port
    server->backlog = 5;
    server->pollserver.fd = sockfd;
    bind_socket(server);
}

Test(listen_socket, successful_listen, .init = redirect_all_std)
{
    int sockfd = set_socket();
    cr_expect_neq(sockfd, -1);

    server_t *server = (server_t *)malloc(sizeof(server_t));
    server->sockfd = sockfd;
    server->port = 0; // Example port
    server->backlog = 5;
    server->pollserver.fd = sockfd;
    bind_socket(server);
    int result = listen_socket(server);
    
    cr_assert_eq(result, 0);
    
    close(sockfd);
}

Test(listen_socket, invalid_socket_fd, .init = redirect_all_std)
{
    server_t *server = (server_t *)malloc(sizeof(server_t));
    server->sockfd = -1;
    server->port = 5; // Example port
    server->backlog = 5;
    server->pollserver.fd = -1;
    int result = listen_socket(server);
    
    cr_assert_eq(result, -1);
}

// Tests for accept_connection.c
Test(accept_connection, invalid_server_fd, .init = redirect_all_std)
{
    server_t *server = (server_t *)malloc(sizeof(server_t));
    server->sockfd = -1;
    server->port = 8080; // Example port
    server->backlog = 5;
    server->pollserver.fd = -1;
    int result = accept_connection(server);
    
    cr_assert_eq(result, -1);
}

