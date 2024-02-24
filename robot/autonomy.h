#ifndef IEEE_ROBOT_2024
#define IEEE_ROBOT_2024

// Enum values represent degrees coutnerclockwise from FRONT.
enum Side {FRONT=0, LEFT=90, BACK=180, RIGHT=270};

enum IRSensor {FRONT_LEFT, FRONT_RIGHT, SIDE_FRONT, SIDE_BACK};

// This class provides a basic interface to the robot's hardware.
class VehicleInterface {
public:
    void set_steer_curvature(double radians);  // Set curvature for differential steering. Left is positive.
    void change_forward_side(Side side); // Rotate wheels to change "forward" direction
    void move_increment();               // Move a tiny step forward.
    bool ir_reads_black(IRSensor sensor);   // Determine whether an infrared sensor reads black.
};

enum RobotState {
    TRACK_NORMAL,               // 0
    TRACK_SEARCH_LINE_LEFT,     // 1
    TRACK_SEARCH_LINE_RIGHT,    // 2
    TRACK_CORRECT_LEFT,         // 3
    TRACK_CORRECT_RIGHT,        // 4
};


// This class is responsible for the robot's logic.
template <class Vehicle>
class Robot {

    Vehicle& vehicle;
    RobotState state = TRACK_NORMAL;
    RobotState prev_state = TRACK_NORMAL;

    void track_normal() {
        // Drive straight forward.
        vehicle.set_steer_curvature(0.0);
        vehicle.move_increment();

        // Read the infrared sensors (Front Left and Front Right).
        bool left = vehicle.ir_reads_black(FRONT_LEFT);
        bool right = vehicle.ir_reads_black(FRONT_RIGHT);

        // See if we need to change states.
        if (left && right) {
            // We can't see the line.
            state = TRACK_SEARCH_LINE_LEFT;
        }
        else if (left) {
            // We're left of the line, so turn right.
            state = TRACK_CORRECT_RIGHT;
        }
        else if (right) {
            // We're right of the line, so turn left.
            state = TRACK_CORRECT_LEFT;
        }
    }

    void track_search_line(bool to_left) {
        float curvature = to_left ? 0.8 : -0.8;
        vehicle.set_steer_curvature(curvature);
        vehicle.move_increment();

        // Read the infrared sensors (Front Left and Front Right).
        bool left = vehicle.ir_reads_black(FRONT_LEFT);
        bool right = vehicle.ir_reads_black(FRONT_RIGHT);
        if (!left || !right) {
            // We've detected part of the line.
            state = TRACK_NORMAL;
        }
    }

    void track_correct_path(int state_timer, bool to_left) {
        float correction = to_left ? 0.1 : -0.1;
        vehicle.set_steer_curvature(correction);
        vehicle.move_increment();

        // Read the infrared sensors (Front Left and Front Right).
        bool left = vehicle.ir_reads_black(FRONT_LEFT);
        bool right = vehicle.ir_reads_black(FRONT_RIGHT);

        // See if we need to change states.
        if (left && right) {
            // We can't see the line.
            state = to_left ? TRACK_SEARCH_LINE_LEFT
                            : TRACK_SEARCH_LINE_RIGHT;
        }
        else if (left) {
            // We're going left of the line, so correct right.
            state = TRACK_CORRECT_RIGHT;
        }
        else if (right) {
            // We're going right of the line, so correct left.
            state = TRACK_CORRECT_LEFT;
        }
        else {
            state = TRACK_NORMAL;
        }
    }


public:
    Robot(Vehicle &v) : vehicle(v) {}

    void run() {
        int state_counter = 0;
        for (int i = 0; i < 2000; state_counter++, i++) {
            if (state != prev_state) {
                // We changed states since the last cycle.
                state_counter = 0;          // Reset the state timer.
                prev_state = state;
            }
            switch (state)
            {
            case TRACK_NORMAL:
                track_normal();
                break;
            
            case TRACK_CORRECT_LEFT:
                track_correct_path(state_counter, true);
                break;

            case TRACK_CORRECT_RIGHT:
                track_correct_path(state_counter, false);
                break;

            case TRACK_SEARCH_LINE_LEFT:
                track_search_line(true);
                break;
            
            case TRACK_SEARCH_LINE_RIGHT:
                track_search_line(false);
                break;

            default:
                break;
            }
        }
    }

};

#endif