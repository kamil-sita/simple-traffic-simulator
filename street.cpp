#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>
#include <cmath>
#include "street.h"
#include "intersection.h"
#include "car.h"
#include "settings.h"

Street::Street(int posXstart, int posYstart, int posXend, int posyEnd, position streetDirection, const SimulationSettings& settings) : 
posXstart(posXstart), posYstart(posYstart), posXend(posXend), posYend(posyEnd), 
streetPosition(streetDirection), settings(settings), streetSize(settings.getStreetSize())  {}


void Street::draw(sf::RenderWindow &window) {
    drawSelf(window);
    drawCars(window);
}


void Street::drawSelf(sf::RenderWindow& window) {
    sf::RectangleShape lines;
    lines.setFillColor(sf::Color(210, 210, 210));
    int lineLength = 0;
    if (this->streetPosition == horizontal) {
        lineLength = (posXend - posXstart) / 11;
        lines.setSize(sf::Vector2f(streetSize/8.f, 2));
        for (int i = 0; i < 6; i++) {
            lines.setPosition(posXstart + i * lineLength * 2, posYstart + (posYend - posYstart)/2.0f);
            window.draw(lines);
        }
    } else {
        lineLength = (posYend - posYstart)/11;
        lines.setSize(sf::Vector2f(2, streetSize/8.f));
        for (int i = 0; i < 6; i++) {
            lines.setPosition(posXstart + (posXend - posXstart)/2.0f, posYstart + i * 2 * lineLength);
            window.draw(lines);
        }
    }
}

void Street::drawCars(sf::RenderWindow& window) {
    for (int i = 0; i < swLane.size(); i++) {
        swLane.get(i)->draw(window);
    }
    for (int i = 0; i < neLane.size(); i++) {
        neLane.get(i)->draw(window);
    }
}


void Street::connectAsOutputToNorthEast(Intersection* s) {
    this->neOutput = s;
}

void Street::connectAsOutputToSouthWest(Intersection* s) {
    this->swOutput = s;
}


void Street::putCarNe(Car* car) {
    if (streetPosition == horizontal) {
        car->setPos(posXstart, posYstart + streetSize*3.0/4.0);
        car->setMovingDirection(east);
    } else {
        car->setPos(posXstart + streetSize*3.0/4.0, posYend);
        car->setMovingDirection(north);
    }
    neLane.add(car);
}

void Street::putCarSw(Car* car) {
    if (streetPosition == horizontal) {
        car->setMovingDirection(west);
        car->setPos(posXend, posYstart + streetSize/4.0);
    } else {
        car->setMovingDirection(south);
        car->setPos(posXstart + streetSize/4.0, posYstart);
    }
    swLane.add(car);
}

void Street::update() {
    for (int i = 0; i < neLane.size(); i++) {
        carUpdate(neLane.get(i));
    }
    for (int i = 0; i < swLane.size(); i++) {
        carUpdate(swLane.get(i));
    }
}

Car* Street::getLastCar(compassDirection dir) {
    switch (dir) {
        case north:
            return neLane.getLast();
        case east:
            return neLane.getLast();
        case west:
            return swLane.getLast();
        case south:
            return swLane.getLast();
    }
    return nullptr;
}

Car *Street::getNextCar(Car* car) {
    if (neLane.contains(car)) {
        int pos = neLane.indexOf(car);
        if (pos == 0) {
            if (getNextIntersection(car) == nullptr) return nullptr;
            return getNextIntersection(car)->lastCar(car->getLaneDirection());
        }
        return neLane.get(pos - 1);
    }
    if (swLane.contains(car)) {
        int pos = swLane.indexOf(car);
        if (pos == 0) {
            if (getNextIntersection(car) == nullptr) return nullptr;
            return getNextIntersection(car)->lastCar(car->getLaneDirection());
        }
        return swLane.get(pos - 1);
    }
    return nullptr;
}

bool Street::isOnStreet(Car* car) {
    if (this->streetPosition == horizontal) {
        return car->getX() >= this->posXstart && car->getX() <= this->posXend;
    } else {
        return car->getY() >= this->posYstart && car->getY() <= this->posYend;
    }
}

bool Street::isOnStreet(Car* car, double backupLength) {
    if (this->streetPosition == horizontal) {
        if (car->getMovingDirection() == east) {
            return car->getX() >= this->posXstart && car->getX() + backupLength<= this->posXend;
        } else {
            return car->getX() - backupLength >= this->posXstart && car->getX()<= this->posXend;
        }
    } else {
        if (car->getMovingDirection() == north) {
            return car->getY() - backupLength >= this->posYstart && car->getY() <= this->posYend;
        } else {
            return car->getY() >= this->posYstart && car->getY() + backupLength <= this->posYend;
        }
    }
}

void Street::carUpdate(Car* car) {
    if (isOnStreet(car)) {
        auto nextCar = getNextCar(car);
        if (!car->isFarEnoughFrom(nextCar)) return;
        //efekt graficzny by samochód nie wyjeżdżał na skrzyżowanie i tam czekał na światła
        if (isOnStreet(car, settings.getStreetSize()/3.0)) {
            car->simpleMoveInCurrentDirection(car->getSpeed());
        } else {
            Intersection* nextIntersection = getNextIntersection(car);
            if (nextIntersection == nullptr) {
                car->simpleMoveInCurrentDirection(car->getSpeed());
                return;
            }
            if (nextIntersection->canCarEnter(car)) {
                car->simpleMoveInCurrentDirection(car->getSpeed());
            }
        }
    } else {
        Intersection* nextIntersection = getNextIntersection(car);
        if (nextIntersection == nullptr) {
            if (neLane.contains(car)) {
                removeCar(car);
                putCarSw(car);
            } else {
                removeCar(car);
                putCarNe(car);
            }
            return;
        }
        if (nextIntersection->canCarEnter(car)) {
            auto nextCar = nextIntersection->lastCar(car->getLaneDirection());
            if (!car->isFarEnoughFrom(nextCar))
                return;

            nextIntersection->addCar(car);
            removeCar(car);
        }
    }
}

Intersection* Street::getNextIntersection(Car* car) {
    if (car->getMovingDirection() == north || car->getMovingDirection() == west) {
        return neOutput;
    }
    return swOutput;
}

void Street::removeCar(Car* car) {
    if (neLane.contains(car)) {
        neLane.removeElement(car);
    }
    if (swLane.contains(car)) {
        swLane.removeElement(car);
    }
}
