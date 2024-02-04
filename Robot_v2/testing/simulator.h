#ifndef IEEE_ROBOT_2024_SIMULATOR
#define IEEE_ROBOT_2024_SIMULATOR

#include <utility>
#include <vector>
#include "../robot.h"

class Simulator : public VehicleInterface {

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

public:
    std::vector<std::pair<double, double>> location_history;

    void set_position(double x, double y, double heading);
    
    // Inherited methods to override
    void move_increment();
    void set_steer_curvature(double value);
    void change_forward_side(Side side);
    bool ir_reads_black(IRSensor sensor);
};

#endif
