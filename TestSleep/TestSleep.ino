/*
 * Sketch for testing sleep mode with wake up on WDT.
 * Donal Morrissey - 2011.
 *
 */
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#define LED_PIN (13)

volatile int Count=0;
volatile int f_wdt=1;

//Watchdog Interrupt Service. This is executed when watchdog timed out.
ISR(WDT_vect)
{
  if(f_wdt == 0)
  {
    f_wdt=1;
  }
  else
  {
    Serial.println("WDT Overrun!!!");
  }
}

//Enters the arduino into sleep mode.
void enterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
  sleep_enable();
  
  /* Now enter sleep mode. */
  sleep_mode();
  
  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  
  /* Re-enable the peripherals. */
  power_all_enable();
}



void setup()
{
  pinMode(LED_PIN,OUTPUT);
  MCUSR &= ~(1<<WDRF);
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  WDTCSR = 1<<WDP0 | 1<<WDP3; 
  WDTCSR |= _BV(WDIE);
}



void loop()
{
  if(f_wdt == 1)
  {
    if (Count < 2)
    {
      Count++;
    }
    else
    {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      Count = 0;
    }

    f_wdt = 0;
    
    /* Re-enter sleep mode. */
    enterSleep();
  }
  else
  {
    /* Do nothing. */
  }
}
//http://donalmorrissey.blogspot.com/2010/04/sleeping-arduino-part-5-wake-up-via.html
