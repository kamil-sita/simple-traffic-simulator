#ifndef CAR_HEADER
#define CAR_HEADER

#include <SFML/Graphics/RenderWindow.hpp>
#include "positional.h"
#include "resizableArray.h"
#include "drawable.h"
#include "settings.h"

class Intersection;
class Street;
/**
 * Klasa reprezentująca samochódm Ma podstawową logika dotycząca poruszania się oraz wykrywania bliskości innych samochodów.
 */
class Car : public Drawable {
private:
    double x;
    double y;
    double speed;
    ///kierunek w którym porusza się samochód
    compassDirection dir;
    sf::Color color;
    ///planowana ścieżka poruszania się samochodu
    resizableArray<drivingDirection> path;

    const SimulationSettings& settings;

public:
    Car(const SimulationSettings& settings);
    void draw(sf::RenderWindow& window) override;
    double getSpeed() const;
    void setPos(double x, double y);
    ///kierunek poruszania się samochodu
    void setMovingDirection(compassDirection dir);
    ///kierunek według położenia drogi, na której samochód się porusza
    void setLaneDirection(compassDirection dir);
    ///zwraca kierunek w którym się porusza
    compassDirection getMovingDirection();
    ///zwraca pozycję ulicy na której jest (porusza się na północ -> jest na wchodniej części ulicy)
    compassDirection getLaneDirection();
    double getX() const;
    double getY() const;
    ///opisuje czy nie jest za blisko innego samochodu, czy może się ruszyć
    bool isFarEnoughFrom(Car* otherCar);
    ///proste poruszenie się o val pikseli w kierunku, w którym samochód się porusza
    void simpleMoveInCurrentDirection(double val);

    ///zwraca pierwszy kierunek z path
    compassDirection getNextPlannedDirection();
    ///usuwa pierwszy kierunek z path
    void removeFirstPlannedDirection();
    ///ustawia pierwszy kierunek z path jako forward
    void setFirstPlannedDirectionAsForward();
};


#endif