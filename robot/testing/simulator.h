#ifndef IEEE_ROBOT_2024_SIMULATOR
#define IEEE_ROBOT_2024_SIMULATOR

#include <utility>
#include <vector>
#include "../autonomy.h"

class Simulator : public VehicleInterface {

    double x, y;

    // Heading is measured in radians counter-clockwise from the x-axis.
    double heading;

    // Steering radius of curvature (steering left = positive)
    double steer_curvature = 0;

    Side forward_side = FRONT;

    double step_distance = 0.01;    // On the robot, 1 step is about 1.2 inches

    // The positions of the infrared sensors WRT the robot's center.
    // x increases toward the front,  and y increases toward the left.
    double ir_fl_x = 5.0;
    double ir_fl_y = 1.0;
    double ir_fr_x = 5.0;
    double ir_fr_y = -1.0;
    double ir_sf_x = 1.0;
    double ir_sf_y = 5.0;
    double ir_sb_x = -1.0;
    double ir_sb_y = 5.0;

    double stripe_width = 2.0;


public:
    std::vector<std::pair<double, double>> location_history;
    std::pair<double, double> sensor_location(IRSensor sensor);

    void set_position(double x, double y, double heading);
    
    // Inherited methods to override
    void move_increment();
    void set_steer_curvature(double value);
    void change_forward_side(Side side);
    bool ir_reads_black(IRSensor sensor);
};

#endif
