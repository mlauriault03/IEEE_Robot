#include <cmath>
#include "robot.h"

class Simulator : Controller {
    public:

    double x, y;
    // Heading is measured in radians counter-clockwise from the x-axis.
    double heading;
    // Steering radius of curvature (steering left = positive)
    double steer = 0;

    double step_distance = 0.1;
    
    Simulator(double _x, double _y, double _heading) {
        x = _x; y = _y; heading = _heading;
    }

    void move_increment() {
        // x_prime and y_prime represents the motion in the robot's initial frame
        // of reference, where the x-axis is the direciton the robot was facing.
        double x_prime = steer * sin(step_distance / steer);
        double y_prime = steer * (1 - cos(step_distance / steer));
        // Rotate (clockwise) into the global coordinate system.
        x += cos(-heading) * x_prime - sin(-heading) * y_prime;
        y += sin(-heading) * x_prime + cos(-heading) * y_prime;
    }

    void set_steer(double value) {
        steer = value;
    }

    void change_direction(Side side) {
        // TODO
    }
    
    void ir_reads_black(IRSensor sensor) {
        // TODO
    }
};


