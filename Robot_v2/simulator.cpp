#include <cmath>
#include "robot.h"

class Simulator : Controller {
    public:

    double x, y;

    // Heading is measured in radians counter-clockwise from the x-axis.
    double heading;

    // Steering radius of curvature (steering left = positive)
    double steer_curvature = 0;

    Side forward_side = FRONT;

    double step_distance = 0.1;
    
    Simulator(double _x, double _y, double _heading) {
        x = _x; y = _y; heading = _heading;
    }

    void move_increment() {
        // x_prime is the distance the robot moved forward (relative to its initial position).
        // y_prime is the distance the robot moved left (relative to its initial position).
        double r = 1/steer_curvature;
        double x_prime = r * cos(step_distance / r);
        double y_prime = r * (1 - sin(step_distance / r));
        // Rotate (clockwise) into the global coordinate system.
        x += cos(-heading) * x_prime - sin(-heading) * y_prime;
        y += sin(-heading) * x_prime + cos(-heading) * y_prime;
    }

    void set_steer_curvature(double value) {
        steer_curvature = value;
    }

    void change_forward_side(Side side) {
        heading += (side - forward_side)*3.14159/180;
        forward_side = side;
    }
    
    void ir_reads_black(IRSensor sensor) {
        // TODO
    }
};


