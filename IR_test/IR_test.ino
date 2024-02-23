
// COM3 = left USB port on back

// PINS
const int IR_FL_OUT = 0;


void setup() {
    Serial.begin(9600);
}


void loop() {
    Serial.print(digitalRead(IR_FL_OUT));
    Serial.print("\n");
}