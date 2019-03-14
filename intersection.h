#ifndef INTERSECTION_CL
#define INTERSECTION_CL

#include <SFML/Graphics/RenderWindow.hpp>
#include "positional.h"
#include "resizableArray.h"
#include "settings.h"
#include "drawable.h"

enum canEnterState {
    can,
    wait,
    notYet
};

class Car;
class Street;
/**
 * Klasa reprezentująca skrzyżowanie. Ma logikę dotyczącą zachowania pojazdów na skrzyżowaniu oraz obsługi świateł
 */
class Intersection : public Drawable {
private:
    //pozycja do rysowania
    int x;
    int y;
    //zmienne dla świateł
    unsigned int currentLightsIteration = 0;
    unsigned int nextCheckIteration = LIGHTS_DELTA;
    compassDirection allowedDirection = north;

    const SimulationSettings& settings;
    int streetSize;
    //połączone ulice
    Street* northStreet;
    Street* eastStreet;
    Street* westStreet;
    Street* southStreet;
    resizableArray<Car*> nLane;
    resizableArray<Car*> sLane;
    resizableArray<Car*> wLane;
    resizableArray<Car*> eLane;
    ///rysuje światła na skrzyżowaniu
    void drawLights(sf::RenderWindow& window);
    ///rysuje wszystkie auta na skrzyżowaniu
    void drawCars(sf::RenderWindow& window);
    ///aktualizuje cykl świateł
    void updateLights();
    ///używa metody carUpdate() dla wszystkich aut na skrzyżowaniu
    void updateCars();
    ///porusza autem na skrzyżowaniu
    void carUpdate(Car* car);
    ///zwraca wartość logiczną w zależności czy auto znajduje się na danym skrzyżowaniu, przy czym jest to wartość graficzna (koordynaty x, y), a nie sprawdzenie zawartości listy
    bool isOnIntersection(Car* car);
    ///zwraca następną ulicę dla danego auta
    Street* getNextStreet(Car* car);
    canEnterState canCarEnterItsDestinationLane(Car* car);
    void moveCarToDestinationLane(Car* car);
    ///opuszczania skrzyżowania przez samochód
    void exitIntersection(Car* car);
public:
    Intersection(int x, int y, const SimulationSettings& s);
    void draw(sf::RenderWindow &window) override;
    ///połączenie skrzyżowania z ulicami
    void connectTo(Street* north, Street* east, Street* west, Street* south);
    void addCar(Car* car);
    void update();
    Car* getNextCar(Car* car);
    ///ostatnie auto znajdujące się na skrzyżowaniu na części w danym kierunku
    Car* lastCar(compassDirection dir);
    ///logika dotycząca tego czy samochód może wjechać na skrzyżowanie (czy np światła mu nie zabraniają)
    bool canCarEnter(Car* car);
    ///usuwa auto ze skrzyżowania, nie robiąc nic innego
    void removeCar(Car* car);


};

#endif