#include "windowLogic.h"
#include "settings.h"

int main() {
    SimulationSettings s;
    s.userSetSettings();
    createAndRunWindow(s);
    return 0;
}
