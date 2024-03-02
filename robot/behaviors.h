#ifndef IGVC2024_BEHAVIORS_402732
#define IGVC2024_BEHAVIORS_402732

#include "robot.h"

class Behavior {
public:
    Hardware& vehicle;

    Behavior(Hardware& v) : vehicle(v) {}

    int run();
};

class FollowLine : public Behavior {
    using Behavior::Behavior;

public:
    int run();

    enum Result { SUCCESS, LOST_LINE };
};

class ForwardOnLine : public Behavior {
    using Behavior::Behavior;

public:
    int run();

    enum Result { SUCCESS, LOST_LINE, LEFT_OF_LINE, RIGHT_OF_LINE };
};

class LeftTowardLine : public Behavior {
    using Behavior::Behavior;

public:
    int run();

    enum Result { SUCCESS, LOST_LINE, LEFT_OF_LINE, RIGHT_OF_LINE };
};

class RightTowardLine : public Behavior {
    using Behavior::Behavior;

public:
    int run();

    enum Result { SUCCESS, LOST_LINE, LEFT_OF_LINE, RIGHT_OF_LINE };
};

class TestMovement : public Behavior {
    using Behavior::Behavior;

public:
    int run();

    enum Result { SUCCESS };
};

#endif
