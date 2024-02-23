#include <iostream>
#include "simulator.h"
#include "../autonomy.h"

int main() {
    Simulator simulator;
    simulator.set_position(0.0, -5.995, 0.5);
    Robot<Simulator> robot(simulator);
    robot.run();
    for (auto point : simulator.location_history) {
        std::cout << point.first << ',' << point.second << std::endl;
    }
    return 0;
}