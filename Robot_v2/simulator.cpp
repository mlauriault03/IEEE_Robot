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

    // The positions of the infrared sensors WRT the robot's center.
    // x increases toward the front,  and y increases toward the left.
    double ir_bl_x = -10.0;
    double ir_bl_y = 1.0;
    double ir_br_x = -10.0;
    double ir_br_y = -1.0;
    double ir_sf_x = 1.0;
    double ir_sf_y = 10.0;
    double ir_sb_x = -1.0;
    double ir_sb_y = 10.0;

    double stripe_width = 2.0;
    
    Simulator(double _x, double _y, double _heading) {
        x = _x; y = _y; heading = _heading;
    }

    void move_increment() {
        // dx_prime is the distance the robot moved forward (relative to its initial position).
        // dy_prime is the distance the robot moved left (relative to its initial position).
        double dx_prime;
        double dy_prime;
        double theta = step_distance * steer_curvature;
        if (steer_curvature < 0.01) {
            // We're essentially going straight.
            dx_prime = step_distance;
            dy_prime = 0.0;
        }
        else {
            dx_prime = cos(theta)/steer_curvature;
            dy_prime = (1 - sin(theta)) / steer_curvature;
        }
        // Rotate (clockwise) into the global coordinate system.
        x += cos(-heading) * dx_prime - sin(-heading) * dy_prime;
        y += sin(-heading) * dx_prime + cos(-heading) * dy_prime;
        heading += theta;
    }

    void set_steer_curvature(double value) {
        steer_curvature = value;
    }

    void change_forward_side(Side side) {
        heading += (side - forward_side)*3.14159/180;
        forward_side = side;
    }
    
    bool ir_reads_black(IRSensor sensor) {
        // Temporarily change the heading so that forward=FRONT.
        Side old_fwd = forward_side;
        change_forward_side(FRONT);

        // Find (x', y'), the position of the sensor WRT the robot's sensor.
        double x_prime;
        double y_prime;
        switch (sensor)
        {
        case BACK_LEFT:
            x_prime = ir_bl_x;
            y_prime = ir_bl_y;
            break;
        case BACK_RIGHT:
            x_prime = ir_br_x;
            y_prime = ir_br_y;
            break;
        case SIDE_FRONT:
            x_prime = ir_sf_x;
            y_prime = ir_sf_y;
            break;
        case SIDE_BACK:
            x_prime = ir_sb_x;
            y_prime = ir_sb_y;
            break;
        }

        // Rotate (x', y') into the global coordinate frame.
        double ir_x = cos(-heading) * x_prime - sin(-heading) * y_prime;
        double ir_y = sin(-heading) * x_prime + cos(-heading) * y_prime;

        // Restore the "forward" side.
        change_forward_side(old_fwd);

        // Check to see if we're on the center stripe.
        return abs(ir_y) < 10;
    }
};
