#ifndef STREET_CL
#define STREET_CL

#include "positional.h"
#include "resizableArray.h"
#include "drawable.h"
#include "settings.h"
#include <SFML/Graphics/RenderWindow.hpp>

class Car;
class Intersection;
/***
 * Klasa reprezentująca ulicę. Posiada logikę dotyczącą rysowania oraz poruszania się na niej samochodów.
 */
class Street : public Drawable {
private:
    int posXstart;
    int posYstart;
    int posXend;
    int posYend;
    Intersection* neOutput = nullptr;
    Intersection* swOutput = nullptr;
    ///północny/wschodni pas ruchu, przy czym chodzi o kierunek jazdy
    resizableArray<Car*> neLane;
    ///południowy/zachodni pas ruchu, przy czym chodzi o kierunek jazdy
    resizableArray<Car*> swLane;
    position streetPosition;

    const SimulationSettings& settings;
    int streetSize;

    void drawSelf(sf::RenderWindow& window);
    void drawCars(sf::RenderWindow& window);
    Intersection* getNextIntersection(Car* car);

public:
    Street(int posXstart, int posYstart, int posXend, int posyEnd, position streetDirection, const SimulationSettings& settings);
    ///funckja która ustawia wyjazd północny/wschodni tej ulicy na dane skrzyżowanie
    void connectAsOutputToNorthEast(Intersection* s);
    ///funckja która ustawia wyjazd południowy/zachodni tej ulicy na dane skrzyżowanie
    void connectAsOutputToSouthWest(Intersection* s);
    void update();
    ///zwraca wartość logiczną w zależności czy auto znajduje się na danej ulicy, przy czym jest to wartość graficzna (koordynaty x, y), a nie sprawdzenie zawartości listy
    bool isOnStreet(Car* car);
    ///działa podobnie jak isOnStreet(), ale dodaje sobie backupLength, żeby auto nie "wyjeżdżało" poza ulicę czekając na światło
    bool isOnStreet(Car* car, double backupLength);
    void draw(sf::RenderWindow& window) override;
    ///dodaje auto do listy oraz ustawia jego parametry
    void putCarNe(Car* car);
    ///dodaje auto do listy oraz ustawia jego parametry
    void putCarSw(Car* car);
    ///zwraca następne auto po danym aucie. Może zwrócić auto z następnego skrzyżowania
    Car* getNextCar(Car* car);
    Car* getLastCar(compassDirection dir);
    ///usuwa auto ze wszystkich linii
    void removeCar(Car* car);

    void carUpdate(Car* car);

};


#endif
