#include <iostream>
#include "simulator.h"
#include "../robot.h"

int main() {
    Simulator simulator;
    simulator.set_position(0.0, 1.0, -0.1);
    Robot<Simulator> robot(simulator);
    robot.run();
    for (auto point : simulator.location_history) {
        std::cout << point.first << ',' << point.second << std::endl;
    }
    return 0;
}