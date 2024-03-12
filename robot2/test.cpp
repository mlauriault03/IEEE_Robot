#ifdef DEBUG

#include <cstdio>
#include <cstdlib>

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

    step_writes_fl = 0;
    step_writes_fr = 0;
    step_writes_bl = 0;
    step_writes_br = 0;

    drive_diff(500, 700, 10);
    // Left wheels should take 10 steps, and right wheels should take 7 steps.
    if ((abs(step_writes_fl - 20) < 3)
            && (abs(step_writes_bl - 20) < 3)
            && (abs(step_writes_br - 14) < 3)
            && (abs(step_writes_fr - 14) < 3)) {
        return 0;
    }
    printf("fl: %d steps; bl: %d steps; fr: %d steps; br: %d steps\n", step_writes_fl, step_writes_bl, step_writes_fr, step_writes_br);
    return -1;
}

int main() {
    if (test_mock_wait_for_micros() < 0) { puts("FAILED: test_mock_wait_for_micros"); }
    if (test_drive_diff() < 0) { puts("FAILED: test_drive_diff"); }
    return 0;
}

#endif
