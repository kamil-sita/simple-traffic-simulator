#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <cmath>
#include "car.h"
#include "simpleRandom.h"
#include "street.h"
#include "settings.h"
#include "intersection.h"


Car::Car(const SimulationSettings& settings) : settings(settings) {
    setPos(0, 0);
    this->speed = getRandom(SPD_MIN, SPD_MAX);
    this->color = sf::Color((sf::Uint8) (getRandom(30, 235)), (sf::Uint8) (getRandom(30, 235)), (sf::Uint8) (getRandom(30, 235))); //losowe kolory aut, by je łatwiej rozróżnić
}

void Car::draw(sf::RenderWindow& window) {
    sf::RectangleShape rect;
    rect.setFillColor(color);
    if (getMovingDirection() == west || getMovingDirection() == east) {
        rect.setSize(sf::Vector2f((float) settings.getCarLength(), (float) settings.getCarWidth()));
        rect.setPosition((float) (x - settings.getCarLength()/2.f), (float) (y - settings.getCarWidth()/2.f));
    } else {
        rect.setSize(sf::Vector2f((float) settings.getCarWidth(), (float) settings.getCarLength()));
        rect.setPosition((float) (x - settings.getCarWidth()/2.f), (float) (y - settings.getCarLength()/2.f));
    }
    window.draw(rect);
}

double Car::getSpeed() const {
    return speed;
}

void Car::setMovingDirection(compassDirection dir) {
    this->dir = dir;
}

void Car::setLaneDirection(compassDirection dir) {
    this->dir = laneDirectionToMovingDirection(dir);
}

compassDirection Car::getMovingDirection() {
    return dir;
}

bool Car::isFarEnoughFrom(Car* otherCar) {
    if (otherCar == nullptr) {
        return true;
    }
    if (this->dir != otherCar->dir) {
        return false;
    }
    if (this->dir == north || this->dir == south) {
        double diff = fabs(this->y - otherCar->y);
        return diff > settings.getMinimumDifferenceBetweenCars();
    }
    double diff = fabs(this->x - otherCar->x);
    return diff > settings.getMinimumDifferenceBetweenCars();
}

void Car::setPos(double x, double y) {
    this->x = x;
    this->y = y;
}

void Car::simpleMoveInCurrentDirection(double val) {
    switch (dir) {
        case north:
            y -= val;
            break;
        case east:
            x += val;
            break;
        case west:
            x -= val;
            break;
        case south:
            y += val;
            break;
    }
}

double Car::getX() const {
    return x;
}

double Car::getY() const {
    return y;
}

compassDirection Car::getLaneDirection() {
    return combine(getMovingDirection(), right);
}

compassDirection Car::getNextPlannedDirection() {
    if (path.size() == 0) {
        fillPath(&path);
    }
    return combine(getMovingDirection(), path.get(0));
}

void Car::removeFirstPlannedDirection() {
    path.remove(0);
}

void Car::setFirstPlannedDirectionAsForward() {
    path.replace(0, forward);
}
