#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>
#include "intersection.h"
#include "street.h"
#include "car.h"
#include "settings.h"


Intersection::Intersection(int x, int y, const SimulationSettings& s) : settings(s), x(x), y(y), streetSize(s.getStreetSize()) {
}

void Intersection::draw(sf::RenderWindow &window) {
    drawLights(window);
    drawCars(window);
}

void Intersection::connectTo(Street* north, Street* east, Street* west, Street* south) {
    this->northStreet = north;
    north->connectAsOutputToSouthWest(this);
    this->eastStreet = east;
    east->connectAsOutputToNorthEast(this);
    this->westStreet = west;
    west->connectAsOutputToSouthWest(this);
    this->southStreet = south;
    south->connectAsOutputToNorthEast(this);
}

void Intersection::addCar(Car* car) {
    switch (car->getMovingDirection()) {
        case north:
            eLane.add(car);
            break;
        case east:
            sLane.add(car);
            break;
        case west:
            nLane.add(car);
            break;
        case south:
            wLane.add(car);
            break;
    }
}

void Intersection::exitIntersection(Car* car) {
    switch (car->getMovingDirection()) {
        case north:
            eLane.removeElement(car);
            northStreet->putCarNe(car);
            break;
        case east:
            sLane.removeElement(car);
            eastStreet->putCarNe(car);
            break;
        case west:
            nLane.removeElement(car);
            westStreet->putCarSw(car);
            break;
        case south:
            wLane.removeElement(car);
            southStreet->putCarSw(car);
            break;
    }
    car->removeFirstPlannedDirection();
}

Street* Intersection::getNextStreet(Car* car) {
    switch (car->getMovingDirection()) {
        case north:
            return northStreet;
        case east:
            return eastStreet;
        case west:
            return westStreet;
        case south:
            return southStreet;
    }
}

void Intersection::update() {
    updateLights();
    updateCars();
}


void Intersection::updateCars() {
    for (int i = 0; i < sLane.size(); i++) {
        carUpdate(sLane.get(i));
    }
    for (int i = 0; i < wLane.size(); i++) {
        carUpdate(wLane.get(i));
    }
    for (int i = 0; i < eLane.size(); i++) {
        carUpdate(eLane.get(i));
    }
    for (int i = 0; i < nLane.size(); i++) {
        carUpdate(nLane.get(i));
    }
}

void Intersection::carUpdate(Car* car) {
    if (isOnIntersection(car)) {
        auto nextCar = getNextCar(car);
        if (!car->isFarEnoughFrom(nextCar)) return;
        if (car->getMovingDirection() != car->getNextPlannedDirection()) {
            switch (canCarEnterItsDestinationLane(car)) {
                case can:
                    moveCarToDestinationLane(car);
                    break;
                case wait:
                    break;
                case notYet:
                    car->simpleMoveInCurrentDirection(car->getSpeed());
                    break;
            }
        } else {
            car->simpleMoveInCurrentDirection(car->getSpeed());
        }

    } else {
        auto nextStreet = getNextStreet(car);
        auto nextCar = nextStreet->getLastCar(car->getMovingDirection());
        if (nextCar == nullptr) {
            exitIntersection(car);
            return;
        }
        if (car->isFarEnoughFrom(nextCar)) {
            exitIntersection(car);
        }
    }
}

bool Intersection::isOnIntersection(Car* car) {
    return
    (car->getX() >= x && car->getX() <= x + settings.getStreetSize()) &&
    (car->getY() >= y && car->getY() <= y + settings.getStreetSize());
}



void Intersection::drawCars(sf::RenderWindow& window) {
    for (int i = 0; i < sLane.size(); i++) {
        sLane.get(i)->draw(window);
    }
    for (int i = 0; i < wLane.size(); i++) {
        wLane.get(i)->draw(window);
    }
    for (int i = 0; i < nLane.size(); i++) {
        nLane.get(i)->draw(window);
    }
    for (int i = 0; i < eLane.size(); i++) {
        eLane.get(i)->draw(window);
    }
}


Car *Intersection::getNextCar(Car* car) {
    if (nLane.contains(car)) {
        int pos = nLane.indexOf(car);
        if (pos != 0) {
            return nLane.get(pos - 1);
        }
    }
    if (sLane.contains(car)) {
        int pos = sLane.indexOf(car);
        if (pos != 0) {
            return sLane.get(pos - 1);
        }
    }
    if (wLane.contains(car)) {
        int pos = wLane.indexOf(car);
        if (pos != 0) {
            return wLane.get(pos - 1);
        }
    }
    if (eLane.contains(car)) {
        int pos = eLane.indexOf(car);
        if (pos != 0) {
            return eLane.get(pos - 1);
        }
    }
    return getNextStreet(car)->getLastCar(car->getMovingDirection());
}

Car* Intersection::lastCar(compassDirection dir) {
    switch (dir) {
        case north:
            return nLane.getLast();
        case east:
            return eLane.getLast();
        case west:
            return wLane.getLast();
        case south:
            return sLane.getLast();
    }
    return nullptr;
}

