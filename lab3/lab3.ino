/*Author: Mark Robinson
 * CWID: 11635959
 * Lab: Lab 3
 * Date: February 21, 2019
 */

int wdtFlag = 0;
int flag = 0;

#include <avr/wdt.h>
#include <fourDigitDisplay.h>

// Initialize 4-segment display object
fourDigitDisplay bcd;

void watchdogSetup(void)
{
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
  bcd.startUp();
  watchdogSetup();
}

void loop() {
  restart();
  //Serial.flush();
  start();
  while (Serial.available() > 0)
    Serial.read();
    
  String inString = "";    // string to hold input
  String currValue = "";


  while(1) {
    // Read serial input:
      while (Serial.available() > 0) {
        wdt_reset();
        int inChar = Serial.read();
        if (isDigit(inChar)) {
          // convert the incoming byte to a char and add it to the string:
          inString += (char)inChar;
        }
       if ((!isDigit(inChar) && inChar != '\n')) {
        while (inChar != '\n') {
          inString += (char)inChar;
          inChar = Serial.read();
        }
        invalid(inString);
        inString = "";
        }
        // if you get a newline, print the string, then the string's value:
        else if (inChar == '\n') {
          int input = inString.toInt();
        if (input < 0 || input > 15) {
          invalid(String(input));
          inString = "";
        }
        else {
          if (input > 9)
            valid(String(input, HEX));
          else
            valid((inString));
          inString = "";
          bcd.display((int)input);
        } 
        inString = "";
        }
      }
  }
}

void invalid(String input) {
  bcd.invalidInput();
  Serial.print("Warning You Entered Invalid Data: ");
  for (int i = 0; i < input.length(); i++) {
    if (isPrintable(input.charAt(i)))
      Serial.print((char)input.charAt(i));
  }
  Serial.println();
}

void valid(String input) {
  input.toUpperCase();
  Serial.print("Hexidecimal Representation Of Inputted Data: ");
  Serial.println(input);
}

void start() {
    bcd.startUp();
    Serial.println();
    Serial.println("Please Enter An Integer In The Range OF [0 - 15]");
    Serial.println("The Value Will Be Displayed In Hexidecimal");
    Serial.println("The Value Being Displayed On The 7-Segment Display Is A Hexidecimal Value If The Period Is Lit");
    Serial.println();
    Serial.println();
}

void restart() {
  for (int i = 0; i < 5; i++) {
    bcd.decimal(1);
    delay(500);
    bcd.decimal(0);
    delay(500);
    wdt_reset();
  }
  wdtFlag = 0;
  wdt_reset();
  bcd.clear();
  return;
}


ISR(WDT_vect) // Watchdog timer interrupt.
{
  Serial.println("Sorry, Time Ran Out!");
}
