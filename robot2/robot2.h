#ifndef THE_MOST_AWESOME_ROBOT
#define THE_MOST_AWESOME_ROBOT

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

const int STEP_MODE = 1;        // microsteps/step  1 is full-step mode
// 200 steps/rev | 5.18 motor rev/wheel revolution based on observation
const double STEPS_PER_REV = 200 * 5.18 * STEP_MODE;
const double STEPS_PER_INCH = 82.4;
const double STEPS_PER_CM = 32.457;
const unsigned int MOTOR_DELAY = 1000;
const float pi = 3.14159265358979323846264338;
const int TRACK = 10;

extern Side forward_side;

void write_drive(bool fl, bool fr, bool bl, bool br, bool value);
void drive_step(bool fl, bool fr, bool bl, bool br);
void move_servos(bool turned);
void change_forward_side(Side side);
bool ir_reads_black(Direction sensor_direction);

#endif
