#include "behaviors.h"
#include "robot.h"

int TestMovement::run() {
  vehicle.move_forward(5000);
  vehicle.change_forward_side(LEFT);
  vehicle.move_forward(3000);

  return TestMovement::Result::SUCCESS;
}
