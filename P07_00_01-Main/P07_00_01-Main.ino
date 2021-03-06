/*
   P07_00_00-Main
   Project: P07_00_00 Raindrop Bob
*/
//Sleep mode
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
volatile int f_wdt = 1;

//Include libraries for SD functions
#include <SPI.h>
#include <SD.h>
const int chipSelect = 10;
File myFile;

#include <CapacitiveSensor.h>
#define Send 3 //Reservoir sensor send
#define Rec 2  //Reservoir sensor recieve
CapacitiveSensor Reservoir = CapacitiveSensor(Send, Rec);


//Global variables
int Sensor01ValPre;
int Sensor02ValPre;
int Sensor01ValPost;
int Sensor02ValPost;
int Plant01Low = 500;
int Plant02Low = 500;
bool Pump01 = 0;
int PumpDelay = 15000;

//Define pins
#define PwrSensor01 9  //Pin to turn on moisture sensor 01
#define PwrSensor02 8  //Pin to turn on moisture sensor 02
#define PwrSensor03 7  //Pin to turn on moisture sensor 03
#define PwrSensor04 6  //Pin to turn on moisture sensor 04
#define Sensor01 A7  //Pin to read moisture sensor 01
#define Sensor02 A6  //Pin to read moisture sensor 02
#define Sensor03 A5  //Pin to read moisture sensor 03
#define Sensor04 A4  //Pin to read moisture sensor 04
#define Motor01 A0  //Pin to turn on motor 01
#define Motor02 A1  //Pin to turn on motor 02
#define Motor03 A2  //Pin to turn on motor 03


void setup() {
  Serial.begin(9600);

  //Set pin status
  pinMode(PwrSensor01, OUTPUT);
  pinMode(PwrSensor02, OUTPUT);
  pinMode(PwrSensor03, OUTPUT);
  pinMode(PwrSensor04, OUTPUT);
  pinMode(Sensor01, INPUT);
  pinMode(Sensor02, INPUT);
  pinMode(Sensor03, INPUT);
  pinMode(Sensor04, INPUT);
  pinMode(Motor01, OUTPUT);
  pinMode(Motor02, OUTPUT);
  pinMode(Motor03, OUTPUT);

  //Set pin levels
  digitalWrite(PwrSensor01, LOW);
  digitalWrite(PwrSensor02, LOW);
  digitalWrite(PwrSensor03, LOW);
  digitalWrite(PwrSensor04, LOW);
  digitalWrite(Motor01, LOW);
  digitalWrite(Motor02, LOW);
  digitalWrite(Motor03, LOW);

  //Check SD card function
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Failed!");
    while (1);
  }
  Serial.println("Done.");
  delay(100);

  Error();

  //Write note to SD
  myFile = SD.open("P7_0_0.CSV", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    delay(100);
    myFile.println("Reboot");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }


  TestMotors();


}

void loop() {
  //Read all sensors
  Sensor01ValPre = CheckSensors(Sensor01, PwrSensor01);
  Sensor02ValPre = CheckSensors(Sensor02, PwrSensor02);
  Serial.println("HI");

  //Water plants if needed
  WaterPlants();

  Sensor01ValPost = CheckSensors(Sensor01, PwrSensor01);
  Sensor02ValPost = CheckSensors(Sensor02, PwrSensor02);

  //Record for log
  WriteToSD();
}




//Function to run pumps if moisture level is low
void WaterPlants() {
  Pump01 = 0;
  //Water plant 01 & 02
  if (Sensor01ValPre < Plant01Low)
  {
    Serial.println("01");
  }
  if ((Sensor01ValPre < Plant01Low) || (Sensor02ValPre < Plant02Low))
  {

    while ((Sensor01ValPost < Plant01Low) || (Sensor02ValPost < Plant02Low))
    {
      digitalWrite(Motor01, HIGH);
      delay(1000);//delay(PumpDelay);
      digitalWrite(Motor01, LOW);

      Sensor01ValPost = CheckSensors(Sensor01, PwrSensor01);
      Sensor02ValPost = CheckSensors(Sensor02, PwrSensor02);
      Serial.print(Sensor01ValPost);
      Serial.print(",");
      Serial.println(Sensor02ValPost);
    }

    Pump01 = 1;
    delay(60000);
  }

}

//Function to write data to SD card
void WriteToSD() {
  //Open File
  myFile = SD.open("P7_0_0.CSV", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.println("Writing to P7_0_0.csv...");
    myFile.print(Sensor01ValPre);
    myFile.print(",");
    myFile.print(Sensor02ValPre);
    myFile.print(",");
    myFile.print(Pump01);
    myFile.print(",");
    myFile.print(Sensor01ValPost);
    myFile.print(",");
    myFile.println(Sensor02ValPost);

    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}


int CheckSensors(int Sensor, int SensorPower) {
  int DelayTime = 200;
  int Result;
  digitalWrite(SensorPower, HIGH);
  delay(DelayTime);
  Result = analogRead(Sensor);
  digitalWrite(SensorPower, LOW);


  Serial.println(Result);
  digitalWrite(PwrSensor01, HIGH);
  delay(200);
  int Sensor01Val = analogRead(Sensor01);
  digitalWrite(PwrSensor01, LOW);
  Serial.print(">>>>>");
  Serial.print(Sensor01Val);
  return Result;

}

void TestMotors()
{
  Serial.println("Motor 1");
  digitalWrite(Motor01, HIGH);
  delay(1000);
  digitalWrite(Motor01, LOW);
  delay(1000);

  Serial.println("Motor 2");
  digitalWrite(Motor02, HIGH);
  delay(1000);
  digitalWrite(Motor02, LOW);
  delay(1000);

  Serial.println("Motor 3");
  digitalWrite(Motor03, HIGH);
  delay(1000);
  digitalWrite(Motor03, LOW);
  delay(1000);
}
void Error() {
  pinMode(4, OUTPUT);
  while (true)
  {
    digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
    delay(100);                       // wait for a second
  }
}

/*
***** ROLL THE CREDITS *****
  >> WriteToSD <<
   SD tutorial example sketch   https://www.arduino.cc/en/Tutorial/ReadWrite
   DIY Arduino SD card reader   https://www.instructables.com/id/Cheap-Arduino-SD-Card-Reader/
   DIY SD card Reader           https://nathan.chantrell.net/20111128/diy-micro-sd-shield-for-arduino/
   DIY capacitive sensor        https://www.instructables.com/id/Building-a-Capacitive-Liquid-Sensor/
***** Thanks everyone! *****
*/
