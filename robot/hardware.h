// Hardware setup and pin declarations.
#ifndef IEEE_ROBOT_HARDWARE_57839273
#define IEEE_ROBOT_HARDWARE_57839273

#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <pins_arduino.h>

/********************************
 ** PIN/COMPONENT DECLARATIONS **
 ********************************/

/* Abbreviations:
 *  F = Front, B = Back, R = Right, L = Left
 *  IR = Infrared Sensor
 */

// Steering servos
const int BR_SERVO_PIN = 2;
const int BL_SERVO_PIN = 3;
const int FL_SERVO_PIN = 4;
const int FR_SERVO_PIN = 5;

const Servo FR_SERVO;
const Servo FL_SERVO;
const Servo BR_SERVO;
const Servo BL_SERVO;

const int BACK_IR_PIN = A1;
const int LEFT_IR_PIN = A2;
const int RIGHT_IR_PIN = A3;
const int FRONT_IR_PIN = A4;

const int BL_DRIVE_DIR = 53;
const int FL_DRIVE_DIR = 49;
const int BR_DRIVE_DIR = 51;
const int FR_DRIVE_DIR = 47;
const int FL_DRIVE_STEP = 48;
const int BL_DRIVE_STEP = 52;
const int FR_DRIVE_STEP = 46;
const int BR_DRIVE_STEP = 50;

//Start group pins
const int start_switch = 47;
const int ready_LED = 48;
const int photosensor = 41;

//Display system
const LiquidCrystal_I2C lcd(0x3F, 16, 2);               // I2C address 0x3F, 16 column and 2 rows

//Final pins
const int round_complete_LED = 46;

/******************************
 ****** OTHER CONSTANTS *******
*******************************/
const int STEP_MODE = 1;                                //microsteps/step  1 is full-step mode
const unsigned int MOTOR_DELAY = 500;
const float pi = 3.14159265358979323846264338;
const int TRACK = 10;
/******************************
 ****** SETUP FUNCTIONS *******
*******************************/
void setup_all_motors();
void setup_servos();
void setup_display();

#endif