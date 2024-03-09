#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <pins_arduino.h>

#include "robot2.h"

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

Servo FR_SERVO;
Servo FL_SERVO;
Servo BR_SERVO;
Servo BL_SERVO;

const int BACK_IR_PIN = A1;
const int LEFT_IR_PIN = A2;
const int RIGHT_IR_PIN = A3;
const int FRONT_IR_PIN = A4;

const int FR_DRIVE_STEP = 46;
const int FR_DRIVE_DIR  = 47;
const int FL_DRIVE_STEP = 48;
const int FL_DRIVE_DIR  = 49;
const int BR_DRIVE_STEP = 50;
const int BR_DRIVE_DIR  = 51;
const int BL_DRIVE_STEP = 52;
const int BL_DRIVE_DIR  = 53;

//Start group pins
const int start_switch = 47;
const int ready_LED = 48;
const int photosensor = 41;

//Display system
// I2C address 0x3F, 16 column and 2 rows
const LiquidCrystal_I2C lcd(0x3F, 16, 2);

//Final pins
const int round_complete_LED = 46;

/******************************
 ********** GLOBALS ***********
*******************************/
int servo_angle = 30;
Side forward_side = FRONT;

/******************************
 ****** SETUP FUNCTIONS *******
*******************************/

void setup_motor(int step_pin, int dir_pin) {
  pinMode(step_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
  digitalWrite(dir_pin, HIGH);  // May not be necessary, but it's in the old code.
}

void setup_all_motors() {
  setup_motor(FR_DRIVE_DIR, FR_DRIVE_STEP);
  setup_motor(FL_DRIVE_DIR, FL_DRIVE_STEP);
  setup_motor(BR_DRIVE_DIR, BR_DRIVE_STEP);
  setup_motor(BL_DRIVE_DIR, BL_DRIVE_STEP);
}

void setup_servos() {
  FR_SERVO.attach(FR_SERVO_PIN, 500, 2500);     //MG996R - 500 to 2500
  FL_SERVO.attach(FL_SERVO_PIN, 500, 2500);     //MG996R - 500 to 2500
  BR_SERVO.attach(BR_SERVO_PIN, 500, 2500);     //MG996R - 500 to 2500
  BL_SERVO.attach(BL_SERVO_PIN, 500, 2500);     //MG996R - 500 to 2500
}

void setup_display() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void setup_IR_sensors() {
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
}


/*******************************
* LOW-LEVEL HARDWARE FUNCTIONS *
*******************************/

// Write HIGH or LOW to specific stepper motors.
void write_drive(bool fl, bool fr, bool bl, bool br, bool value) {
  if (fl) { digitalWrite(FL_DRIVE_STEP, value); }
  if (bl) { digitalWrite(BL_DRIVE_STEP, value); }
  if (fr) { digitalWrite(FR_DRIVE_STEP, value); }
  if (br) { digitalWrite(BR_DRIVE_STEP, value); }
}

// Move one or more of the drive (stepper) motors one step.
void drive_step(bool fl, bool fr, bool bl, bool br) {
  write_drive(fl, fr, bl, br, HIGH);
  delayMicroseconds(MOTOR_DELAY);
  write_drive(fl, fr, bl, br, LOW);
  delayMicroseconds(MOTOR_DELAY); 
  update_servos();
}

// Set all four servos to a specified angle.
// 
// degrees = 30 sets the servos to the "normal" forward position.
// degrees = 150 sets the servos to the fully rotated position (to drive "sideways")
void update_servos() {
  FL_SERVO.write(servo_angle);
  BR_SERVO.write(servo_angle);
  FR_SERVO.write(180 - servo_angle);
  BL_SERVO.write(180 - servo_angle);
}

// Move steering servos to the normal or turned position.
//
// NOTE: This chnages the stepper directions as a side effect.
// Be sure to set them to the desired direction after calling this function.
void move_servos(bool turned) {
  if (turned) {
    digitalWrite(FL_DRIVE_DIR, LOW);
    digitalWrite(FR_DRIVE_DIR, HIGH);
    digitalWrite(BL_DRIVE_DIR, HIGH);
    digitalWrite(BR_DRIVE_DIR, LOW);
  } else {
    digitalWrite(FL_DRIVE_DIR, HIGH);
    digitalWrite(FR_DRIVE_DIR, LOW);
    digitalWrite(BL_DRIVE_DIR, LOW);
    digitalWrite(BR_DRIVE_DIR, HIGH);
  }

  // Assume the servos and the wheels have approximately the same radius.
  int nsteps = (int)(0.25 * STEPS_PER_REV);
  double desired_angle = turned ? 150.0 : 30.0;
  double angle_inc = (desired_angle - servo_angle) / (double)nsteps;

  // Since servo_angle is an int, we need a double to keep track of angle increments, which may be fractional.
  double current_angle = servo_angle;   

  for (int i = 0; i < nsteps; i++) {
    current_angle += angle_inc;
    servo_angle = current_angle;
    drive_step(true, true, true, true);   // Moves steppers and servos.
  }
  servo_angle = desired_angle;  // Make sure we have the right angle.
}

// Prepare the vehicle to drive toward the given side.
// Moves the servos and updates the stepper directions.
void change_forward_side(Side side) {
  forward_side = side;
  // Move the steering servos.
  if (side == FRONT || side == BACK) {
    turn_servos(30);
    //move_servos(false);
  }
  else {
    turn_servos(150);
    //move_servos(true);
  }
  // Update stepper directions.
  switch (side) {
  case FRONT:
    digitalWrite(FL_DRIVE_DIR, HIGH);
    digitalWrite(FR_DRIVE_DIR, HIGH);
    digitalWrite(BL_DRIVE_DIR, HIGH);
    digitalWrite(BR_DRIVE_DIR, HIGH);
    break;
  case BACK:
    digitalWrite(FL_DRIVE_DIR, LOW);
    digitalWrite(FR_DRIVE_DIR, LOW);
    digitalWrite(BL_DRIVE_DIR, LOW);
    digitalWrite(BR_DRIVE_DIR, LOW);
    break;
  case LEFT:
    digitalWrite(FL_DRIVE_DIR, LOW);
    digitalWrite(FR_DRIVE_DIR, HIGH);
    digitalWrite(BL_DRIVE_DIR, HIGH);
    digitalWrite(BR_DRIVE_DIR, LOW);
    break;
  case RIGHT:
    digitalWrite(FL_DRIVE_DIR, HIGH);
    digitalWrite(FR_DRIVE_DIR, LOW);
    digitalWrite(BL_DRIVE_DIR, LOW);
    digitalWrite(BR_DRIVE_DIR, HIGH);
    break;
  }
}

bool ir_reads_black(Direction sensor_direction) {
  int side = (forward_side + sensor_direction) % 360;
  switch (side) {
    case 0:
      return analogRead(FRONT_IR_PIN) == 1;
    case 90:
      return analogRead(LEFT_IR_PIN) == 1;
    case 180:
      return analogRead(BACK_IR_PIN) == 1;
    case 270:
      return analogRead(RIGHT_IR_PIN) == 1;
  }
}


// Turn both servos 90 Degrees
void turn_servos(int degree) {
  if (servo_angle <= degree) {
    float rotation_fraction = (float) (degree - servo_angle)/360;                    //Fraction of a full circle to rotate

    bool old_DIR_FL = digitalRead(FL_DRIVE_DIR);
    digitalWrite(FL_DRIVE_DIR, LOW);
    bool old_DIR_BR = digitalRead(BR_DRIVE_DIR);
    digitalWrite(BR_DRIVE_DIR, HIGH);
    bool old_DIR_FR = digitalRead(FR_DRIVE_DIR);
    digitalWrite(FR_DRIVE_DIR, LOW);
    bool old_DIR_BL = digitalRead(BL_DRIVE_DIR);
    digitalWrite(BL_DRIVE_DIR, HIGH);
    

    unsigned long turn_steps = (unsigned long) STEP_MODE * 200 * 5.18 * rotation_fraction;             //Fraction of rotation times steps per rotation
    float angle_increment = (float) (360 * rotation_fraction)/turn_steps;
  
    digitalWrite(FR_DRIVE_STEP, HIGH);
    digitalWrite(BL_DRIVE_STEP, HIGH);

    digitalWrite(FL_DRIVE_STEP, HIGH);
    digitalWrite(BR_DRIVE_STEP, HIGH);

    for (unsigned long x = 0; x < turn_steps; x++) {
      digitalWrite(FR_DRIVE_STEP, HIGH);
      digitalWrite(BL_DRIVE_STEP, HIGH);
      delayMicroseconds(MOTOR_DELAY);                            //Was a 500 ms delay
      digitalWrite(FR_DRIVE_STEP, LOW);
      digitalWrite(BL_DRIVE_STEP, LOW);
      delayMicroseconds(MOTOR_DELAY); 
      BL_SERVO.write(180 - int(servo_angle + x * angle_increment));
      FR_SERVO.write(180 - int(servo_angle + x * angle_increment));
    }

    for (unsigned long x = 0; x < turn_steps; x++) {
      digitalWrite(FL_DRIVE_STEP, HIGH);
      digitalWrite(BR_DRIVE_STEP, HIGH);
      delayMicroseconds(MOTOR_DELAY);                            //Was a 500 ms delay
      digitalWrite(FL_DRIVE_STEP, LOW);
      digitalWrite(BR_DRIVE_STEP, LOW);
      delayMicroseconds(MOTOR_DELAY); 
      BR_SERVO.write(int(servo_angle + x * angle_increment));
      FL_SERVO.write(int(servo_angle + x * angle_increment));
    }
  
    digitalWrite(FL_DRIVE_DIR, old_DIR_FL);
    digitalWrite(BR_DRIVE_DIR, old_DIR_BR);
    digitalWrite(FR_DRIVE_DIR, old_DIR_FR);
    digitalWrite(BL_DRIVE_DIR, old_DIR_BL);
  }
  else {
    float rotation_fraction = (float) (servo_angle - degree)/360;                    //Fraction of a full circle to rotate

    bool old_DIR_FL = digitalRead(FL_DRIVE_DIR);
    digitalWrite(FL_DRIVE_DIR, HIGH);
    bool old_DIR_BR = digitalRead(BR_DRIVE_DIR);
    digitalWrite(BR_DRIVE_DIR, LOW);
    bool old_DIR_FR = digitalRead(FR_DRIVE_DIR);
    digitalWrite(FR_DRIVE_DIR, HIGH);
    bool old_DIR_BL = digitalRead(BL_DRIVE_DIR);
    digitalWrite(BL_DRIVE_DIR, LOW);
    
    unsigned long turn_steps = (unsigned long) STEP_MODE * 200 * 5.18 * rotation_fraction;            //Fraction of rotation times steps per rotation
    float angle_increment = (float) (360 * rotation_fraction)/turn_steps;
  
    digitalWrite(FR_DRIVE_STEP, HIGH);
    digitalWrite(BL_DRIVE_STEP, HIGH);

    for (unsigned long x = 0; x < turn_steps; x++) {
      digitalWrite(FL_DRIVE_STEP, HIGH);
      digitalWrite(BR_DRIVE_STEP, HIGH);
      delayMicroseconds(MOTOR_DELAY);                            //Was a 500 ms delay
      digitalWrite(FL_DRIVE_STEP, LOW);
      digitalWrite(BR_DRIVE_STEP, LOW);
      delayMicroseconds(MOTOR_DELAY); 
      FL_SERVO.write(180 - servo_angle + int((turn_steps - x) * angle_increment));
      BR_SERVO.write(180 - servo_angle + int((turn_steps - x) * angle_increment));
    }
  
    digitalWrite(FL_DRIVE_STEP, HIGH);
    digitalWrite(BR_DRIVE_STEP, HIGH);

    for (unsigned long x = 0; x < turn_steps; x++) {
      digitalWrite(FR_DRIVE_STEP, HIGH);
      digitalWrite(BL_DRIVE_STEP, HIGH);
      delayMicroseconds(MOTOR_DELAY);                            //was a 500 ms delay
      digitalWrite(FR_DRIVE_STEP, LOW);
      digitalWrite(BL_DRIVE_STEP, LOW);
      delayMicroseconds(MOTOR_DELAY); 
      FR_SERVO.write(180 - servo_angle + x * angle_increment);
      BL_SERVO.write(180 - servo_angle + x * angle_increment);
    }
    digitalWrite(FL_DRIVE_DIR, old_DIR_FL);
    digitalWrite(BR_DRIVE_DIR, old_DIR_BR);
    digitalWrite(FR_DRIVE_DIR, old_DIR_FR);
    digitalWrite(BL_DRIVE_DIR, old_DIR_BL);
  }
  
  FL_SERVO.write(degree);
  BR_SERVO.write(degree);
  FR_SERVO.write(180 - degree);
  BL_SERVO.write(180 - degree);
  servo_angle = degree;
}

/*******************************
 ****** TEST FUNCTIONS *********
*******************************/
void test_servos() {
  move_servos(false);
  delay(800);
  move_servos(true);
  delay(800);
  move_servos(false);
}

void test_steppers() {
  change_forward_side(FRONT);
  for (int i = 0; i < 1000; i++) {
    drive_step(true, true, true, true);
  }
}

void test_turns() {
  delay(2000);
  change_forward_side(FRONT);
  delay(2000);
  for (int i = 0; i < 1000; i++) {
    drive_step(true, true, true, true);
  }
  delay(2000);
  change_forward_side(LEFT);
  delay(2000);
  for (int i = 0; i < 1000; i++) {
    drive_step(true, true, true, true);
  }
  delay(2000);
  change_forward_side(BACK);
  delay(2000);
  for (int i = 0; i < 1000; i++) {
    drive_step(true, true, true, true);
  }
  delay(2000);
  change_forward_side(RIGHT);
  delay(2000);
  for (int i = 0; i < 1000; i++) {
    drive_step(true, true, true, true);
  }
  delay(2000);
  change_forward_side(FRONT);
}

void test_plow() {
  // Move the plow up and down.
}

/*******************************
 ******** ENTRY POINTS *********
*******************************/

void setup() {
  delay(4000);          // Give some time to step away.
  setup_all_motors();
  setup_servos();
  setup_display();
}

void loop() {
  test_turns();
  while (true) {};      // Wait forever.
}

