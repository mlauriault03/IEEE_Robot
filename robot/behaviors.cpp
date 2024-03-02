#include "behaviors.h"
#include "robot.h"

int TestMovement::run() {
    vehicle.move_forward(500);
    vehicle.change_forward_side(LEFT);
    vehicle.move_forward(300);

    return TestMovement::Result::SUCCESS;
}
