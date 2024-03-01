void setup() {
  // put your setup code here, to run once:
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);

  Serial.begin(9600);
}

void loop() {
  int Back_sensor;
  int Left_sensor;
  int Right_sensor;
  int Front_sensor;

  Back_sensor = analogRead(A1);
  Left_sensor = analogRead(A2);
  Right_sensor = analogRead(A3);
  Front_sensor = analogRead(A4);
// Sean Was Here
  Serial.print("Back Sensor: ");
  Serial.println(Back_sensor);
  Serial.print("Left Sensor: ");
  Serial.println(Left_sensor);
  Serial.print("Right Sensor: ");
  Serial.println(Right_sensor);
  Serial.print("Front Sensor: ");
  Serial.println(Front_sensor);

  delay(2000);
}
