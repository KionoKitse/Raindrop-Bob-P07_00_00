/*
   P07_00_00-TestSD
   Project: P07_00_00 Raindrop Bob
   Schematic: P07_00_00-TestSD.fzz

   Description: Testing SD card functions. Write a message to the txt
   file.

   Notes: Text file max length is 7 or 8 characters
*/

//Include libraries for SD functions
#include <SPI.h>
#include <SD.h>
const int chipSelect = 10;
File myFile;

//Global variables
int Sensor01Val = 1;
int Sensor02Val = 2;
int Sensor03Val = 3;
int Sensor04Val = 4;

void setup() {
  Serial.begin(9600);

  //Check SD card function
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}

void loop() {
  //Open File
  myFile = SD.open("P7_0_0.csv", FILE_WRITE);
  if (myFile) {
    Serial.println("Writing to P7_0_0.csv...");
    Serial.print("Testing SD Write");

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
  delay(2000);
}

/*
***** ROLL THE CREDITS *****
>> GENERAL <<
 * SD tutorial example sketch   https://www.arduino.cc/en/Tutorial/ReadWrite
 * DIY Arduino SD card reader   https://www.instructables.com/id/Cheap-Arduino-SD-Card-Reader/
 * DIY SD card Reader           https://nathan.chantrell.net/20111128/diy-micro-sd-shield-for-arduino/
***** Thanks everyone! ***** 
 */
