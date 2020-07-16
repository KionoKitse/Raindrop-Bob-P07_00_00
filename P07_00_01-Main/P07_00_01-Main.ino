/*
   P07_00_00-Main
   Project: P07_00_00 Raindrop Bob
*/
//Sleep mode
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
volatile int f_wdt=1;

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
int Sensor01Val;
int Sensor02Val;
int Sensor03Val;
int Sensor04Val;
long ReservoirVal;
int Plant01Low = 40;
int Plant02Low = 131;
int Plant03Low = 650;
int Plant04Low = 680;
int PumpDelay = 5000;
bool Pump01 = 0;
bool Pump02 = 0;
bool Pump03 = 0;

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

  //Sleep mode setup
  MCUSR &= ~(1<<WDRF);
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  WDTCSR = 1<<WDP0 | 1<<WDP3; 
  WDTCSR |= _BV(WDIE);

  //TestMotors();
}

void loop() {
  //Check if it's time to take a measuremet
  
  //Read all sensors
  CheckSensors();

  //Water plants if needed
  WaterPlants();

  //Record for log
   WriteToSD();
   
  //Sleep 30 min
  SleepMode(1800000);
  
  
}


//Funcnction to be in sleep mode
void SleepMode(long TimeMs){
  Serial.println("Going to sleep");
  delay(100);
  //Determine cycles to sleep 
  int CyclesToSleep  = TimeMs/8000;
  int RemainderTime = TimeMs - CyclesToSleep*8000;

  //Enter sleep mode for the number of cycles required
  bool KeepSleeping = true;
  volatile byte SleepCycles = 0;
  while (KeepSleeping)
  {
    //Watchdog just woke up
    if(f_wdt == 1)
    {
      //Check if it's time to wake up
      if (SleepCycles < CyclesToSleep)
      {
        SleepCycles++;
      }
      else
      {
        break;
      }
      
      //Clear the flag
      f_wdt = 0;
      
      //Re-enter sleep mode.
      enterSleep();
    }
  }
  //Delay for the remainder of the time
  delay(RemainderTime);
  
}

//Watchdog Interrupt Service. This is executed when watchdog timed out.
ISR(WDT_vect)
{
  if(!f_wdt)
  {
    f_wdt=true;
  }
  else
  {
    Serial.println("WDT Overrun!!!");
  }
}

//Enters the arduino into sleep mode.
void enterSleep(void)
{
  //Set sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   
  sleep_enable();
  
  //Enter sleep mode
  sleep_mode();
  
  //After waking up disable the sleep mode
  sleep_disable(); 
  
  // Re-enable the peripherals
  //power_all_enable();
}

//Function to run pumps if moisture level is low
void WaterPlants(){
   Pump01 = 0;
   Pump02 = 0;
   Pump03 = 0;
  //Water plant 03
  if (Sensor03Val < Plant03Low)
  {
    digitalWrite(Motor02, HIGH);
    delay(PumpDelay);
    digitalWrite(Motor02, LOW);
    Pump01 = 1;
  }
  //Water plant 04
  if (Sensor04Val < Plant04Low)
  {
    digitalWrite(Motor03, HIGH);
    delay(PumpDelay);
    digitalWrite(Motor03, LOW);
    Pump02 = 1;
  }
}

//Function to write data to SD card
void WriteToSD() {
  //Open File
  myFile = SD.open("P7_0_0.CSV", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.println("Writing to P7_0_0.csv...");
    myFile.print(Sensor01Val);
    myFile.print(",");
    myFile.print(Sensor02Val);
    myFile.print(",");
    myFile.print(Sensor03Val);
    myFile.print(",");
    myFile.print(Sensor04Val);
    myFile.print(",");
    myFile.print(ReservoirVal);
    myFile.print(",");
    myFile.print(Pump01);
    myFile.print(",");
    myFile.print(Pump02);
    myFile.print(",");
    myFile.println(Pump03);

    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void CheckSensors() {
  int DelayTime = 100;
  //Sensor value totals for calculating the average
  uint16_t TotalSensor01 = 0;
  uint16_t TotalSensor02 = 0;
  uint16_t TotalSensor03 = 0;
  uint16_t TotalSensor04 = 0;
  long TotalReservoir = 0;

  //Check each sensor five times
  for (int i = 0; i < 5; i++) {
    //Check sensor 01
    digitalWrite(PwrSensor01, HIGH);
    delay(DelayTime);
    TotalSensor01 = TotalSensor01 + analogRead(Sensor01);
    digitalWrite(PwrSensor01, LOW);

    //Check sensor 02
    digitalWrite(PwrSensor02, HIGH);
    delay(DelayTime);
    TotalSensor02 = TotalSensor02 + analogRead(Sensor02);
    digitalWrite(PwrSensor02, LOW);

    //Check sensor 03
    digitalWrite(PwrSensor03, HIGH);
    delay(DelayTime);
    TotalSensor03 = TotalSensor03 + analogRead(Sensor03);
    digitalWrite(PwrSensor03, LOW);

    //Check sensor 04
    digitalWrite(PwrSensor04, HIGH);
    delay(DelayTime);
    TotalSensor04 = TotalSensor04 + analogRead(Sensor04);
    digitalWrite(PwrSensor04, LOW);

    //Check reservoir
    TotalReservoir = TotalReservoir + Reservoir.capacitiveSensorRaw(200);
  }

  //Calculate an average
  Sensor01Val = TotalSensor01 / 5;
  Sensor02Val = TotalSensor02 / 5;
  Sensor03Val = TotalSensor03 / 5;
  Sensor04Val = TotalSensor04 / 5;
  ReservoirVal = TotalReservoir / 5;

  Serial.print(Sensor01Val);
  Serial.print(",");
  Serial.print(Sensor02Val);
  Serial.print(",");
  Serial.print(Sensor03Val);
  Serial.print(",");
  Serial.print(Sensor04Val);
  Serial.print(",");
  Serial.print(ReservoirVal);
  Serial.println("");

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

/*
***** ROLL THE CREDITS *****
  >> WriteToSD <<
   SD tutorial example sketch   https://www.arduino.cc/en/Tutorial/ReadWrite
   DIY Arduino SD card reader   https://www.instructables.com/id/Cheap-Arduino-SD-Card-Reader/
   DIY SD card Reader           https://nathan.chantrell.net/20111128/diy-micro-sd-shield-for-arduino/
   DIY capacitive sensor        https://www.instructables.com/id/Building-a-Capacitive-Liquid-Sensor/
***** Thanks everyone! *****
*/
