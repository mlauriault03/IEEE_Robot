// Hardware setup functions.
#include <Arduino.h>      // Inlcude to make VSCode happy.
#include "hardware.h"

Servo FR_SERVO;
Servo FL_SERVO;
Servo BR_SERVO;
Servo BL_SERVO;

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



