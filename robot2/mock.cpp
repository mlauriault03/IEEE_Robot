#ifdef DEBUG
// Mock Arduino-specific functions for use of testing.
#include "robot2.h"

// Global variables
unsigned long global_milliseconds = 0;
unsigned int global_microseconds = 0;
int servo_angle = 30;
Side forward_side;

bool fl_drive_state;
bool fr_drive_state;
bool bl_drive_state;
bool br_drive_state;
bool fl_direction_state;
bool fr_direction_state;
bool bl_direction_state;
bool br_direction_state;


// Functions from the Arduino library

void delay(unsigned long ms) {
    global_milliseconds += ms;
}

void delayMicroseconds(unsigned int us) {
    global_microseconds += us;
    global_milliseconds += global_microseconds / 1000;
    global_microseconds %= 1000;
}

unsigned long millis() {
    return global_milliseconds;
}

// Custom functions

void write_drive(bool fl, bool fr, bool bl, bool br, bool value) {
  if (fl) { fl_drive_state = value; }
  if (bl) { bl_drive_state = value; }
  if (fr) { fr_drive_state = value; }
  if (br) { br_drive_state = value; }
}

void update_servos() {}

void move_servos(bool turned) {
  int nsteps = (int)(0.25 * STEPS_PER_REV);
  double desired_angle = turned ? 150.0 : 30.0;
  double angle_inc = (desired_angle - servo_angle) / (double)nsteps;
  for (int i = 0; i < nsteps; i++) {
    drive_step(true, true, true, true);
  }
  servo_angle = desired_angle;
}

void drive_step(bool fl, bool fr, bool bl, bool br) {
  write_drive(fl, fr, bl, br, true);
  delayMicroseconds(MOTOR_DELAY);
  write_drive(fl, fr, bl, br, false);
  delayMicroseconds(MOTOR_DELAY); 
  update_servos();
}

void change_forward_side(Side side) {
  forward_side = side;
  if (side == FRONT || side == BACK) {
    move_servos(false);
  }
  else {
    move_servos(true);
  }
}

#endif
