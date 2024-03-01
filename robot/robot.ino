#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#include "robot.h"
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
  // Represents the position of the steering servos.
  // 30 = Default "forward" position.
  // 90 = About half-way rotated.
  // 150 = Rotated 90 degrees for driving "sideways."
  int servo_angle = 30;

  // Move one or more of the drive (stepper) motors one step.
  void drive_step(bool fl, bool fr, bool bl, bool br) {
      if (fl) { digitalWrite(FL_DRIVE_STEP, HIGH); }
      if (bl) { digitalWrite(BL_DRIVE_STEP, HIGH); }
      if (fr) { digitalWrite(FR_DRIVE_STEP, HIGH); }
      if (br) { digitalWrite(BR_DRIVE_STEP, HIGH); }
      delayMicroseconds(MOTOR_DELAY);                            //Was a 500 microsecond delay
      if (fl) { digitalWrite(FL_DRIVE_STEP, LOW); }
      if (bl) { digitalWrite(BL_DRIVE_STEP, LOW); }
      if (fr) { digitalWrite(FR_DRIVE_STEP, LOW); }
      if (br) { digitalWrite(BR_DRIVE_STEP, LOW); }
      delayMicroseconds(MOTOR_DELAY); 
      update_servos();
  }

  // Set all four servos to a specified angle.
  // 
  // degrees = 0 sets the servos to the "normal" forward position.
  // degrees = 90 sets the servos to the fully rotated position (to drive "sideways")
  void update_servos() {
    FR_SERVO.write(servo_angle);
    FL_SERVO.write(180 - servo_angle);
    BR_SERVO.write(180 - servo_angle);
    BL_SERVO.write(servo_angle);
  }

  // Move steering servos to the normal or turned position.
  //
  // NOTE: This chnages the stepper directions as a side effect.
  // Be sure to set them to the desired direction after calling this function.
  void move_servos(bool turned) {
    if (turned) {
      digitalWrite(FL_DRIVE_DIR, LOW);
      digitalWrite(FR_DRIVE_DIR, HIGH);
      digitalWrite(BL_DRIVE_DIR, HIGH);
      digitalWrite(BR_DRIVE_DIR, LOW);
    } else {
      digitalWrite(FL_DRIVE_DIR, HIGH);
      digitalWrite(FR_DRIVE_DIR, LOW);
      digitalWrite(BL_DRIVE_DIR, LOW);
      digitalWrite(BR_DRIVE_DIR, HIGH);
    }

    // Assume the servos and the wheels have approximately the same radius.
    int nsteps = (int)(0.25 / REV_PER_STEP);
    double desired_angle = turned ? 150.0 : 30.0;
    double angle_inc = (desired_angle - servo_angle) / (double)nsteps;
    for (int i = 0; i < nsteps; i++) {
      servo_angle += angle_inc;
      drive_step(true, true, true, true);   // Moves steppers and servos.
    }
  }

public:

  Hardware() {
    // Make sure the servos are where we think they are.
    update_servos();
    // Now initialize stepper motor directions.
    change_forward_side(FRONT);
  }

  void move_forward(int nsteps) {
    for (int i = 0; i < nsteps; i++) {
      drive_step(true, true, true, true);
    }
  }

  void move_left_wheels(int nsteps) {
    for (int i = 0; i < nsteps; i++) {
      drive_step(true, false, true, false);
    }
  }

  void move_right_wheels(int nsteps) {
    for (int i = 0; i < nsteps; i++) {
      drive_step(false, true, false, true);
    }
  }

  void spin(double deg_to_left) {
  }

  bool ir_reads_black(Side sensor_side) {
    switch (sensor_side) {
      case BACK:
        return analogRead(A1) == 1;
      case LEFT:
        return analogRead(A2) == 1;
      case RIGHT:
        return analogRead(A3) == 1;
      case FRONT:
        return analogRead(A3) == 1;
    }
  }

  void change_forward_side(Side side) {
    // Move the steering servos.
    if (side == FRONT || side == BACK) {
      move_servos(false);
    }
    else {
      move_servos(true);
    }
    // Update stepper directions.
    switch (side) {
    case FRONT:
      digitalWrite(FL_DRIVE_DIR, LOW);
      digitalWrite(FR_DRIVE_DIR, HIGH);
      digitalWrite(BL_DRIVE_DIR, LOW);
      digitalWrite(BR_DRIVE_DIR, HIGH);
      break;
    case BACK:
      digitalWrite(FL_DRIVE_DIR, HIGH);
      digitalWrite(FR_DRIVE_DIR, LOW);
      digitalWrite(BL_DRIVE_DIR, HIGH);
      digitalWrite(BR_DRIVE_DIR, LOW);
      break;
    case LEFT:
      digitalWrite(FL_DRIVE_DIR, HIGH);
      digitalWrite(FR_DRIVE_DIR, HIGH);
      digitalWrite(BL_DRIVE_DIR, LOW);
      digitalWrite(BR_DRIVE_DIR, LOW);
      break;
    case RIGHT:
      digitalWrite(FL_DRIVE_DIR, LOW);
      digitalWrite(FR_DRIVE_DIR, LOW);
      digitalWrite(BL_DRIVE_DIR, HIGH);
      digitalWrite(BR_DRIVE_DIR, HIGH);
      break;
    }
  }

};
