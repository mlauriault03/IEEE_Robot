#include "behaviors.h"
#include "robot2.h"

int FollowLine::run() {
    bool see_right = ir_reads_black(RIGHTWARD);
    bool see_left = ir_reads_black(LEFTWARD);
    if (see_right && see_left) {
        int result = ForwardOnLine::run();
    }
}

int ForwardOnLine::run() {

}

int LeftTowardLine::run() {

}

int RightTowardLine::run() {

}
