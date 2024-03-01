#include <Servo.h>

// motor FR (Front-Right) (23, 24)
const int STEP_FR = 46;     // PUL+
const int DIR_FR = 47;      // DIR+
// motors FL (Front-Left) (25, 26)
const int STEP_FL = 48;     // PUL+
const int DIR_FL = 49;      // DIR+
// motors FR and BL (Front-Right and Back-Left) (27, 28)
const int STEP_BR = 50;     // PUL+
const int DIR_BR = 51;      // DIR+
// motors FR and BL (Front-Right and Back-Left) (29, 30)
const int STEP_BL = 52;     // PUL+
const int DIR_BL = 53;      // DIR+

Servo BL;
Servo BR;
Servo FL;
Servo FR;

int step_mode = 1;                                //microsteps/step  1 is full-step mode
unsigned int motor_delay = 500;
float pi = 3.14159265358979323846264338;
int wheel_radius = 2;
int servo_angle = 30;

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
  BL.attach(3, 500, 2500);
  BR.attach(2, 500, 2500);
  FL.attach(4, 500, 2500);
  FR.attach(5, 500, 2500);
  
}

void setup() {
  setup_motors();
  setup_servos();
}

void move_forward(unsigned long steps) {
    for (unsigned long x = 0; x < steps; x++) {
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
      FL.write(servo_angle);
      BR.write(servo_angle);
      FR.write(180 - servo_angle);
      BL.write(180-servo_angle);
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

unsigned long centimeters_to_steps(float centimeters_to_go) {
  float inches_to_go = centimeters_to_go / 2.54;
  float revolutions = inches_to_go / (2*pi*wheel_radius);
  unsigned long steps = (unsigned long) step_mode * 200 * revolutions * 5.18;        //200 steps/rev | 5.18 motor revolutions/wheel revolution based on observation
  return steps;
}

void adjust_speed(float speed_desired) {
  //input desired speed in cm/s to adjust robot speed
  unsigned long speed_steps_per_sec = centimeters_to_steps(speed_desired);
  unsigned long microsec_per_step = 1000000/speed_steps_per_sec;
  motor_delay = (unsigned int) 1.5 * microsec_per_step ;                              //Each step takes about 1.5 times delay
}

void loop() {
  delay(4000);                                              //Give some time to step away
  adjust_speed(30);
  move_forward(centimeters_to_steps(194.6275));
  

}
