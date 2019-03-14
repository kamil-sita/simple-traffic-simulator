#include "positional.h"

compassDirection nextDirection(compassDirection dir) {
    return combine(dir, right);
}

compassDirection combine(compassDirection compassDir, drivingDirection drivingDir) {
    if (drivingDir == forward) return compassDir;
    if (drivingDir == right) {
        switch (compassDir) {
            case north:
                return east;
            case east:
                return south;
            case west:
                return north;
            case south:
                return west;
        }
    }
    switch (compassDir) {
        case north:
            return west;
        case east:
            return north;
        case west:
            return south;
        case south:
            return east;
    }
}


compassDirection movingDirectionToLaneDirection(compassDirection dir) {
    return combine(dir, right);
}

compassDirection laneDirectionToMovingDirection(compassDirection dir) {
    return combine(dir, left);
};
