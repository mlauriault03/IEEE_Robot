#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#include "robot.h"

// motor FR (Front-Right) (23, 24)
const int STEP_FR = 23;     // PUL+
const int DIR_FR = 24;      // DIR+
// motors FL (Front-Left) (25, 26)
const int STEP_FL = 25;     // PUL+
const int DIR_FL = 26;      // DIR+
// motors FR and BL (Front-Right and Back-Left) (27, 28)
const int STEP_BR = 27;     // PUL+
const int DIR_BR = 28;      // DIR+
// motors FR and BL (Front-Right and Back-Left) (29, 30)
const int STEP_BL = 29;     // PUL+
const int DIR_BL = 30;      // DIR+

//Steering servos
const int FL_BR_servo_pin = 2;     
const int FR_BL_servo_pin = 3; 

Servo FL_BR_Servo;
Servo FR_BL_Servo;

//Sweeping servos
const int left_sweep_servo_pin = 4;
const int right_sweep_servo_pin = 5;
const int center_sweep_servo_pin = 6;

Servo left_sweep_servo;
Servo right_sweep_servo;
Servo center_sweep_servo;

//Crater crossing servos
const int bridge_release_servo_pin = 7;
const int back_bar_servo_pin = 8;
const int front_hook_servo_pin = 9;
const int back_hook_servo_pin = 10;

Servo bridge_release_servo;
Servo back_bar_servo;
Servo front_hook_servo;
Servo back_hook_servo;

//Start group pins
const int start_switch = 47;
const int ready_LED = 48;
const int photosensor = 41;

//Display system
//const int disp_SDA = 20;                        Just reserved. They are found by the library, so don't declare
//const int disp_SCL = 21;
LiquidCrystal_I2C lcd(0x3F, 16, 2);               // I2C address 0x3F, 16 column and 2 rows

//Final pins
const int round_complete_LED = 46;


// CONSTANTS
int step_mode = 1;                                //microsteps/step  1 is full-step mode
unsigned int motor_delay = 500;
float pi = 3.14159265358979323846264338;
int wheel_radius = 2;
int servo_angle = 30;
float distance_travelled;


// SETUP FUNCTIONS

void setup_motors() {
    // motor FR
    pinMode(STEP_FR, OUTPUT);             
    pinMode(DIR_FR, OUTPUT);
    digitalWrite(DIR_FR, HIGH);           // LOW = counter-clockwise
    // motor FL
    pinMode(STEP_FL, OUTPUT);
    pinMode(DIR_FL, OUTPUT);
    digitalWrite(DIR_FL, HIGH);           // HIGH = clockwise - backward
    // motor BR
    pinMode(STEP_BR, OUTPUT);
    pinMode(DIR_BR, OUTPUT);
    digitalWrite(DIR_BR, HIGH);    
    // motors BL
    pinMode(STEP_BL, OUTPUT);
    pinMode(DIR_BL, OUTPUT);
    digitalWrite(DIR_BL, HIGH);    
}

void setup_servos() {
  FL_BR_Servo.attach(FL_BR_servo_pin, 500, 2500);                     //MG996R - 500 to 2500
  FR_BL_Servo.attach(FR_BL_servo_pin, 500, 2500);
  FL_BR_Servo.write(servo_angle);
  FR_BL_Servo.write(180 - servo_angle);                                  //Parallax - 1300 to 1700
  left_sweep_servo.attach(left_sweep_servo_pin, 500, 2500);                  //SG90 - 1000 - 2000
  right_sweep_servo.attach(right_sweep_servo_pin, 1000, 2000);               //Make this one the other kind. Check the pulse width.
  center_sweep_servo.attach(center_sweep_servo_pin, 500, 2500);
  center_sweep_servo.write(150);
  bridge_release_servo.attach(bridge_release_servo_pin, 1300, 1700);
  bridge_release_servo.write(180);
  back_bar_servo.attach(back_bar_servo_pin, 1300, 1700);
  front_hook_servo.attach(front_hook_servo_pin, 1000, 2000);                             
  back_hook_servo.attach(back_hook_servo_pin, 1000, 2000);
}

void setup_display() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void setup() {
  setup_motors();
  setup_servos();
  setup_display();
  //Serial.begin(9600);
}

// PROCEDURES

