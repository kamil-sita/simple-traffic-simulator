#ifndef CISKITEST_SIMPLERANDOM_H
#define CISKITEST_SIMPLERANDOM_H

#include "resizableArray.h"
#include "positional.h"

///losowa liczba z danego przedziału
double getRandom(double min, double max);
///losowo wypełnia ściężkę jazdy samochodu
void fillPath(resizableArray<drivingDirection>* path);

#endif
