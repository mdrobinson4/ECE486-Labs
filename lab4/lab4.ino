/*Author: Mark Robinson
 * CWID: 11635959
 * Lab: Lab 4
 * Date: February 25, 2019
 */


#include <avr/wdt.h>
#include <fourDigitDisplay.h>

// Initialize 4-segment display object
fourDigitDisplay bcd;

/*Name: watchdogSetup
 * Inputs: void
 * Outputs: void
 * Functions: Sets the watchdog timer to 4 seconds
 */
void watchdogSetup(void) {
 cli();
 wdt_reset();
 
 // Enter Watchdog Configuration mode:
 WDTCSR |= (1<<WDCE) | (1<<WDE);
 // Set Watchdog settings:
 WDTCSR = (1<<WDIE) | (1<<WDE) |
 (1<<WDP3) | (0<<WDP2) | (0<<WDP1) |
 (0<<WDP0);

 sei();
}

void setup() {
  Serial.begin(9600);
  Serial.println("Board Was Reset");
  reset();
  watchdogSetup();  // Configures watchdog timer
}

void loop() {
  String inString = "";
  int potPin = 0;
  
  while(1) {
    while (Serial.available() > 0) {
      wdt_reset();
      char inChar = Serial.read();
      if (inChar != '\n') {
        inString += (char)inChar;
      }
      // if you get a newline, print the string, then the string's value:
      else if (inChar == '\n') {
        if (inString.length() == 1 && inString[0] == 'c') {
          convert(potPin);
        }
        else {
          Serial.println("Error: invalid user input - the only vaid user input is 'c'");
          Serial.println();
        }
        inString = "";
      }
    }
  }
}

/*Name: reset
 * Inputs: void
 * Outputs: void
 * Functions: Displays the welcome message
 */
void reset() {
  Serial.println("Enter 'c' To Start A Set Of Conversions");
  Serial.println();
  wdt_reset();
  return;
}

/*Name: convert
 * Inputs: void
 * Outputs: void
 * Functions: Reads the analog value at the potentiometer,
 * converts it to a digital value, and displays the values
 * in hex and the time it took to convert
 */
void convert(int potPin) {
  unsigned long tempTime[30];
  char hex[50] = {0};
  for (int i = 0; i < 30; i++) {
    wdt_reset();
    tempTime[i] = micros();
    int value = analogRead(potPin);
    tempTime[i] = micros() - tempTime[i];
    Serial.print("#");
    Serial.print(i + 1);
    Serial.print(":   digital value = ");
    sprintf(hex, "%03X", value);
    Serial.print(hex);
    Serial.print("    Time = ");
    Serial.print(tempTime[i]);
    Serial.println(" usecs");
    Serial.println();
    delay(1000);
  }
  unsigned long avg = 0;
  for (int i = 0; i < 30; i++) {
    wdt_reset();
    avg += tempTime[i];
  }
  Serial.print("avg conversion time = ");
  Serial.print((unsigned long)avg / 30.00);
  Serial.println(" usecs");
  Serial.println();
  while (Serial.available() > 0) {
    Serial.read();
    wdt_reset();
  }
  wdt_reset();
  reset();
}
