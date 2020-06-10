/*
   P07_00_00-TestSensor
   Project: P07_00_00 Raindrop Bob
   Schematic: P07_00_00-TestSensor.fzz

   Description: Test diy soil moisture sensor based on design from
   https://www.instructables.com/id/DIY-SOIL-MOISTURE-SENSOR-CHEAP-YET-ACCURATE-/
*/

//Variables
#define PwrSensor01 3  //Pin to turn on moisture sensor 01
#define Sensor01 A0  //Pin to read moisture sensor 01

void setup() {
  Serial.begin(9600);

  //Setup pins
  pinMode(PwrSensor01, OUTPUT);
  pinMode(Sensor01, INPUT);
  digitalWrite(PwrSensor01, LOW);
}

void loop() {
  //Turn sensor on
  digitalWrite(PwrSensor01, HIGH);
  delay(200);

  //Read sensor
  Serial.println(analogRead(Sensor01));

  //Turn off sensor
  digitalWrite(PwrSensor01, LOW);
  delay(1000);
}
