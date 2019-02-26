/*Author: Mark Robinson
 * CWID: 11635959
 * Lab: Lab 4
 * Date: February 25, 2019
 */


#include <avr/wdt.h>
#include <fourDigitDisplay.h>

// Initialize 4-segment display object
fourDigitDisplay bcd;

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

unsigned long time;

void setup() {
  Serial.begin(9600);
  reset();
  watchdogSetup();  // Configures watchdog timer
}

void loop() {
  String inString = "";
  int potPin = 0;
  
  while(1) {
    //Serial.println(decToHexa(1));
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

void reset() {
  Serial.println("Board Was Reset");
  Serial.println("Enter 'c' To Start A Set Of Conversions");
  Serial.println();
  return;
}

void convert(int potPin) {
  unsigned long tempTime[30];
  for (int i = 0; i < 30; i++) {
    wdt_reset();
    tempTime[i] = micros();
    int value = analogRead(potPin);
    tempTime[i] = micros() - tempTime[i];
    Serial.print("#");
    Serial.print(i + 1);
    Serial.print(": digital value = ");
    Serial.print(value, HEX );
    Serial.print(" ---- ");
    Serial.print(value, DEC);
    Serial.print(" Time = ");
    Serial.print(tempTime[i]);
    Serial.println(" usecs");
    Serial.println();
    delay(1000);
  }
  int avg = 0;
  for (int i = 0; i < 30; i++) {
    wdt_reset();
    avg += tempTime[i];
  }
  Serial.print("avg conversion time = ");
  Serial.print(avg / 30);
  Serial.println(" usecs");
  Serial.println();
}

// function to convert decimal to hexadecimal 
String decToHexa(int n) { 
    // char array to store hexadecimal number 
    char hexaDeciNum[100]; 
      
    // counter for hexadecimal number array 
    int i = 0; 
    while(n!=0) 
    {    
        // temporary variable to store remainder 
        int temp  = 0; 
          
        // storing remainder in temp variable. 
        temp = n % 16; 
          
        // check if temp < 10 
        if(temp < 10) 
        { 
            hexaDeciNum[i] = temp + 48; 
            i++; 
        } 
        else
        { 
            hexaDeciNum[i] = temp + 55; 
            i++; 
        } 
          
        n = n/16; 
    } 
    String hex = "";
    // printing hexadecimal number array in reverse order 
    for(int j=i-1; j>=0; j--) 
        hex += hexaDeciNum[j]; 
    return hex;
} 
