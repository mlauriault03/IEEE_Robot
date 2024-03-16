#ifdef DEBUG

#include <cstdio>
#include <cstdlib>

#include "robot2.h"
#include "mechanics.h"
#include "mock.h"

#define UNIT_TEST(TESTFUNC, TESTNAME) printf("Running %s\n", (TESTNAME)); \
    if (!(TESTFUNC())) printf("\nFailed: %s\n\n", (TESTNAME))

unsigned long global_time_ref = 0;

void init_steppers(Side forward) {
    change_forward_side(forward);
    step_writes_fl = 0;
    step_writes_fr = 0;
    step_writes_bl = 0;
    step_writes_br = 0;
}

bool assert_steps(int fl, int fr, int bl, int br) {
    if ((step_writes_bl == bl) && (step_writes_br == br)
            && (step_writes_fl == fl) && (step_writes_fr == fr)) {
        return true;
    }
    printf("step_writes_fl=%d; step_writes_fr=%d; "
            "step_writes_bl=%d; step_writes_br=%d;\n",
            step_writes_fl, step_writes_fr, step_writes_bl,
            step_writes_br);
    return false;
}

bool test_mock_wait_for_micros() {
    long start = micros();
    while (micros() - start < 100);
    return true;
}

bool test_step_in_time_left_5writes() {
    init_steppers(FRONT);
    unsigned long start = micros();
    unsigned long last_step = micros();
    bool last_write = true;
    while (micros() - start < 1000) {
        step_in_time(true, 195, last_step, last_write);
    }
    return assert_steps(5, 0, 5, 0);
}

bool test_step_in_time_forward_is_right() {
    init_steppers(RIGHT);
    unsigned long start = micros();
    unsigned long last_step = micros();
    bool last_write = true;
    while (micros() - start < 1000) {
        step_in_time(true, 195, last_step, last_write);
    }
    return assert_steps(5, 5, 0, 0);
}

bool is_1000_past_ref() {
    return micros() - global_time_ref >= 1000;
}

bool test_diff_drive_until() {
    init_steppers(FRONT);
    global_time_ref = micros();
    diff_drive_until(195, 295, is_1000_past_ref);
    return assert_steps(5, 3, 5, 3);
}

int main() {
    UNIT_TEST(test_mock_wait_for_micros, "test_mock_wait_for_micros");

    UNIT_TEST(test_step_in_time_left_5writes,
            "test_step_in_time_left_5writes");

    UNIT_TEST(test_diff_drive_until, "test_diff_drive_until");

    UNIT_TEST(test_step_in_time_forward_is_right,
            "test_step_in_time_forward_is_right");

    return 0;
}

#endif
