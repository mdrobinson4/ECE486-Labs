/*Author: Mark Robinson
 * CWID: 11635959
 * Lab: Lab 2
 * Date: February 13, 2019
 */

#include <fourDigitDisplay.h>

// Initialize 4-segment display object
fourDigitDisplay bcd;

void setup() {
  Serial.begin(9600);
  // Displays message explaining the operation of the counter and the output
  Serial.println("|| On The 4-Dgit 7-Segment Display. ||");
  Serial.println("|| The Decimal Value Appears On The First Digit. ||");
  Serial.println("|| The Hexidecimal Value appears On The 4th Digit. ||");
  Serial.println("|| _______________________________________________ ||");

  pinMode(LED_BUILTIN, OUTPUT); // Set on-board led to output 
  pinMode(12, OUTPUT);  // Set off-board led as output
  pinMode(2, INPUT);  // Set toggle switch as input
}

void loop() {
  int ledPin = 12; // off-board led
  const int buttonPin = 2;    // the number of the pushbutton pin
  
  int ledState = LOW;         // the current state of the output pin
  int buttonState = digitalRead(buttonPin);             // the current reading from the input pin
  int lastButtonState = (buttonState == HIGH) ? LOW : HIGH;   // the previous reading from the input pin
  int off = buttonState;
  char hex = ' ';
  int value = 0;
  
  bcd.display(0); // Display change state count value with 7-segment display
  // Write current decimal / hex values to serial
  Serial.print("Count = (Decimal) ");
  Serial.print(value);
  Serial.print(" (Hex) ");
  Serial.println(hex);

  while(1) {
    // read the state of the switch into a local variable:
    int reading = digitalRead(buttonPin); 
    // If the switch state has changed, update the button state 
    // and increment the state change variable
    if (reading != buttonState) {
      buttonState = reading;
      value += 1;
      if (value > 9) {  // Call function to get hexidecimal value if necessary
        hex = getHex(value);
      }
      else
        hex = ' ';  // Clear hexidecimal value
      bcd.display(value); // Display the value on the 4-segment display
      // Display the value in the Serial monitor
      Serial.print("Count = (Decimal) ");
      if (value < 10)
        Serial.print(value);
      Serial.print(" (Hex) ");
      Serial.println(hex);
      hex = ' ';
    }
    lastButtonState = reading;  // Set the last button state to the current valuee
    if (buttonState != off) // If the switch is positioned "ON" blink the LEDs
      blink(reading, buttonState);
    else
      delay(15);
    // Reset the state change variable if it has reached 15
    if (value == 15) 
      value = 0;
  }
}

/*Name: getHex
 * Inputs: int value
 * Outputs: char hex
 * Functions: Returns the corresponding hexidecimal value given
 * a decimal value
 */
char getHex(int value) {
  if (value == 10)
    return 'A';
  if (value == 11)
    return 'B';
  if (value == 12)
    return 'C';
  if (value == 13)
    return 'D';
  if (value == 14)
    return 'E';
  if (value == 15)
    return 'F';
}

/*Name: blink
 * Inputs: int reading, int buttonState
 * Outputs: void
 * Functions: Blinks the on-board and off-board LEDs out of phase ( 1 second delay)
 * The delays are broken up into 250 milliseconds. This is done as an easy way
 * To increase the resolution
 */
void blink(int reading, int buttonState) {
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(12, HIGH);
  delay(250);  // Turn on-board led on for 1 seconds
  if (reading != buttonState)
    return;
  delay(250); 
  if (reading != buttonState)
    return;
  delay(250);
  if (reading != buttonState)
    return;
  delay(250);
  if (reading != buttonState)
    return;
    
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(12, LOW);
    delay(250);  // Turn on-board led on for 1 seconds
  if (reading != buttonState)
    return;
  delay(250);
  if (reading != buttonState)
    return;
    delay(250);  
  if (reading != buttonState)
    return;
  delay(250);
  if (reading != buttonState)
    return;
}

/*Name: turnOff
 * Inputs: None
 * Outputs: void
 * Functions: Turns off both the on-board and off-board LEDs
 */
void turnOff() {
  digitalWrite(LED_BUILTIN, LOW); // turn on-board LED off
  digitalWrite(12, LOW);  // turn off-board off
}