// Move forward <steps> number of steps
void move_forward(unsigned long steps) {
    for (unsigned long x = 0; x < steps; x++) {
      digitalWrite(STEP_FL, HIGH);
      digitalWrite(STEP_BR, HIGH);
      digitalWrite(STEP_FR, HIGH);
      digitalWrite(STEP_BL, HIGH);
      delayMicroseconds(motor_delay);                            //Was a 500 microsecond delay
      digitalWrite(STEP_FL, LOW);
      digitalWrite(STEP_BR, LOW);
      digitalWrite(STEP_FR, LOW);
      digitalWrite(STEP_BL, LOW);
      delayMicroseconds(motor_delay); 
      FL_BR_Servo.write(servo_angle);
      FR_BL_Servo.write(180 - servo_angle);
      distance_travelled = distance_travelled + steps_to_centimeters(1);
      sweep_servos();
  }
}

// Swap the direction of the FR and BL motors for when servos turn
void swap_motor_dir() {
  if (digitalRead(DIR_FL) == HIGH) {
    digitalWrite(DIR_FL, LOW);
    digitalWrite(DIR_BR, LOW);
  }
  else {
    digitalWrite(DIR_FL, HIGH);
    digitalWrite(DIR_BR, HIGH);
  }
}

// Change driving direction
void reverse_motor_dir() {
  // Change direction of front left and back right motors
  if (digitalRead(DIR_FL) == HIGH) {
    digitalWrite(DIR_FL, LOW);
    digitalWrite(DIR_BR, LOW);
  }
  else {
    digitalWrite(DIR_FL, HIGH);
    digitalWrite(DIR_BR, HIGH);
  }
  // Change direction of front right and back left motors
  if (digitalRead(DIR_FR) == HIGH) {
    digitalWrite(DIR_FR, LOW);
    digitalWrite(DIR_BL, LOW);
  }
  else {
    digitalWrite(DIR_FR, HIGH);
    digitalWrite(DIR_BL, HIGH);
  }
}

