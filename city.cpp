#include <iostream>
#include "city.h"
#include "simpleRandom.h"
#include "settings.h"


City::City(int height, int width, int horizontalBuildingCount, int verticalBuildingCount, const SimulationSettings& settings) : settings(settings), streetSize(settings.getStreetSize()) {

    this->horizontalBuildingCount = horizontalBuildingCount;
    this->verticalBuildingCount = verticalBuildingCount;

    //generowanie miasta

    //generowanie budynków (czysto graficzne)
    int horizontalStreetRowCount = horizontalBuildingCount - 1;
    int horizontalBuildingSize = (width - horizontalStreetRowCount * streetSize) / horizontalBuildingCount;
    int verticalStreetRowCount = verticalBuildingCount - 1;
    int verticalBuildingSize = (height - verticalStreetRowCount * streetSize) / verticalBuildingCount;

    for (int x = 0; x < horizontalBuildingCount; x++) {
        for (int y = 0; y < verticalBuildingCount; y++) {
            int xLoc = x * (horizontalBuildingSize + streetSize);
            int yLoc = y * (verticalBuildingSize + streetSize);
            auto* b = new Building(xLoc, yLoc, horizontalBuildingSize, verticalBuildingSize);
            buildings.add(b);
        }
    }

    //generowanie skrzyżowań
    for (int x = 0; x < horizontalStreetRowCount; x++) {
        for (int y = 0; y < verticalStreetRowCount; y++) {
            int xPos = horizontalBuildingSize + x * (horizontalBuildingSize + streetSize);
            int yPos = verticalBuildingSize + y * (verticalBuildingSize + streetSize);
            auto* intersection = new Intersection(xPos, yPos, settings);
            intersections.add(intersection);
        }
    }

    //generowanie ulic

    //poziome
    for (int y = 0; y < verticalBuildingCount - 1; y++) {
        for (int x = 0; x < horizontalBuildingCount; x++) {
            int xStart = (horizontalBuildingSize + streetSize) * x;
            int yStart = verticalBuildingSize + (verticalBuildingSize + streetSize) * y;
            int xEnd = horizontalBuildingSize + xStart;
            int yEnd = yStart + streetSize;
            auto* street = new Street(xStart, yStart, xEnd, yEnd, horizontal, settings);
            horizontalStreets.add(street);
        }
    }

    //pionowe
    for (int y = 0; y < verticalBuildingCount; y++) {
        for (int x = 0; x < horizontalBuildingCount - 1; x++) {
            int xStart = horizontalBuildingSize + (horizontalBuildingSize + streetSize) * x;
            int yStart = (verticalBuildingSize + streetSize) * y;
            int xEnd = xStart + streetSize;
            int yEnd = verticalBuildingSize + yStart;
            auto* street = new Street(xStart, yStart, xEnd, yEnd, vertical, settings);
            verticalStreets.add(street);
        }
    }

    for (int i = 0; i < settings.getCarCount(); i++) {
        addCar();
    }

    connectStreetsAndIntersections();
}

void City::update() {
    for (int i = 0; i < verticalStreets.size(); i++) {
        verticalStreets.get(i)->update();
    }

    for (int i = 0; i < horizontalStreets.size(); i++) {
        horizontalStreets.get(i)->update();
    }

    for (int i = 0; i < intersections.size(); i++) {
        intersections.get(i)->update();
    }
}

void City::draw(sf::RenderWindow& window) {
    window.clear(sf::Color(64, 64, 64));


    for (int i = 0; i < verticalStreets.size(); i++) {
        verticalStreets.get(i)->draw(window);
    }

    for (int i = 0; i < horizontalStreets.size(); i++) {
        horizontalStreets.get(i)->draw(window);
    }

    for (int i = 0; i < intersections.size(); i++) {
        intersections.get(i)->draw(window);
    }

    for (int i = 0; i < buildings.size(); i++) {
        buildings.get(i)->draw(window);
    }

}


void City::connectStreetsAndIntersections() {
    for (int x = 0; x < horizontalBuildingCount - 1; x++) {
        for (int y = 0; y < verticalBuildingCount - 1; y++) {
            auto intersection = getIntersectionAt(x, y);
            auto streetNorth = getVerticalStreetAt(x, y);
            auto streetWest = getHorizontalStreetAt(x, y);
            auto streetSouth = getVerticalStreetAt(x, y + 1);
            auto streetEast = getHorizontalStreetAt(x + 1, y);
            intersection->connectTo(streetNorth, streetEast, streetWest, streetSouth);
        }
    }
}


City::~City() {
    for (int i = 0; i < cars.size(); i++) {
        delete cars.get(i);
    }
    cars.removeAll();

    for (int i = 0; i < verticalStreets.size(); i++) {
        delete verticalStreets.get(i);
    }
    verticalStreets.removeAll();

    for (int i = 0; i < horizontalStreets.size(); i++) {
        delete horizontalStreets.get(i);
    }
    horizontalStreets.removeAll();

    for (int i = 0; i < buildings.size(); i++) {
        delete buildings.get(i);
    }
    buildings.removeAll();

    for (int i = 0; i < intersections.size(); i++) {
        delete intersections.get(i);
    }
    intersections.removeAll();
}

Street* City::getHorizontalStreetAt(int x, int y) {
    int mult = horizontalBuildingCount;
    int id = x + y * mult;
    return horizontalStreets.get(id);
}

Street* City::getVerticalStreetAt(int x, int y) {
    int mult = horizontalBuildingCount - 1;
    int id = x + y * mult;
    return verticalStreets.get(id);
}

Intersection* City::getIntersectionAt(int x, int y) {
    int mult = verticalBuildingCount - 1;
    int id = y + x * mult;
    return intersections.get(id);
}

void City::removeFirstCar() {
    if (cars.size() == 0)
        return;
    auto car = cars.get(0);
    for (int i = 0; i < horizontalStreets.size(); i++) {
        horizontalStreets.get(i)->removeCar(car);
    }
    for (int i = 0; i < verticalStreets.size(); i++) {
        verticalStreets.get(i)->removeCar(car);
    }
    for (int i = 0; i < horizontalStreets.size(); i++) {
        horizontalStreets.get(i)->removeCar(car);
    }
    for (int i = 0; i < intersections.size(); i++) {
        intersections.get(i)->removeCar(car);
    }
    cars.remove(0);
    delete car;
}

void City::addCar() {
    auto car = new Car(settings);
    cars.add(car);
    if (getRandom(0, 1) < 0.5) {
        int x = (int) getRandom(0, horizontalBuildingCount);
        int y = (int) getRandom(0, verticalBuildingCount - 1);
        auto street = getHorizontalStreetAt(x, y);
        if (getRandom(0, 1) < 0.5) {
            street->putCarNe(car);
        } else {
            street->putCarSw(car);
        }
    } else {
        int x = (int) getRandom(0, horizontalBuildingCount - 1);
        int y = (int) getRandom(0, verticalBuildingCount);
        auto street = getVerticalStreetAt(x, y);
        if (getRandom(0, 1) < 0.5) {
            street->putCarNe(car);
        } else {
            street->putCarSw(car);
        }
    }

}

