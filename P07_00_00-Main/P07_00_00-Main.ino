/*
 * P07_00_00-Main 
 * Project: P07_00_00 Raindrop Bob
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
#define Motor01 7  //Pin to turn on motor 01
#define Motor02 8  //Pin to turn on motor 02
#define Motor03 9  //Pin to turn on motor 03

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
  digitalWrite(PwrSensor01,LOW);
  digitalWrite(PwrSensor02,LOW);
  digitalWrite(PwrSensor03,LOW);
  digitalWrite(PwrSensor04,LOW);
  digitalWrite(Motor01,LOW);
  digitalWrite(Motor02,LOW);
  digitalWrite(Motor03,LOW);

  //Check SD card function
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  

}

void loop() {
  WriteToSD();
}

//Function to write data to SD card
void WriteToSD(){
  //Open File
  myFile = SD.open("P07_00_00-Data.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void CheckSensors(){
  //Sensor value totals for calculating the average
  uint16_t TotalSensor01 = 0;
  uint16_t TotalSensor02 = 0;
  uint16_t TotalSensor03 = 0;
  uint16_t TotalSensor04 = 0;

  //Check each sensor five times
  for (int i = 0; i<5; i++){
    //Check sensor 01
    digitalWrite(PwrSensor01,HIGH);
    delay(200);
    TotalSensor01 = TotalSensor01 + analogRead(Sensor01);
    digitalWrite(PwrSensor01,LOW);

    //Check sensor 02
    digitalWrite(PwrSensor02,HIGH);
    delay(200);
    TotalSensor02 = TotalSensor02 + analogRead(Sensor02);
    digitalWrite(PwrSensor02,LOW);

    //Check sensor 03
    digitalWrite(PwrSensor03,HIGH);
    delay(200);
    TotalSensor03 = TotalSensor03 + analogRead(Sensor03);
    digitalWrite(PwrSensor03,LOW);

    //Check sensor 04
    digitalWrite(PwrSensor04,HIGH);
    delay(200);
    TotalSensor04 = TotalSensor04 + analogRead(Sensor04);
    digitalWrite(PwrSensor04,LOW);
  }

  //Calculate an average
  Sensor01Val = TotalSensor01/5;
  Sensor02Val = TotalSensor02/5;
  Sensor03Val = TotalSensor03/5;
  Sensor04Val = TotalSensor04/5;
}
