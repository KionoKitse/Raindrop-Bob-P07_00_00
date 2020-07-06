
//Sleep mode
#include <avr/wdt.h>            // library for default watchdog functions
#include <avr/interrupt.h>      // library for interrupts handling
#include <avr/sleep.h>          // library for sleep
#include <avr/power.h>          // library for power control
int nbr_remaining;              // how many times remain to sleep before wake up

//Include libraries for SD functions
#include <SPI.h>
#include <SD.h>
const int chipSelect = 10;
File myFile;

//Capacitive Reservoir sensor
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


void setup(){
  //Serial communication
  Serial.begin(9600);
  Serial.println("");
  Serial.println("Setup");

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
  Serial.print("Start SD ...");
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
    Serial.print("Write Reboot ...");
    delay(100);
    myFile.println("Reboot");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("Failed");
  }

  // configure the watchdog
  configure_wdt();
}

void loop(){
  Serial.println("Loop");
  delay(5);
  
  // sleep for a given number of cycles (here, 5 * 8 seconds) in lowest power mode
  sleep(255);
  
  // usefull stuff should be done here before next long sleep
  CheckSensors();
  WriteToSD();
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
    myFile.println(ReservoirVal);

    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("Faild");
  }
}

// interrupt raised by the watchdog firing
// when the watchdog fires during sleep, this function will be executed
// remember that interrupts are disabled in ISR functions
ISR(WDT_vect)
{
        // not hanging, just waiting
        // reset the watchdog
        wdt_reset();
}

// function to configure the watchdog: let it sleep 8 seconds before firing
// when firing, configure it for resuming program execution
void configure_wdt(void)
{
 
  cli();                           // disable interrupts for changing the registers

  MCUSR = 0;                       // reset status register flags

                                   // Put timer in interrupt-only mode:                                       
  WDTCSR |= 0b00011000;            // Set WDCE (5th from left) and WDE (4th from left) to enter config mode,
                                   // using bitwise OR assignment (leaves other bits unchanged).
  WDTCSR =  0b01000000 | 0b100001; // set WDIE: interrupt enabled
                                   // clr WDE: reset disabled
                                   // and set delay interval (right side of bar) to 8 seconds

  sei();                           // re-enable interrupts

  // reminder of the definitions for the time before firing
  // delay interval patterns:
  //  16 ms:     0b000000
  //  500 ms:    0b000101
  //  1 second:  0b000110
  //  2 seconds: 0b000111
  //  4 seconds: 0b100000
  //  8 seconds: 0b100001
 
}

// Put the Arduino to deep sleep. Only an interrupt can wake it up.
void sleep(int ncycles)
{  
  nbr_remaining = ncycles; // defines how many cycles should sleep

  // Set sleep to full power down.  Only external interrupts or
  // the watchdog timer can wake the CPU!
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
 
  // Turn off the ADC while asleep.
  power_adc_disable();
 
  while (nbr_remaining > 0){ // while some cycles left, sleep!

  // Enable sleep and enter sleep mode.
  sleep_mode();

  // CPU is now asleep and program execution completely halts!
  // Once awake, execution will resume at this point if the
  // watchdog is configured for resume rather than restart
 
  // When awake, disable sleep mode
  sleep_disable();
  
  // we have slept one time more
  nbr_remaining = nbr_remaining - 1;
 
  }
 
  // put everything on again
  power_all_enable();
 
}