// Turn both servos 90 Degrees
void turn_servos(int degree) {
  if (servo_angle <= degree) {
    float rotation_fraction = (float) (degree - servo_angle)/360;                    //Fraction of a full circle to rotate

    bool old_DIR_FL = digitalRead(DIR_FL);
    digitalWrite(DIR_FL, LOW);
    bool old_DIR_BR = digitalRead(DIR_BR);
    digitalWrite(DIR_BR, HIGH);
    bool old_DIR_FR = digitalRead(DIR_FR);
    digitalWrite(DIR_FR, LOW);
    bool old_DIR_BL = digitalRead(DIR_BL);
    digitalWrite(DIR_BL, HIGH);
    

    unsigned long turn_steps = (unsigned long) step_mode * 200 * 5.18 * rotation_fraction;             //Fraction of rotation times steps per rotation
    float angle_increment = (float) (360 * rotation_fraction)/turn_steps;
  
    digitalWrite(STEP_FR, HIGH);
    digitalWrite(STEP_BL, HIGH);

    digitalWrite(STEP_FL, HIGH);
    digitalWrite(STEP_BR, HIGH);

    for (unsigned long x = 0; x < turn_steps; x++) {
      digitalWrite(STEP_FR, HIGH);
      digitalWrite(STEP_BL, HIGH);
      delayMicroseconds(motor_delay);                            //Was a 500 ms delay
      digitalWrite(STEP_FR, LOW);
      digitalWrite(STEP_BL, LOW);
      delayMicroseconds(motor_delay); 
      FR_BL_Servo.write(180 - int(servo_angle + x * angle_increment));
    }

    for (unsigned long x = 0; x < turn_steps; x++) {
      digitalWrite(STEP_FL, HIGH);
      digitalWrite(STEP_BR, HIGH);
      delayMicroseconds(motor_delay);                            //Was a 500 ms delay
      digitalWrite(STEP_FL, LOW);
      digitalWrite(STEP_BR, LOW);
      delayMicroseconds(motor_delay); 
      FL_BR_Servo.write(int(servo_angle + x * angle_increment));
    }
  
    digitalWrite(DIR_FL, old_DIR_FL);
    digitalWrite(DIR_BR, old_DIR_BR);
    digitalWrite(DIR_FR, old_DIR_FR);
    digitalWrite(DIR_BL, old_DIR_BL);
  }
  else {
    float rotation_fraction = (float) (servo_angle - degree)/360;                    //Fraction of a full circle to rotate

    bool old_DIR_FL = digitalRead(DIR_FL);
    digitalWrite(DIR_FL, HIGH);
    bool old_DIR_BR = digitalRead(DIR_BR);
    digitalWrite(DIR_BR, LOW);
    bool old_DIR_FR = digitalRead(DIR_FR);
    digitalWrite(DIR_FR, HIGH);
    bool old_DIR_BL = digitalRead(DIR_BL);
    digitalWrite(DIR_BL, LOW);
    
    unsigned long turn_steps = (unsigned long) step_mode * 200 * 5.18 * rotation_fraction;            //Fraction of rotation times steps per rotation
    float angle_increment = (float) (360 * rotation_fraction)/turn_steps;
  
    digitalWrite(STEP_FR, HIGH);
    digitalWrite(STEP_BL, HIGH);

    for (unsigned long x = 0; x < turn_steps; x++) {
      digitalWrite(STEP_FL, HIGH);
      digitalWrite(STEP_BR, HIGH);
      delayMicroseconds(motor_delay);                            //Was a 500 ms delay
      digitalWrite(STEP_FL, LOW);
      digitalWrite(STEP_BR, LOW);
      delayMicroseconds(motor_delay); 
      FL_BR_Servo.write(180 - servo_angle + int((turn_steps - x) * angle_increment));
    }
  
    digitalWrite(STEP_FL, HIGH);
    digitalWrite(STEP_BR, HIGH);

    for (unsigned long x = 0; x < turn_steps; x++) {
      digitalWrite(STEP_FR, HIGH);
      digitalWrite(STEP_BL, HIGH);
      delayMicroseconds(motor_delay);                            //Was a 500 ms delay
      digitalWrite(STEP_FR, LOW);
      digitalWrite(STEP_BL, LOW);
      delayMicroseconds(motor_delay); 
      FR_BL_Servo.write(180 - servo_angle + x * angle_increment);
    }
    digitalWrite(DIR_FL, old_DIR_FL);
    digitalWrite(DIR_BR, old_DIR_BR);
    digitalWrite(DIR_FR, old_DIR_FR);
    digitalWrite(DIR_BL, old_DIR_BL);
  }
  
  FL_BR_Servo.write(degree);
  FR_BL_Servo.write(180 - degree);
  servo_angle = degree;
}

unsigned long centimeters_to_steps(float centimeters_to_go) {
  float inches_to_go = centimeters_to_go / 2.54;
  float revolutions = inches_to_go / (2*pi*wheel_radius);
  unsigned long steps = (unsigned long) step_mode * 200 * revolutions * 5.18;        //200 steps/rev | 5.18 motor revolutions/wheel revolution based on observation
  return steps;
}

float steps_to_centimeters(unsigned long steps_to_go) {
  float revolutions = (float) steps_to_go / (200 * 5.18 * step_mode);
  float inches_to_go = revolutions * 2*pi*wheel_radius;
  float centimeters = inches_to_go * 2.54;
  return centimeters;
}

void adjust_speed(float speed_desired) {
  //input desired speed in cm/s to adjust robot speed
  unsigned long speed_steps_per_sec = centimeters_to_steps(speed_desired);
  unsigned long microsec_per_step = 1000000/speed_steps_per_sec;
  motor_delay = (unsigned int) 1.5 * microsec_per_step ;                              //Each step takes about 1.5 times delay
}

void sweep_servos() {
  float distance_to_hill;
  distance_to_hill = 22.32;
  if (distance_travelled < distance_to_hill) {
    center_sweep_servo.write(40);
    left_sweep_servo.write(10);
    right_sweep_servo.write(170);
  }
  else if (distance_travelled < (distance_to_hill + 24.77)) {
    center_sweep_servo.write(60);
    left_sweep_servo.write(10);
    right_sweep_servo.write(170);
  }
  else if (distance_travelled < (distance_to_hill + 67)) {
    center_sweep_servo.write(40);
    left_sweep_servo.write(10);
    right_sweep_servo.write(170);
  }
  else if (distance_travelled < (distance_to_hill + 91.77)) {
    center_sweep_servo.write(20);
    left_sweep_servo.write(10);
    right_sweep_servo.write(170);
  }
  else if (distance_travelled < (distance_to_hill + 161.14)) {
    center_sweep_servo.write(40);
    left_sweep_servo.write(10);
    right_sweep_servo.write(170);
  }
  else {
    center_sweep_servo.write(150);                                     //Retract the sweep arm
    left_sweep_servo.write(130);
    right_sweep_servo.write(50);
  }
}

