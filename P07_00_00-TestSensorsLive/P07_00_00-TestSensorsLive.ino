/*
   P07_00_00-TestSensorsLive
   Project: P07_00_00 Raindrop Bob
   Schematic: P07_00_00-TestSensorsLive.fzz
   
   Description: Stream soil moisture sensor values over serial and 
   save to SD for graphing.
   
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
}

//Function to write data to SD card
void WriteToSD() {
  //Open File
  myFile = SD.open("P7_0_0L.csv", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
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
    Serial.println("error opening P7_0_0L.csv");
  }
}

void CheckSensors() {
  //Check sensor 01
    digitalWrite(PwrSensor01, HIGH);
    delay(200);
    Sensor01Val = analogRead(Sensor01);
    digitalWrite(PwrSensor01, LOW);
    Serial.print(Sensor01Val);
    Serial.print(",");

    //Check sensor 02
    digitalWrite(PwrSensor02, HIGH);
    delay(200);
    Sensor02Val = analogRead(Sensor02);
    digitalWrite(PwrSensor02, LOW);
    Serial.print(Sensor02Val);
    Serial.print(",");
    
    //Check sensor 03
    digitalWrite(PwrSensor03, HIGH);
    delay(200);
    Sensor03Val = analogRead(Sensor03);
    digitalWrite(PwrSensor03, LOW);
    Serial.print(Sensor03Val);
    Serial.print(",");

    //Check sensor 04
    digitalWrite(PwrSensor04, HIGH);
    delay(200);
    Sensor04Val = analogRead(Sensor04);
    digitalWrite(PwrSensor04, LOW);
    Serial.print(Sensor04Val);
    Serial.print(" Write: ");
}
