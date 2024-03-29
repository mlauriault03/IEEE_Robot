#ifndef IEEE_ROBOT_MECHANICS_4789326
#define IEEE_ROBOT_MECHANICS_4789326

#include "robot2.h"

// Follow a curved path using differential drive.
//
// left_delay: The microsecond delay for the left steppers.
// right_delay: The microsecnd delay for the right steppers.
// nsteps: The maximum number of steps a motor will take.
void drive_diff(unsigned long left_delay, unsigned long right_delay, int nsteps);

void step_in_time(bool left, unsigned long period,
        unsigned long &last_stepped, bool &last_write);

void diff_drive_until(unsigned long left_delay,
        unsigned long right_delay, bool stop_condition());

#endif
