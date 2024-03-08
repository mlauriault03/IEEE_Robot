#include <Arduino.h>
#include "behaviors.h"
#include "robot.h"

int TestMovement::run() {
    //vehicle.move_forward(2000);
    //vehicle.change_forward_side(LEFT);
    vehicle.drive(1000, 0);
    delay(1000);
    vehicle.drive(1000, 0);
//    vehicle.change_forward_side(LEFT);
//    vehicle.drive(1000, 0);
//    vehicle.change_forward_side(BACK);
//
//    vehicle.drive(1000, 0);
//    vehicle.change_forward_side(RIGHT);
//    vehicle.drive(1000, 0);
//    vehicle.change_forward_side(FRONT);
//
//    vehicle.drive(1000, 30);
//    vehicle.drive(1000, -30);

    return TestMovement::Result::SUCCESS;
}
