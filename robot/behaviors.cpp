#include "behaviors.h"
#include "robot.h"

int TestMovement::run() {
    //vehicle.move_forward(2000);
    //vehicle.change_forward_side(LEFT);
    vehicle.drive(10, 0);
    vehicle.change_forward_side(LEFT);
    vehicle.drive(10, 0);
    vehicle.change_forward_side(BACK);

    vehicle.drive(10, 0);
    vehicle.change_forward_side(RIGHT);
    vehicle.drive(10, 0);
    vehicle.change_forward_side(FRONT);

    vehicle.drive(100, 30);
    vehicle.drive(100, -30);

    return TestMovement::Result::SUCCESS;
}
