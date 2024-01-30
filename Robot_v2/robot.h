enum Side {FRONT, BACK, LEFT, RIGHT};
enum IRSensor {BACK_LEFT, BACK_RIGHT, SIDE_FRONT, SIDE_BACK};

// This class is responsible for the robot's logic.
class RobotModel {
    void start();   // Start the robot.
};

// This class provides a basic interface to the robot's hardware.
class Controller {
    void set_steer(double radians);     // Set differential steering, in radians left from center
    void change_direction(Side side);   // Rotate wheels to change "forward" direction
    void move_increment();              // Move a tiny step forward.
    bool ir_reads_black(IRSensor sensor);   // Determine whether an infrared sensor reads black.
};
