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

void Vehicle::write_drive(bool fl, bool fr, bool bl, bool br, bool value) {
  if (fl) { digitalWrite(FL_DRIVE_STEP, value); }
  if (bl) { digitalWrite(BL_DRIVE_STEP, value); }
  if (fr) { digitalWrite(FR_DRIVE_STEP, value); }
  if (br) { digitalWrite(BR_DRIVE_STEP, value); }
}

// Send HIGH or LOW to the left or right drive motors.
// "Left" and "right" are defined relative to the forward_side.
void Vehicle::write_drive(bool left, bool value) {
  bool fl, fr, bl, br;

  // Assume we want the right wheels (relative to forward_side).
  switch (forward_side) {
    case (FRONT):
      // "right" = true right
      fl = false; fr = true;  bl = false; br = true;
      break;
    case (LEFT):
      // "right" = true back
      fl = false; fr = false; bl = true;  br = true;
      break;
    case (BACK):
      // "right" = true left
      fl = true;  fr = false; bl = true;  br = false;
      break;
    case (RIGHT):
      // "right" = true front
      fl = true;  fr = true;  bl = false; br = false;
      break;
  }

  if (fl) { digitalWrite(FL_DRIVE_STEP, value); }
  if (bl) { digitalWrite(BL_DRIVE_STEP, value); }
  if (fr) { digitalWrite(FR_DRIVE_STEP, value); }
  if (br) { digitalWrite(BR_DRIVE_STEP, value); }
}

// If the wheels on one side of the robot step with base_delay,
// find the delay for the other set of wheels if the robot is to
// drive with the given curvature.
//
// The value returned should be less than base_delay. So use base_delay
// for the side moving slower and the result of this function for the
// side moving faster.
//
// The value returned will be no smaller than 250.
int Vehicle::delay_for_curvature(int base_delay, double curvature) {
  // Only accept positive curvature;
  if (curvature < 0) {curvature = -curvature;}
  int extended_delay = (int)(base_delay / (1 + TRACK*curvature));

  // Just to be safe, set minimum values for the delay.
  if (extended_delay < 250) {
    return 250;
  }
  return extended_delay;
}

// Drive a specified distance with the given curvature.
//
// Curvature is measured in 1/in. A positive curvature curves to the left.
void Vehicle::drive(double inches, double curv) {
  int left_delay, right_delay;
  if (curv > 0) {
    left_delay = MOTOR_DELAY;
    right_delay = delay_for_curvature(left_delay, curv);
  } else {
    right_delay = MOTOR_DELAY;
    left_delay = delay_for_curvature(right_delay, curv);
  }

  int total_time = inches * STEPS_PER_INCH * MOTOR_DELAY;
  int t = 0;
  int next_left = 0;
  int next_right = 0;
  bool left_state = LOW;
  bool right_state = LOW;

  while (t < total_time) {
    bool move_left = next_left <= next_right;
    bool move_right = next_right <= next_left;
    
    int wait_time;
    if (move_left) {
      wait_time = next_left - t;
      next_left += left_delay;
      left_state = !left_state;
      write_drive(true, left_state);
    }
    if (move_right) {
      wait_time = next_right - t;
      next_right += right_delay;
      right_state = !right_state;
      write_drive(false, right_state);
    }

    delay(wait_time);
    t += wait_time;
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

