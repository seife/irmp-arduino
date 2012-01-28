/* This is a
/* use TimerOne http://arduino.cc/playground/Code/Timer1 for interrupts */
#include <TimerOne.h>
/* first include Arduino.h, the IDE includes it after irmp*.h ... */
#include "Arduino.h"
/* ... and then chokes on uintX_t ... */

extern "C" {
#include <irmpconfig.h>
#include <irmp.h>
}

/* undefine this if you don't want blinking LED for diagnosis */
#define LED_PIN 13
#define SER_BAUD 115200

/* F_INTERRUPTS is the interrupt frequency defined in irmpconfig.h */
#define US (1000000 / F_INTERRUPTS)
void setup()
{
  Serial.begin(SER_BAUD);
  /* greeting string and debugging ouput */
  Serial.println("IRMP test sketch");
  Serial.print("US: ");
  Serial.println(US);
  /* configure the input pin. IRMP_BIT is defined in irmpconfig.h,
     default is digital pin 2 */
  pinMode(IRMP_BIT, INPUT);
#ifdef LED_PIN
  pinMode(LED_PIN, OUTPUT);
#endif
  led(HIGH);
  delay(20); /* make sure the greeting string is out before starting */
  led(LOW);
  Timer1.initialize(US);
  Timer1.attachInterrupt(timerinterrupt);
}

IRMP_DATA irmp_data;
void loop()
{
  if (irmp_get_data (&irmp_data))
  {
    led(HIGH);
    Serial.print(irmp_data.protocol);
    Serial.print(" A:");
    Serial.print(irmp_data.address, HEX);
    Serial.print(" C:");
    Serial.print(irmp_data.command, HEX);
    Serial.print(" ");
    Serial.print(irmp_data.flags, HEX);
    Serial.println("");
    /* Serial.print is asynchronous, so the LED is only flashing very lightly */
    led(LOW);
  }
}

/* helper function: attachInterrupt wants void(), but irmp_ISR is uint8_t() */
void timerinterrupt()
{
  irmp_ISR();
}

static inline void led(int state)
{
#ifdef LED_PIN
  digitalWrite(LED_PIN, state);
#endif
}
