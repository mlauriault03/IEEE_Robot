// Enum values represent degrees coutnerclockwise from FRONT.
enum Side {FRONT=0, LEFT=90, BACK=180, RIGHT=270};

enum IRSensor {BACK_LEFT, BACK_RIGHT, SIDE_FRONT, SIDE_BACK};

// This class is responsible for the robot's logic.
class RobotModel {
    void start();   // Start the robot.
};

// This class provides a basic interface to the robot's hardware.
class Controller {
    void set_steer_curvature(double radians);  // Set curvature for differential steering. Left is positive.
    void change_forward_side(Side side); // Rotate wheels to change "forward" direction
    void move_increment();               // Move a tiny step forward.
    bool ir_reads_black(IRSensor sensor);   // Determine whether an infrared sensor reads black.
};
