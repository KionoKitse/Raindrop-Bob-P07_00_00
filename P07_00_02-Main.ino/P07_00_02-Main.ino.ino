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

//Global variables
int Sensor01ValPre;
int Sensor02ValPre;
int Sensor01ValPost;
int Sensor02ValPost;
int Plant01Low = 500;
int Plant02Low = 500;
bool Pump01 = 0;
int PumpDelay = 60000;
bool HardExit = false;

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
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  //Check sensors
  Serial.println("Check sensors");
  digitalWrite(PwrSensor01, HIGH);
  delay(200);
  Sensor01ValPre = analogRead(Sensor01);
  digitalWrite(PwrSensor01, LOW);

  digitalWrite(PwrSensor02, HIGH);
  delay(200);
  Sensor02ValPre = analogRead(Sensor02);
  digitalWrite(PwrSensor02, LOW);

  Serial.print(Sensor01ValPre);
  Serial.print(",");
  Serial.println(Sensor02ValPre);

  WaterPlants();

  //Check sensors again
  Serial.println("Check sensors again");
  digitalWrite(PwrSensor01, HIGH);
  delay(200);
  Sensor01ValPost = analogRead(Sensor01);
  digitalWrite(PwrSensor01, LOW);

  digitalWrite(PwrSensor02, HIGH);
  delay(200);
  Sensor02ValPost = analogRead(Sensor02);
  digitalWrite(PwrSensor02, LOW);
  
  Serial.print(Sensor01ValPost);
  Serial.print(",");
  Serial.println(Sensor02ValPost);

  Serial.println("Write to SD");
  WriteToSD();
  if (HardExit)
  {
    Error();
  }
}

void loop() {
}
//Function to run pumps if moisture level is low
void WaterPlants() {
  Pump01 = 0;

  //Water plant 01 & 02
  if (Sensor01ValPre < Plant01Low)
  {
    Serial.print("Plant1 low: ");
    Serial.println(Sensor01ValPre);
  }
  if (Sensor02ValPre < Plant02Low)
  {
    Serial.print("Plant2 low: ");
    Serial.println(Sensor02ValPre);
  }
  if ((Sensor01ValPre < Plant01Low) || (Sensor02ValPre < Plant02Low))
  {
    Serial.println("A plant needs water!");
    Pump01 = 1;
    HardExit = true;

    //Turn on pumps for a bit
    digitalWrite(Motor01, HIGH);
    delay(120000);
    digitalWrite(Motor01, LOW);

    Serial.println("Pumps off: wait 2 min");
    delay(120000);
  }
}

//Function to write data to SD card
void WriteToSD() {
  //Open File
  myFile = SD.open("P7_0_0L.csv", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    myFile.print(Sensor01ValPre);
    myFile.print(",");
    myFile.print(Sensor02ValPre);
    myFile.print(",");
    myFile.print(Pump01);
    myFile.print(",");
    myFile.print(Sensor01ValPost);
    myFile.print(",");
    myFile.print(Sensor02ValPost);
    myFile.print(",");
    myFile.println(HardExit);

    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening P7_0_0L.csv");
  }
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
***** Thanks everyone! *****
*/