void start_sequence() {
  pinMode(start_switch, INPUT);
  pinMode(ready_LED, OUTPUT);
  pinMode(photosensor, INPUT);
  bool start_switch_output = digitalRead(start_switch);
  while (start_switch_output != HIGH)   {                                              //Switch to tell the robot when to start looking is active high
    start_switch_output = digitalRead(start_switch);
    delay(5);
  }
  digitalWrite(ready_LED, HIGH);                                                       //Light to tell when it is looking turns on
  bool photosensor_state_1 = digitalRead(photosensor);
  Serial.println(photosensor_state_1);
  delay(20);
  bool photosensor_state_2 = digitalRead(photosensor);
  delay(20);
  bool photosensor_state_3 = digitalRead(photosensor);
  delay(20);                          
  while ((photosensor_state_1  != LOW) | (photosensor_state_2 != LOW) | (photosensor_state_3 != LOW)) {        //Look at photosensor to see when the start LED turns on. Photosensor gives 0 when it sees light
    photosensor_state_1 = digitalRead(photosensor);
    delay(20);
    photosensor_state_2 = digitalRead(photosensor);
    delay(20);
    photosensor_state_3 = digitalRead(photosensor);
    delay(20);
  }
  digitalWrite(ready_LED, LOW);                                                       //turn off the LED since you are now going
}

void crater_cross() {
  int front_delay = 3000;
  int back_delay = 3000;

  //adjust_speed(5);
  center_sweep_servo.write(70);                                                      //Set center sweep down far enough to get out of the way
  bridge_release_servo.write(150);                                                       //turn counter-clockwise at about 1/4 speed or about 27 RPM
  delay(front_delay);
  bridge_release_servo.write(1500);                                                       //set it back to being still
  front_hook_servo.write(90);                                                        //NOT SURE WHAT THIS WILL NEED TO BE
  move_forward(centimeters_to_steps(30.48));
  back_bar_servo.write(1450);                                                        //Probably will need to tune these delays
  delay(back_delay);
  back_bar_servo.write(150);                                               
  back_hook_servo.write(90);                                                         //NOT SURE WHAT THIS WILL NEED TO BE
  move_forward(centimeters_to_steps(22.86));                                         
  front_hook_servo.write(0);                                                         //Retract the crater crossing servos
  bridge_release_servo.write(145);                          
  delay(front_delay);
  bridge_release_servo.write(150);
  back_hook_servo.write(0);
  back_bar_servo.write(150);
  delay(back_delay);
  back_hook_servo.write(150);
  //adjust_speed(30);
}

void non_static_display() {                      
    lcd.setCursor(0, 0);                  // moves cursor to top left
    lcd.print("         GO EMV ");        // prints "GoEMV"
    lcd.setCursor(2, 1);                  // moves cursor to bottom row and slightly to the right
    lcd.print("       ENG 496");
    delay(5000);                          //do non-static team promotion for 5 seconds
    lcd.clear();
}

void stop_motion() {
  bool keep_going = LOW;
  while (keep_going != HIGH) {
    //Do Nothing. The robot is to have stopped moving
  }
}

