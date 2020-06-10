/*
   P07_00_00-CalibrateSensors
   Project: P07_00_00 Raindrop Bob
   Schematic: P07_00_00-CalibrateSensors.fzz
   
   Description: Every hour take five sample measurements from the four 
   moisture sensors. Save that data to the SD card in CSV format for 
   review latter.
*/

//Include libraries for SD functions
#include <SPI.h>
#include <SD.h>
const int chipSelect = 10;
File myFile;

//Global variables
int Sensor01Val;
int Sensor02Val;
int Sensor03Val;
int Sensor04Val;

//Define pins
#define PwrSensor01 3  //Pin to turn on moisture sensor 01
#define PwrSensor02 4  //Pin to turn on moisture sensor 02
#define PwrSensor03 5  //Pin to turn on moisture sensor 03
#define PwrSensor04 6  //Pin to turn on moisture sensor 04
#define Sensor01 A0  //Pin to read moisture sensor 01
#define Sensor02 A1  //Pin to read moisture sensor 02
#define Sensor03 A2  //Pin to read moisture sensor 03
#define Sensor04 A3  //Pin to read moisture sensor 04

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

  //Set pin levels
  digitalWrite(PwrSensor01, LOW);
  digitalWrite(PwrSensor02, LOW);
  digitalWrite(PwrSensor03, LOW);
  digitalWrite(PwrSensor04, LOW);

  //Check SD card function
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");


}

void loop() {
  CheckSensors();
  WriteToSD();
  delay(60UL * 60UL * 1000UL); // 1 hour
}

//Function to write data to SD card
void WriteToSD() {
  //Open File
  myFile = SD.open("P7_0_0.csv", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.println("Writing to P7_0_0.csv...");
    Serial.print(Sensor01Val);
    Serial.print(",");
    Serial.print(Sensor02Val);
    Serial.print(",");
    Serial.print(Sensor03Val);
    Serial.print(",");
    Serial.println(Sensor04Val);

    myFile.print(Sensor01Val);
    myFile.print(",");
    myFile.print(Sensor02Val);
    myFile.print(",");
    myFile.print(Sensor03Val);
    myFile.print(",");
    myFile.println(Sensor04Val);

    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening P7_0_0.csv");
  }
}

void CheckSensors() {
  Serial.println("Read Sensors");

  //Sensor value totals for calculating the average
  int TotalSensor01 = 0;
  int TotalSensor02 = 0;
  int TotalSensor03 = 0;
  int TotalSensor04 = 0;

  //Check each sensor five times
  for (int i = 0; i < 5; i++) {
    //Check sensor 01
    digitalWrite(PwrSensor01, HIGH);
    delay(200);
    TotalSensor01 = TotalSensor01 + analogRead(Sensor01);
    digitalWrite(PwrSensor01, LOW);

    //Check sensor 02
    digitalWrite(PwrSensor02, HIGH);
    delay(200);
    TotalSensor02 = TotalSensor02 + analogRead(Sensor02);
    digitalWrite(PwrSensor02, LOW);

    //Check sensor 03
    digitalWrite(PwrSensor03, HIGH);
    delay(200);
    TotalSensor03 = TotalSensor03 + analogRead(Sensor03);
    digitalWrite(PwrSensor03, LOW);

    //Check sensor 04
    digitalWrite(PwrSensor04, HIGH);
    delay(200);
    TotalSensor04 = TotalSensor04 + analogRead(Sensor04);
    digitalWrite(PwrSensor04, LOW);
  }

  //Calculate an average
  Sensor01Val = TotalSensor01 / 5;
  Sensor02Val = TotalSensor02 / 5;
  Sensor03Val = TotalSensor03 / 5;
  Sensor04Val = TotalSensor04 / 5;
}

/*
***** ROLL THE CREDITS *****
>> WriteToSD <<
 * SD tutorial example sketch   https://www.arduino.cc/en/Tutorial/ReadWrite
 * DIY Arduino SD card reader   https://www.instructables.com/id/Cheap-Arduino-SD-Card-Reader/
 * DIY SD card Reader           https://nathan.chantrell.net/20111128/diy-micro-sd-shield-for-arduino/
***** Thanks everyone! ***** 
 */