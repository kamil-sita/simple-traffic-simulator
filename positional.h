
#ifndef CISKITEST_DIRECTIONTYPE_H
#define CISKITEST_DIRECTIONTYPE_H

///określa kierunek poziomy/pionowy
enum position {
    horizontal,
    vertical
};

///określa kierunek wg kompasu
enum compassDirection {
    north,
    east,
    west,
    south
};

///określa kierunek jazdy z punktu widzenia samochodu/kierowcy
enum drivingDirection {
    forward,
    left,
    right
};

///zwraca następny kierunek
compassDirection nextDirection(compassDirection dir);
///łączy kierunek kompasu i kierunek pojazdu, np kierunek północny i prawo -> wschód
compassDirection combine(compassDirection compassDir, drivingDirection drivingDir);
///konwertuje kierunek poruszania na położenie drogi
compassDirection movingDirectionToLaneDirection(compassDirection dir);
///konwertuje położenie drogi na kierunek poruszania się
compassDirection laneDirectionToMovingDirection(compassDirection dir);

#endif