void Intersection::updateLights() {
    currentLightsIteration++;
    if (currentLightsIteration >= nextCheckIteration) {
        //jeżeli niczego nie ma na skrzyżowaniu, światła przełączają się
        int count = 0;
        count += sLane.size();
        count += wLane.size();
        count += eLane.size();
        count += nLane.size();
        if (count == 0) {
            this->allowedDirection = nextDirection(this->allowedDirection);
            nextCheckIteration = currentLightsIteration + LIGHTS_DELTA;
        }
    }
}

bool Intersection::canCarEnter(Car* car) {
    return car->getMovingDirection() == allowedDirection;
}

void Intersection::drawLights(sf::RenderWindow& renderWindow) {
    sf::Color red(245, 120, 120);
    sf::Color green(120, 245, 120);
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(streetSize/2.0f, streetSize/20.0f));

    //n
    rect.setPosition(x, y - streetSize/20.0f);
    rect.setFillColor(allowedDirection == south ? green : red);
    renderWindow.draw(rect);

    //s
    rect.setPosition(x + streetSize/2.f, y + streetSize);
    rect.setFillColor(allowedDirection == north ? green : red);
    renderWindow.draw(rect);

    rect.setSize(sf::Vector2f(streetSize/20.0f, streetSize/2.0f));

    //w
    rect.setPosition(x - streetSize/20.f, y + streetSize/2.f);
    rect.setFillColor(allowedDirection == east ? green : red);
    renderWindow.draw(rect);

    //e
    rect.setPosition(x + streetSize, y);
    rect.setFillColor(allowedDirection == west ? green : red);
    renderWindow.draw(rect);



}

canEnterState Intersection::canCarEnterItsDestinationLane(Car* car) {

    //żeby auto mogło zmienić kierunek, musi mieć miejsce i musi przekroczyć odpowiednią wartość X lub Y by wskoczyć na daną linię
    compassDirection destination = car->getNextPlannedDirection();
    compassDirection normalizedDestination = movingDirectionToLaneDirection(destination);
    //sprawdzanie czy auto może przeskoczyć na linię - przekroczyło odpowiednie X i Y
    switch (normalizedDestination) {
        case north:
            if (car->getLaneDirection() == east) {
                if (!(car->getY() <= y + streetSize/4.0)) return notYet;
            } else if (car->getLaneDirection() == west) {
                if (!(car->getY() >= y + streetSize/4.0)) return notYet;
            }
            break;
        case east:

            if (car->getLaneDirection() == south) {
                if (!(car->getX() >= x + streetSize*3.0/4.0)) return notYet;
            } else if (car->getLaneDirection() == north) {
                if (!(car->getX() <= x + streetSize*3.0/4.0)) return notYet;
            }
            break;
        case west:
            if (car->getLaneDirection() == south) {
                if (!(car->getX() >= x + streetSize/4.0)) return notYet;
            } else if (car->getLaneDirection() == north) {
                if (!(car->getX() <= x + streetSize/4.0)) return notYet;
            }
            break;
        case south:
            if (car->getLaneDirection() == east) {
                if (!(car->getY() <= y + streetSize*3.0/4.0)) return notYet;
            } else if (car->getLaneDirection() == west) {
                if (!(car->getY() >= y + streetSize*3.0/4.0)) return notYet;
            }
            break;
    }


    //dla ułatwienia założę że linia musi być pusta
    switch (normalizedDestination) {
        case north:
            if (nLane.size() != 0) return wait;
            break;
        case east:
            if (eLane.size() != 0) return wait;
            break;
        case west:
            if (wLane.size() != 0) return wait;
            break;
        case south:
            if (sLane.size() != 0) return wait;
            break;
    }

    return can;

}

void Intersection::moveCarToDestinationLane(Car* car) {
    auto laneDestination = movingDirectionToLaneDirection(car->getNextPlannedDirection());
    auto oldCarLane = car->getLaneDirection();
    removeCar(car);
    car->setLaneDirection(laneDestination);
    switch (laneDestination) {
        case north:
            car->setPos(oldCarLane == east ? x + streetSize*3.0/4.0 : x + streetSize/4.0, y + streetSize/4.0);
            nLane.add(car);
            break;
        case east:
            car->setPos(x + streetSize*3.0/4.0, oldCarLane == north ? y + streetSize/4.0 : y  + streetSize*3.0/4.0);
            eLane.add(car);
            break;
        case west:
            car->setPos(x + streetSize/4.0, oldCarLane == north ? y + streetSize/4.0 : y + streetSize*3.0/4.0);
            wLane.add(car);
            break;
        case south:
            car->setPos(oldCarLane == east ? x + streetSize*3.0/4.0 : x + streetSize/4.0, y + streetSize*3.0/4.0);
            sLane.add(car);
            break;
    }
    car->setFirstPlannedDirectionAsForward();
}

void Intersection::removeCar(Car* car) {
    if (nLane.contains(car)) {
        nLane.removeElement(car);
    }
    if (eLane.contains(car)) {
        eLane.removeElement(car);
    }
    if (wLane.contains(car)) {
        wLane.removeElement(car);
    }
    if (sLane.contains(car)) {
        sLane.removeElement(car);
    }
}
