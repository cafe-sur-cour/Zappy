/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Subject
*/

#include <memory>
#include <string>

#include "Subject.hpp"


void Subject::addObserver(std::shared_ptr<IObserver> observer) {
    _observers.push_back(observer);
}

void Subject::removeObserver(std::shared_ptr<IObserver> observer) {
    _observers.erase(
    std::remove_if(_observers.begin(), _observers.end(),
        [&observer](const std::weak_ptr<IObserver>& weak_obs) {
            return weak_obs.expired() || weak_obs.lock() == observer;
        }),
    _observers.end());
}

void Subject::notifyObservers() {
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

void Subject::notifyGameEvent(GameEventType eventType, const std::string& teamName) {
    _observers.erase(
    std::remove_if(_observers.begin(), _observers.end(),
        [eventType, &teamName](const std::weak_ptr<IObserver>& weak_obs) {
            if (auto obs = weak_obs.lock()) {
                obs->onGameEvent(eventType, teamName);
                return false;
            }
            return true;
        }),
    _observers.end());
}
