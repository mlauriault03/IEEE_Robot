#ifndef IEEE_ROBOT_2024
#define IEEE_ROBOT_2024

// Side is an absolute reference to a side of the robot.
// In other words, FRONT always refers to the same side of the robot,
// no matter which way the steering servos are turned.
//
// Values represent degrees coutnerclockwise from FRONT.
enum Side {FRONT=0, LEFT=90, BACK=180, RIGHT=270};

// Direction is a relative reference to a side of the robot
// that changes with the steering servos.
//
// FORWARD is the direction we are currently driving, and the
// other directions are defined accordingly.
enum Direction {FORWARD, LEFTWARD, RIGHTWARD, BACKWARD};

// This class provides a basic interface to the robot's hardware.
class VehicleInterface {
public:
    void move_forward(int nsteps);              // Move forward.
    void move_left_wheels(int nsteps);          // Move left wheels only.
    void move_right_wheels(int nsteps);         // Move right wheels only.

    void spin(double deg_to_left);              // Do in-place rotation by differential drive.

    void change_forward_side(Side side);        // Rotate servos to change "forward" direction
    bool ir_reads_black(Direction side);        // Determine whether an infrared sensor reads black.
};

#endif
