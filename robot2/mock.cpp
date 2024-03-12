#ifdef DEBUG
// Mock Arduino-specific functions for use of testing.

#include <chrono>
#include <thread>
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

int nsteps_fl = 0;
int nsteps_fr = 0;
int nsteps_bl = 0;
int nsteps_br = 0;


// Functions from the Arduino library

// Function to get the number of milliseconds since program started running (ChatGPT)
unsigned long millis() {
    static const auto start_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
}

unsigned long micros() {
    static const auto start_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
}

void delay(unsigned long ms) {
    unsigned long start = millis();
    while (millis() - start < ms);
}

void delayMicroseconds(unsigned int us) {
    unsigned long start = micros();
    while (micros() - start < us);
}


// Custom functions

void write_drive(bool fl, bool fr, bool bl, bool br, bool value) {
  if (fl) { fl_drive_state = value; nsteps_fl++; }
  if (bl) { bl_drive_state = value; nsteps_bl++; }
  if (fr) { fr_drive_state = value; nsteps_fr++; }
  if (br) { br_drive_state = value; nsteps_br++; }
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
