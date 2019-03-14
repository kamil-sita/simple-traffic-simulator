#ifndef SIMULATOR_SETTINGS_H
#define SIMULATOR_SETTINGS_H

///minimalna prędkość samochodu w pikselach
const double SPD_MIN = 0.6;
///maksymalna prędkość samochodu w pikselach
const double SPD_MAX = 1;

///czas minimalny między zmianą świateł na skrzyżowaniu
const unsigned int LIGHTS_DELTA = 60;

class SimulationSettings {
private:
    int horizontalBuildingCount = 4;
    int verticalBuildingCount = 4;
    int streetSize = 65;
    int windowWidth = 800;
    int windowHeight = 800;

    int carCount = 300;

    double carLength = streetSize/4.5;
    double carWidth = streetSize/8.5;
    double minimumDifferenceBetweenCars = carLength * 1.8;


    /**
     * Czyta wartość podaną od użytkownika lub zwraca wartość domyślną
     * @param defaultValue wartość domyślna
     * @param min wartość minimalna lub -1 jeżeli nieokreślona
     * @param max wartość maksymalna lub -1 jeżeli nieokreślona
     * @return wartość podana od użytkownika lub domyślna
     */
    int getOrDefault(int defaultValue, int min, int max);

public:
    /**
     * Pozwala użytkownikowi na wpisanie parametrów symulacji
     */
    void userSetSettings();


    /*
     * Poniżej gettery
     */

    int getHorizontalBuildingCount() const;

    int getVerticalBuildingCount() const;

    int getStreetSize() const;

    int getWindowWidth() const;

    int getWindowHeight() const;

    double getCarLength() const;

    double getCarWidth() const;

    double getMinimumDifferenceBetweenCars() const;

    int getCarCount() const;
};





#endif