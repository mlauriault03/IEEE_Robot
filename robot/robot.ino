#include <Arduino.h>
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

  // The minimum number of steps a motor will take in one "increment".
  // This should be small enough so that an "increment" is small,
  //    but large enough to prevent jerky motion.
  // Some of the motors may take more steps than the minimum in order
  //    to steer the robot.
  const int min_steps = 20;

  void set_steer_curvature(double radians) {
    steer_curvature = radians;
  }

  void move_increment() {
    move_forward(min_steps);
    // Move one side "extra" steps to accomplish steering.
    int extra = steer_curvature*TRACK*min_steps;
    if (extra > 0) {
      move_rightside(extra);  // Steer left.
    }
    else {
      move_leftside(-extra);  // Steer right.
    }
  }

  void move_forward(int nsteps) {
    for (int i = 0; i < nsteps; i++) {
      digitalWrite(FL_DRIVE_STEP, HIGH);
      digitalWrite(BL_DRIVE_STEP, HIGH);
      digitalWrite(FR_DRIVE_STEP, HIGH);
      digitalWrite(BR_DRIVE_STEP, HIGH);
      delayMicroseconds(MOTOR_DELAY);                            //Was a 500 microsecond delay
      digitalWrite(FL_DRIVE_STEP, LOW);
      digitalWrite(BL_DRIVE_STEP, LOW);
      digitalWrite(FR_DRIVE_STEP, LOW);
      digitalWrite(BR_DRIVE_STEP, LOW);
      delayMicroseconds(MOTOR_DELAY); 
    }
  }

  void move_leftside(int nsteps) {
    for (int i = 0; i < nsteps; i++) {
      digitalWrite(FL_DRIVE_STEP, HIGH);
      digitalWrite(BL_DRIVE_STEP, HIGH);
      delayMicroseconds(MOTOR_DELAY);                            //Was a 500 microsecond delay
      digitalWrite(FL_DRIVE_STEP, LOW);
      digitalWrite(BL_DRIVE_STEP, LOW);
      delayMicroseconds(MOTOR_DELAY); 
    }
  }

  void move_rightside(int nsteps) {
    for (int i = 0; i < nsteps; i++) {
      digitalWrite(FR_DRIVE_STEP, HIGH);
      digitalWrite(BR_DRIVE_STEP, HIGH);
      delayMicroseconds(MOTOR_DELAY);                            //Was a 500 microsecond delay
      digitalWrite(FR_DRIVE_STEP, LOW);
      digitalWrite(BR_DRIVE_STEP, LOW);
      delayMicroseconds(MOTOR_DELAY); 
    }
  }

  // TODO
  void ir_reads_black(IRSensor sensor) {}
  void change_forward_side(Side side) {}

};
