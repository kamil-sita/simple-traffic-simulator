#ifndef CISKITEST_CITY_H
#define CISKITEST_CITY_H

#include "building.h"
#include "intersection.h"
#include "street.h"
#include "car.h"
#include "resizableArray.h"

/**
 * Klasa reprezentująca miasto. Posiada logikę dotyczącą inicjializowania miasta (generowania i łączenia skrzyżowań i dróg) oraz logikę dotyczącą
 * odświeżania symulacji
 */
class City : public Drawable {
private:
    int horizontalBuildingCount;
    int verticalBuildingCount;

    resizableArray<Building*> buildings;
    resizableArray<Car*> cars;
    resizableArray<Street*> verticalStreets;
    resizableArray<Street*> horizontalStreets;
    resizableArray<Intersection*> intersections;

    const SimulationSettings& settings;
    int streetSize;

    Street* getHorizontalStreetAt(int x, int y);
    Street* getVerticalStreetAt(int x, int y);
    Intersection* getIntersectionAt(int x, int y);
    void connectStreetsAndIntersections();

public:
    City(int height, int width, int horizontalBuildingCount, int verticalBuildingCount, const SimulationSettings& settings);
    ///usunięcie pierwszego ws dodania auta
    void removeFirstCar();
    ///dodanie auta w losowym miejscu
    void addCar();
    ~City();
    ///aktualizacja symulacji
    void update();
    ///rysowanie symulacji
    void draw(sf::RenderWindow& window) override;
};


#endif