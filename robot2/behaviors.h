#ifndef IGVC2024_BEHAVIORS_402732
#define IGVC2024_BEHAVIORS_402732

namespace FollowLine {

    int run();

    int go_forward();

    int go_left();

    int go_right();

    enum Status { SUCCESS, LOST_LINE, LEFT_OF_LINE, RIGHT_OF_LINE,
        END_OF_LINE };
};

namespace Tests {
    void test_diff_drive();
}

#endif
