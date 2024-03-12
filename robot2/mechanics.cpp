#include "mechanics.h"
#include "robot2.h"

#ifdef DEBUG
#include <cstdio>
#include "mock.h"
#endif

#ifndef DEBUG
#include <Arduino.h>
#endif

void drive_diff(unsigned long left_delay, unsigned long right_delay, int nsteps) {
    unsigned long start_left = micros();
    unsigned long start_right = micros();

    int nsteps_right = 0;
    int nsteps_left = 0;

    bool right_state = true;
    bool left_state = true;
    write_drive(true, false, true, false, right_state);
    write_drive(false, true, false, true, left_state);

    while (nsteps_left < nsteps && nsteps_right < nsteps) {
        unsigned long t_left = micros() - start_left;
        unsigned long t_right = micros() - start_right;
        if (t_left >= left_delay) {
            left_state = !left_state;
            write_drive(true, false, true, false, right_state);
            if (left_state) {
                nsteps_left++;
            }
            start_left = micros();
        }
        if (t_right >= right_delay) {
            right_state = !right_state;
            write_drive(false, true, false, true, left_state);
            if (right_state) {
                nsteps_right++;
            }
            start_right = micros();
        }
    }

    write_drive(true, false, true, false, true);
    write_drive(false, true, false, true, true);
}
