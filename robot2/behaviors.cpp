#include "behaviors.h"
#include "robot2.h"
#include "mechanics.h"

namespace FollowLine {

    int where_on_line() {
        bool see_right = !ir_reads_black(RIGHTWARD);
        bool see_left = !ir_reads_black(LEFTWARD);

        if (see_right && see_left) {
            return SUCCESS;
        }
        else if (see_right) {
            return LEFT_OF_LINE;
        }
        else if (see_left) {
            return RIGHT_OF_LINE;
        }
        else {
            return SUCCESS;
        }
    }

    int run() {
        int status = SUCCESS;

        status = where_on_line();

        while (true) {
            switch (status) {
                case SUCCESS:
                    status = go_forward();
                    break;
                case LEFT_OF_LINE:
                    status = go_right();
                    break;
                case RIGHT_OF_LINE:
                    status = go_left();
                    break;
                case LOST_LINE:
                    return LOST_LINE;
            }
        }

        return status;
    }

    int go_forward() {
        while (!ir_reads_black(LEFTWARD) && !ir_reads_black(RIGHTWARD)) {
            drive_diff(MOTOR_DELAY, MOTOR_DELAY, 100);
        }
        return where_on_line();
    }

    int go_right() {
        while (ir_reads_black(LEFTWARD) && !ir_reads_black(RIGHTWARD)) {
            drive_diff(MOTOR_DELAY*0.9, MOTOR_DELAY, 100);
        }
        return where_on_line();
    }

    int go_left() {
        while (!ir_reads_black(LEFTWARD) && ir_reads_black(RIGHTWARD)) {
            drive_diff(MOTOR_DELAY, MOTOR_DELAY*0.9, 100);
        }
        return where_on_line();
    }

}

namespace Tests {
    void test_diff_drive() {
        drive_diff(MOTOR_DELAY, MOTOR_DELAY*1.2, 1000);
        drive_diff(MOTOR_DELAY*1.2, MOTOR_DELAY, 1000);
    }
}
