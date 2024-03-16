#include "mechanics.h"
#include "robot2.h"

#ifdef DEBUG
#include <cstdio>
#include "mock.h"
#endif

#ifndef DEBUG
#include <Arduino.h>
#endif

// Write to the "Left" or "Right" motors, as defined by the
// direction the robot is driving.
void write_drive_rel(bool leftward, bool value) {
    bool fl, fr, bl, br;
    // Get motors for "leftward" side.
    switch (forward_side) {
        case FRONT:
            fl = true; bl = true; fr = false; br = false;
            break;
        case BACK:
            fl = false; bl = false; fr = true; br = true;
            break;
        case LEFT:
            fl = false; bl = true; fr = false; br = true;
            break;
        case RIGHT:
            fl = true; bl = false; fr = true; br = false;
            break;
    }
    if (!leftward) {
        fl = !fl; bl = !bl; fr = !fr; br = !br;
    }
    write_drive(fl, fr, bl, br, value);
}

void step_in_time(bool left, unsigned long period, unsigned long &last_stepped, bool &last_write) {
    unsigned long now = micros();
    if (now > last_stepped && now - last_stepped >= period) {
        last_write = !last_write;
        write_drive_rel(left, last_write);
        last_stepped = now;
    }
}

void diff_drive_until(unsigned long left_delay, unsigned long right_delay, bool stop_condition()) {
    unsigned long last_step_left = micros();
    unsigned long last_step_right = micros();
    bool left_state = true;
    bool right_state = true;

    while (!stop_condition()) {
        step_in_time(true, left_delay, last_step_left, left_state);
        step_in_time(false, right_delay, last_step_right, right_state);
    }
}
