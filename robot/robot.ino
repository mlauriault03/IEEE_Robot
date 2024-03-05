#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#include "behaviors.h"
#include "robot.h"
#include "hardware.h"

// Move one or more of the drive (stepper) motors one step.
void Vehicle::drive_step(bool fl, bool fr, bool bl, bool br) {
  if (fl) { digitalWrite(FL_DRIVE_STEP, HIGH); }
  if (bl) { digitalWrite(BL_DRIVE_STEP, HIGH); }
  if (fr) { digitalWrite(FR_DRIVE_STEP, HIGH); }
  if (br) { digitalWrite(BR_DRIVE_STEP, HIGH); }
  delayMicroseconds(MOTOR_DELAY);
  if (fl) { digitalWrite(FL_DRIVE_STEP, LOW); }
  if (bl) { digitalWrite(BL_DRIVE_STEP, LOW); }
  if (fr) { digitalWrite(FR_DRIVE_STEP, LOW); }
  if (br) { digitalWrite(BR_DRIVE_STEP, LOW); }
  delayMicroseconds(MOTOR_DELAY); 
  update_servos();
}

// Set all four servos to a specified angle.
// 
// degrees = 30 sets the servos to the "normal" forward position.
// degrees = 150 sets the servos to the fully rotated position (to drive "sideways")
void Vehicle::update_servos() {
  FL_SERVO.write(servo_angle);
  BR_SERVO.write(servo_angle);
  FR_SERVO.write(180 - servo_angle);
  BL_SERVO.write(180 - servo_angle);
}

// Move steering servos to the normal or turned position.
//
// NOTE: This chnages the stepper directions as a side effect.
// Be sure to set them to the desired direction after calling this function.
void Vehicle::move_servos(bool turned) {
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
  int nsteps = (int)(0.25 * STEPS_PER_REV);
  double desired_angle = turned ? 150.0 : 30.0;
  double angle_inc = (desired_angle - servo_angle) / (double)nsteps;
  for (int i = 0; i < nsteps; i++) {
    servo_angle += angle_inc;
    drive_step(true, true, true, true);   // Moves steppers and servos.
  }
  servo_angle = desired_angle;  // Make sure we have the right angle.
}

Vehicle::Vehicle() {
  // Make sure the servos are where we think they are.
  update_servos();
  // Now initialize stepper motor directions.
  change_forward_side(FRONT);
}

void Vehicle::move_forward(int nsteps) {
  for (int i = 0; i < nsteps; i++) {
    drive_step(true, true, true, true);
  }
}

void Vehicle::move_left_wheels(int nsteps) {
  for (int i = 0; i < nsteps; i++) {
    drive_step(true, false, true, false);
  }
}

void Vehicle::move_right_wheels(int nsteps) {
  for (int i = 0; i < nsteps; i++) {
    drive_step(false, true, false, true);
  }
}

void Vehicle::spin(double deg_to_left) {
}

bool Vehicle::ir_reads_black(Direction sensor_direction) {
  int side = (forward_side + sensor_direction) % 360;
  switch (side) {
    case 0:
      return analogRead(FRONT_IR_PIN) == 1;
    case 90:
      return analogRead(LEFT_IR_PIN) == 1;
    case 180:
      return analogRead(BACK_IR_PIN) == 1;
    case 270:
      return analogRead(RIGHT_IR_PIN) == 1;
  }
}

void Vehicle::change_forward_side(Side side) {
  forward_side = side;
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
    digitalWrite(FL_DRIVE_DIR, HIGH);
    digitalWrite(FR_DRIVE_DIR, HIGH);
    digitalWrite(BL_DRIVE_DIR, HIGH);
    digitalWrite(BR_DRIVE_DIR, HIGH);
    break;
  case BACK:
    digitalWrite(FL_DRIVE_DIR, LOW);
    digitalWrite(FR_DRIVE_DIR, LOW);
    digitalWrite(BL_DRIVE_DIR, LOW);
    digitalWrite(BR_DRIVE_DIR, LOW);
    break;
  case LEFT:
    digitalWrite(FL_DRIVE_DIR, LOW);
    digitalWrite(FR_DRIVE_DIR, HIGH);
    digitalWrite(BL_DRIVE_DIR, HIGH);
    digitalWrite(BR_DRIVE_DIR, LOW);
    break;
  case RIGHT:
    digitalWrite(FL_DRIVE_DIR, HIGH);
    digitalWrite(FR_DRIVE_DIR, LOW);
    digitalWrite(BL_DRIVE_DIR, LOW);
    digitalWrite(BR_DRIVE_DIR, HIGH);
    break;
  }
}

void setup() {
  delay(4000);          // Give some time to step away.

  setup_all_motors();
  setup_servos();
  setup_display();

  Vehicle vehicle = Vehicle();
  TestMovement action = TestMovement(vehicle);
  action.run();
}

void loop() {
  while (true) {}
}

