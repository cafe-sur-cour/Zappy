/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** my_unsignedlen
*/

int my_unsignedlen(unsigned int nb)
{
    int i = 0;

    while (nb / 10 > 0) {
        i++;
        nb /= 10;
    }
    i++;
    return (i);
}
