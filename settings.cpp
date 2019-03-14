#include <iostream>
#include <sstream>
#include <limits>
#include "settings.h"

void SimulationSettings::userSetSettings() {
    std::cout << "Podaj szerokosc okna lub pozostaw puste (zalecane: mniej niz rozdzielczosc)" << std::endl;
    windowWidth = getOrDefault(800, 200, -1);

    std::cout << "Podaj wysokosc okna lub pozostaw puste (zalecane: mniej niz rozdzielczosc)" << std::endl;
    windowHeight = getOrDefault(800, 200, -1);

    std::cout << "Podaj liczbe kolumn budynkow lub pozostaw puste (zalecane 2-10)" << std::endl;
    horizontalBuildingCount = getOrDefault(4, 2, -1);

    std::cout << "Podaj liczbe wierszy budynkow lub pozostaw puste (zalecane 2-10)" << std::endl;
    verticalBuildingCount = getOrDefault(4, 2, -1);

    std::cout << "Podaj grubosc ulicy lub pozostaw puste (zalecane okolo 65, w zaleznosci od poprzednich parametrow)" << std::endl;
    streetSize = getOrDefault(65, 10, -1);

    std::cout << "Podaj liczbe aut lub pozostaw puste (zalecane do okolo 60 przy domyslnych parametrach, wiecej moze bardzo szybko powodowac korki) " << std::endl;
    carCount = getOrDefault(60, 0, 100000);

    carLength = streetSize/4.5;
    carWidth = streetSize/8.5;
    minimumDifferenceBetweenCars = carLength * 1.7;
}

int SimulationSettings::getOrDefault(int defaultValue, int min, int max) {
    int returnValue = defaultValue;

    std::string input;
    std::getline(std::cin, input);

    std::istringstream iss(input);

    if (iss >> returnValue) {
        if (returnValue < min && min != -1) {
            return min;
        }
        if (returnValue > max && max != -1) {
            return max;
        }

        return returnValue;
    } else {
        return defaultValue;
    }

   //if (std::cin.peek() == '\n') {
   //    std::cout << "uwu" << std::endl;
   //    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
   //    return defaultValue;
   //}

   //if (!(std::cin >> returnValue)) {
   //    std::cin.clear();
   //    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
   //    std::cerr << "Bledne wejscie. Program moze nie dzialac jak oczekiwano." << std::endl;
   //    return defaultValue;
   //}

   //if (returnValue < min && min != -1) {
   //    return min;
   //}
   //if (returnValue > max && max != -1) {
   //    return max;
   //}

   //return returnValue;
}


/*
 * Poniżej gettery dla klasy settings
 */

int SimulationSettings::getHorizontalBuildingCount() const {
    return horizontalBuildingCount;
}

int SimulationSettings::getVerticalBuildingCount() const {
    return verticalBuildingCount;
}

int SimulationSettings::getStreetSize() const {
    return streetSize;
}

int SimulationSettings::getWindowWidth() const {
    return windowWidth;
}

int SimulationSettings::getWindowHeight() const {
    return windowHeight;
}

double SimulationSettings::getCarLength() const {
    return carLength;
}

double SimulationSettings::getCarWidth() const {
    return carWidth;
}

double SimulationSettings::getMinimumDifferenceBetweenCars() const {
    return minimumDifferenceBetweenCars;
}

int SimulationSettings::getCarCount() const {
    return carCount;
}
