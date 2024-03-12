#include "mechanics.h"
#include <cstdio>

#ifdef DEBUG
#include "mock.h"
#endif

void drive_diff(int left_delay, int right_delay, int nsteps) {
    long start = micros();

    int nsteps_right = 0;
    int nsteps_left = 0;

    bool right_state = true;
    bool left_state = true;
    write_drive(true, false, true, false, right_state);
    write_drive(false, true, false, true, left_state);

    while (nsteps_left < nsteps && nsteps_right < nsteps) {
        long elapsed = micros() - start;
        if (elapsed >= left_delay) {
            right_state = ~right_state;
            write_drive(true, false, true, false, right_state);
            if (right_state) {
                nsteps_left++;
            }
            start = micros();
        }
        if (elapsed >= right_delay) {
            left_state = ~left_state;
            write_drive(false, true, false, true, left_state);
            if (left_state) {
                nsteps_right++;
            }
            start = micros();
        }
    }

    write_drive(true, false, true, false, true);
    write_drive(false, true, false, true, true);
}
