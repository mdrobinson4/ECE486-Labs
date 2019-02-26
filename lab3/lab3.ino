/*Author: Mark Robinson
 * CWID: 11635959
 * Lab: Lab 3
 * Date: February 25, 2019
 */
 int flag = 0;

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
  bcd.startUp();  // Sets the bcd to only display the horizontal segment
  watchdogSetup();  // Configures watchdog timer
}

void loop() {
  //restart();  // Blinks the decimal for five seconds
  start();  // Shows welcome message
  while (Serial.available() > 0)  // Gets rid of erraneous input
    Serial.read();
    
  String inString = "";    // string to hold input

  while(1) {
    if (flag == 1) {
      flag++;
      Serial.println("Sorry, Time Ran Out!");
      while (Serial.available() > 0)
        Serial.read();
      restart();
    }
    // Read serial input:
    while (Serial.available() > 0 && flag == 0) {
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
        else if (String(inString) == "") {
          invalid(" ");
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

/*Name: invalid
 * Inputs: string input
 * Outputs: void
 * Functions: Informs the user that the latest input was invalid
 */
void invalid(String input) {
  bcd.invalidInput();
  Serial.print("Warning You Entered Invalid Data: ");
  for (int i = 0; i < input.length(); i++) {
    if (isPrintable(input.charAt(i)))
      Serial.print((char)input.charAt(i));
  }
  Serial.println();
}

/*Name: valid
 * Inputs: String input
 * Outputs: void
 * Functions: Displays the hexidecimal value of the input on the serial monitor and bcd
 */
void valid(String input) {
  input.toUpperCase();
  Serial.print("Hexidecimal Representation Of Inputted Data: ");
  Serial.println(input);
}

/*Name: start
 * Inputs: void
 * Outputs: void
 * Functions: Displays the welcome message
 */
void start() {
  bcd.startUp();
  Serial.println();
  Serial.println("___________________________________________________");
  Serial.println();
  Serial.println("Please Enter An Integer In The Range OF [0 - 15]");
  Serial.println("The Value Will Be Displayed In Hexidecimal");
  Serial.println("The Value Being Displayed On The 7-Segment Display ");
  Serial.println("Is A Hexidecimal Value If The Period Is Lit");
  Serial.println("___________________________________________________");
  Serial.println();
  Serial.println();
}

/*Name: restart
 * Inputs: void
 * Outputs: char hex
 * Functions: Blinks the decimal for five seconds and clears the bcd
 * a decimal value
 */
void restart() {
  for (int i = 0; i < 5; i++) {
    bcd.decimal(1);
    delay(500);
    bcd.decimal(0);
    delay(500);
    wdt_reset();
  }
  wdt_reset();
  bcd.clear();
  return;
}

/*Name: ISR
 * Inputs: WDT_vect
 * Outputs: void
 * Functions: Interrupt service routine. Informs the user that the watchdog
 * Timer rand out
 */
ISR(WDT_vect){
  flag = 1;
}
