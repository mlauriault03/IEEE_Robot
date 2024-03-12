#ifndef IGVC2024_BEHAVIORS_402732
#define IGVC2024_BEHAVIORS_402732

namespace FollowLine {

    int run();

    enum Result { SUCCESS, LOST_LINE };
};

namespace ForwardOnLine {
    int run();

    enum Result { SUCCESS, LOST_LINE, LEFT_OF_LINE, RIGHT_OF_LINE,
        END_OF_LINE };
};

namespace LeftTowardLine {
    int run();

    enum Result { SUCCESS, LOST_LINE, LEFT_OF_LINE, RIGHT_OF_LINE,
        END_OF_LINE };
};

namespace RightTowardLine {
    int run();

    enum Result { SUCCESS, LOST_LINE, LEFT_OF_LINE, RIGHT_OF_LINE,
        END_OF_LINE };
};

#endif
