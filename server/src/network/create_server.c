/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** create_server
*/

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>


int set_socket(void)
{
    int opt = 1;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Set socket options failed");
        close(sockfd);
        return -1;
    }
    return sockfd;
}

int bind_socket(int fd, int port)
{
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(fd);
        return -1;
    }
    return 0;
}

int listen_socket(int fd, int backlog)
{
    if (listen(fd, backlog) < 0) {
        perror("Listen failed");
        close(fd);
        return -1;
    }
    return 0;
}
