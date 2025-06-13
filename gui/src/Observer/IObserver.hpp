/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** IObserver
*/

#ifndef IOBSERVER_HPP_
#define IOBSERVER_HPP_

class IObserver {
    public:
        virtual ~IObserver() = default;
        virtual void update() = 0;
};

#endif /* !IOBSERVER_HPP_ */
