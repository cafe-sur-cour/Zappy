/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** network
*/

#ifndef NETWORK_H_
    #define NETWORK_H_

/* Write an errro message */
void error_print(char const *message);
/* Set the socket of the file descriptor */
int set_socket(void);
/* Bind the file decriptor to the port */
int bind_socket(int fd, int port);
/* Specify the queue the fd will use */
int listen_socket(int fd, int backlog);

/* Close the server */
void close_fd(int fd);

/* Accept new connetion */
int accept_connection(int server_fd);
/* Handle Message input */
char *get_message(int fd, int timeout);
/* Hello */
int write_message(int fd, const char *message);
#endif /* !NETWORK_H_ */
