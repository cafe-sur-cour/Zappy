/*
** EPITECH PROJECT, 2025
** Server
** File description:
** Server :: Main
*/

#include <stdio.h>
#include <string.h>

int helper(void)
{
    printf("USAGE: ./zappy_server -p port -x width -y height ");
    printf("-n name1 name2 ... -c clientsNb -f freq\n");
    return 84;
}

int main(int argc, char **argv)
{
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "-help") == 0)) {
        return helper();
    }
    printf("\033[1;34m[SERVER]\033[0m ");
    printf("Welcome to the Tranttorian world!\n");
    return 0;
}
