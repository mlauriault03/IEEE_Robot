#ifdef DEBUG

#include <cstdio>

#include "robot2.h"
#include "mechanics.h"
#include "mock.h"

int test_mock_wait_for_micros() {
    long start = micros();
    while (micros() - start < 100);
    return 0;
}

int test_drive_diff() {
    change_forward_side(FRONT);
    drive_diff(500, 700, 10);
    // Left wheels should take 10 steps, and right wheels should take 7 steps.
    if (nsteps_fl == 10 && nsteps_bl == 10 && nsteps_br == 7 && nsteps_fr == 7) {
        return 0;
    }
    printf("fl: %d steps; bl: %d steps; br: %d steps; fr: %d steps\n", nsteps_fl, nsteps_bl, nsteps_fr, nsteps_fr);
    return -1;
}

int main() {
    if (test_mock_wait_for_micros() < 0) { puts("FAILED: test_mock_wait_for_micros"); }
    if (test_drive_diff() < 0) { puts("FAILED: test_drive_diff"); }
    return 0;
}

#endif