void rotate_robot(bool direction) {                                           //HIGH: robot goes clockwise, LOW: robot goes counter-clockwise
  bool old_DIR_FL = digitalRead(DIR_FL);
  bool old_DIR_BR = digitalRead(DIR_BR);
  bool old_DIR_FR = digitalRead(DIR_FR);
  bool old_DIR_BL = digitalRead(DIR_BL);
  
  if (direction == HIGH) {
    digitalWrite(DIR_FL, LOW);
    digitalWrite(DIR_BR, HIGH);
    digitalWrite(DIR_FR, HIGH);
    digitalWrite(DIR_BL, LOW);
  }

  else {
    digitalWrite(DIR_FL, HIGH);
    digitalWrite(DIR_BR, LOW);
    digitalWrite(DIR_FR, LOW);
    digitalWrite(DIR_BL, HIGH);
  }

  for (unsigned long x = 0; x < centimeters_to_steps(27.93*1.13); x++) {
      digitalWrite(STEP_FL, HIGH);
      digitalWrite(STEP_BR, HIGH);
      digitalWrite(STEP_FR, HIGH);
      digitalWrite(STEP_BL, HIGH);
      delayMicroseconds(motor_delay);                            //Was a 500 ms delay
      digitalWrite(STEP_FL, LOW);
      digitalWrite(STEP_BR, LOW);
      digitalWrite(STEP_FR, LOW);
      digitalWrite(STEP_BL, LOW);
      delayMicroseconds(motor_delay); 
      FL_BR_Servo.write(servo_angle);
      FR_BL_Servo.write(180 - servo_angle);
  }

  digitalWrite(DIR_FL, old_DIR_FL);
  digitalWrite(DIR_BR, old_DIR_BR);
  digitalWrite(DIR_FR, old_DIR_FR);
  digitalWrite(DIR_BL, old_DIR_BL);
}

// LOOP

void loop() {
  start_sequence();
  delay(5);                                                    //Give some time to step away
  adjust_speed(30);
  delay(5);
  move_forward(centimeters_to_steps(194.6275));
  delay(5);
  turn_servos(180-servo_angle);                             //Change to a perpendicular arrangement to turn left. 30 is straight | 150 is turned 90 degrees
  delay(500);                             
  swap_motor_dir();                                           //turned to opposite orientation, so need to change
  delay(5);
  move_forward(centimeters_to_steps(60.96));
  delay(5);
  turn_servos(180-servo_angle);                             //Change to a perpendicular arrangement to turn left. 30 is straight | 150 is turned 90 degrees
  delay(5);
  turn_servos(servo_angle);
  delay(500);          
  swap_motor_dir();                                           //Get the motors to all have the same direction
  delay(5);
  reverse_motor_dir();                                        //Reverse the direction of all the motors since the robot is now facing backward
  delay(5);
  move_forward(centimeters_to_steps(81.25));
  delay(5);
  turn_servos(90);
  delay(5);
  turn_servos(90);
  delay(5);
  rotate_robot(LOW);
  delay(500);
  turn_servos(150);
  delay(500);
  swap_motor_dir();
  delay(5);
  reverse_motor_dir();
  delay(5);
  move_forward(centimeters_to_steps(8));
  delay(5);
  bridge_release_servo.write(100);
  delay(500);
  reverse_motor_dir();
  adjust_speed(10);
  delay(5);
  move_forward(centimeters_to_steps(10));
  delay(500);
  reverse_motor_dir();
  delay(5);
  move_forward(centimeters_to_steps(12));
  delay(500);
  reverse_motor_dir();
  adjust_speed(30);
  delay(5);
  move_forward(centimeters_to_steps(10));
  delay(5);
  turn_servos(90);
  delay(500);
  turn_servos(90);
  delay(500);
  rotate_robot(HIGH);
  delay(500);
  turn_servos(30);
  delay(500);
  swap_motor_dir();
  delay(5);
  reverse_motor_dir();
  delay(5);
  move_forward(centimeters_to_steps(70));
  delay(5);
  turn_servos(180-servo_angle);
  delay(500);
  swap_motor_dir();
  delay(5);
  move_forward(centimeters_to_steps(6));
  delay(5);
  turn_servos(180-servo_angle);
  delay(500);
  swap_motor_dir();
  delay(5);
  reverse_motor_dir();
  delay(5);
  move_forward(centimeters_to_steps(70 + 53.34 + 48.26+15));
  delay(5);
  non_static_display();
  delay(5);
  turn_servos(180-servo_angle);
  delay(500);
  swap_motor_dir();
  delay(5);
  reverse_motor_dir();
  delay(5);
  move_forward(centimeters_to_steps(10));
  delay(5);
  reverse_motor_dir();
  delay(5);
  move_forward(centimeters_to_steps(2));
  delay(5);
  stop_motion();
}

/**********Vehicle Interface Class Implementation**********/

void VehicleInterface::set_steer_curvature(double radians) {
  steer_curvature = radians;
}

void VehicleInterface::move_increment() {
  // TODO
}

void VehicleInterface::ir_reads_black(IRSensor sensor) {
  
}
