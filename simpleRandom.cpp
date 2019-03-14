#include <random>
#include <iostream>
#include "simpleRandom.h"

double getRandom(double min, double max) {
    std::uniform_real_distribution<double> uniformRealDistribution(min, max);
    static std::default_random_engine randomEngine; //nie chcemy inicjalizować silnika na nowo za każdym razem, bo dałby te same liczby
    return uniformRealDistribution(randomEngine);
}

void fillPath(resizableArray<drivingDirection>* path) {
    for (int i = 0; i < 5; i++) {
        if (getRandom(0, 1) < 0.6) {
            path->add(forward);
        } else {
            if (getRandom(0, 1) < 0.5) {
                path->add(left);
            } else {
                path->add(right);
            }
        }
    }
}
