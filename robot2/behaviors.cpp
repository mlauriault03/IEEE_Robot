#include <Arduino.h>

#include "behaviors.h"
#include "robot2.h"
#include "mechanics.h"

class Timer {
    unsigned long duration;
    unsigned long start;

    public:

    Timer(unsigned long us) : duration(us) {
        start = micros();
    };

    bool is_done() {
        return micros() - start >= duration;
    }
};

namespace FollowLine {

    Timer stop_timer(0);

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

    int run(double cm) {
        stop_timer = Timer(cm * STEPS_PER_CM * 2 * MOTOR_DELAY);
        int status = where_on_line();

        while (!stop_timer.is_done()) {
            switch (status) {
                case LOST_LINE:
                    status = go_forward();
                    break;
                case SUCCESS:
                    status = go_forward();
                    break;
                case LEFT_OF_LINE:
                    status = go_right();
                    break;
                case RIGHT_OF_LINE:
                    status = go_left();
                    break;
            }
        }

        return status;
    }

    bool stop_forward() {
        return stop_timer.is_done() || where_on_line() != SUCCESS;
    }

    bool stop_right() {
        return stop_timer.is_done() || where_on_line() != LEFT_OF_LINE;
    }

    bool stop_left() {
        return stop_timer.is_done() || where_on_line() != RIGHT_OF_LINE;
    }

    int go_forward() {
        diff_drive_until(MOTOR_DELAY, MOTOR_DELAY, stop_forward);
        return where_on_line();
    }

    int go_right() {
        diff_drive_until(MOTOR_DELAY*0.9, MOTOR_DELAY, stop_right);
        return where_on_line();
    }

    int go_left() {
        diff_drive_until(MOTOR_DELAY, MOTOR_DELAY*0.9, stop_left);
        return where_on_line();
    }

}

namespace Course {
    void run() {
        FollowLine::run(194.6275);
        change_forward_side(LEFT);
        FollowLine::run(60.96);
        change_forward_side(BACK);
        FollowLine::run(81.25);
    }
}

namespace Tests {
    void test_diff_drive() {
        drive_diff(MOTOR_DELAY, MOTOR_DELAY*1.2, 1000);
        drive_diff(MOTOR_DELAY*1.2, MOTOR_DELAY, 1000);
    }
}
