#include <cmath>
#include <iostream>     // for debugging
#include <utility>
#include "../robot.h"
#include "simulator.h"

void Simulator::set_position(double _x, double _y, double _heading) {
    x = _x; y = _y; heading = _heading;
    std::pair point(x, y);
    location_history.push_back(point);
}

void Simulator::move_increment() {
    // dx_prime is the distance the robot moved forward (relative to its initial position).
    // dy_prime is the distance the robot moved left (relative to its initial position).
    double dx_prime;
    double dy_prime;
    double theta = step_distance * steer_curvature;
    if (steer_curvature < 0.1) {
        // We're essentially going straight.
        dx_prime = step_distance;
        dy_prime = 0.0;
    }
    else {
        dx_prime = sin(theta)/steer_curvature;
        dy_prime = (1 - cos(theta)) / steer_curvature;
    }
    // Rotate (clockwise) into the global coordinate system.
    x += cos(heading) * dx_prime - sin(heading) * dy_prime;
    y += sin(heading) * dx_prime + cos(heading) * dy_prime;
    heading += theta;

    // Record new location
    std::pair point(x, y);
    location_history.push_back(point);
}

void Simulator::set_steer_curvature(double value) {
    steer_curvature = value;
}

void Simulator::change_forward_side(Side side) {
    heading += (side - forward_side)*3.14159/180;
    forward_side = side;
}

bool Simulator::ir_reads_black(IRSensor sensor) {
    // Check to see if we're on the center stripe.
    auto point = sensor_location(sensor);
    return (point.second > 1.0) || (point.second < -1.0);
}


std::pair<double, double> Simulator::sensor_location(IRSensor sensor) {
    // Temporarily change the heading so that forward=FRONT.
    Side old_fwd = forward_side;
    change_forward_side(FRONT);

    // find (x', y'), the position of the sensor wrt the robot's sensor.
    double ir_x;
    double ir_y;
    switch (sensor)
    {
    case FRONT:
        ir_x = IR_FRONT_X;
        ir_y = IR_FRONT_Y;
        break;
    case BACK:
        ir_x = IR_BACK_X;
        ir_y = IR_BACK_Y;
        break;
    case LEFT:
        ir_x = IR_LEFT_X;
        ir_y = IR_LEFT_Y;
        break;
    case RIGHT:
        ir_x = IR_RIGHT_X;
        ir_y = IR_RIGHT_Y;
        break;
    }

    // restore the "forward" side.
    change_forward_side(old_fwd);
    
    std::pair<double, double> point(ir_x + x, ir_y + y);
    return point;
}

