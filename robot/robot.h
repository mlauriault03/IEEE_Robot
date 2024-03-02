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
//
// Values represent degress coutnerclockwise from FORWARD.
enum Direction {FORWARD=0, LEFTWARD=90, BACKWARD=180, RIGHTWARD=270};

// This class provides a basic interface to the robot's hardware.
class Hardware {
    // Represents the position of the steering servos.
    // 30 = Default "forward" position.
    // 90 = About half-way rotated.
    // 150 = Rotated 90 degrees for driving "sideways."
    int servo_angle = 30;

    Side forward_side = FRONT;

    void drive_step(bool fl, bool fr, bool bl, bool br);
    void update_servos();
    void move_servos(bool turned);

public:
    Hardware();

    void move_forward(int nsteps);              // Move forward.
    void move_left_wheels(int nsteps);          // Move left wheels only.
    void move_right_wheels(int nsteps);         // Move right wheels only.

    void spin(double deg_to_left);              // Do in-place rotation by differential drive.

    void change_forward_side(Side side);        // Rotate servos to change "forward" direction
    bool ir_reads_black(Direction side);        // Determine whether an infrared sensor reads black.
};

#endif
