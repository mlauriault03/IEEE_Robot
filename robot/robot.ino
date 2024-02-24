#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#include "autonomy.h"
#include "hardware.h"

void setup() {
  setup_all_motors();
  setup_servos();
  setup_display();
}

void loop() {
  // TODO
}

class Hardware : public VehicleInterface {
  double steer_curvature = 0.0;

  void set_steer_curvature(double radians) {
    steer_curvature = radians;
  }

  void move_increment() {
    // The minimum number of steps a motor will take in one "increment".
    // This should be small enough so that an "increment" is small,
    //    but large enough to prevent jerky motion.
    // Some of the motors may take more steps than the minimum in order
    //    to steer the robot.
    int min_steps = 20;
  }

  // TODO
  void ir_reads_black(IRSensor sensor) {}
  void change_forward_side(Side side) {}

};
