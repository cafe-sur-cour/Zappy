/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Subject
*/

#include <algorithm>
#include <memory>
#include <vector>

#include "ISubject.hpp"

#ifndef SUBJECT_HPP_
#define SUBJECT_HPP_

class Subject : public ISubject {
    public:
        virtual ~Subject() = default;

        void addObserver(std::shared_ptr<IObserver> observer) override {
            _observers.push_back(observer);
        }

        void removeObserver(std::shared_ptr<IObserver> observer) override {
            _observers.erase(
                std::remove_if(_observers.begin(), _observers.end(),
                    [&observer](const std::weak_ptr<IObserver>& weak_obs) {
                        return weak_obs.expired() || weak_obs.lock() == observer;
                    }),
                _observers.end());
        }

        void notifyObservers() override {
            _observers.erase(
                std::remove_if(_observers.begin(), _observers.end(),
                    [](const std::weak_ptr<IObserver>& weak_obs) {
                        if (auto obs = weak_obs.lock()) {
                            obs->update();
                            return false;
                        }
                        return true;
                    }),
                _observers.end());
        }

    private:
        std::vector<std::weak_ptr<IObserver>> _observers;
};

#endif /* !SUBJECT_HPP_ */
