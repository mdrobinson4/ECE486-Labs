#include <fourDigitDisplay.h>

fourDigitDisplay bcd;

void setup() {
  Serial.begin(9600);
  Serial.println("|| On The 4-Dgit 7-Segment Display. ||");
  Serial.println("|| The Decimal Value Appears On The First Digit. ||");
  Serial.println("|| The Hexidecimal Value appears On The 4th Digit. ||");
  Serial.println("|| _______________________________________________ ||");
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(2, INPUT);
  pinMode(12, OUTPUT);
}

void loop() {
  int ledPin = 12; // off-board led
  // constants won't change. They're used here to set pin numbers:
  const int buttonPin = 2;    // the number of the pushbutton pin
  
  // Variables will change:
  int ledState = HIGH;         // the current state of the output pin
  int buttonState;             // the current reading from the input pin
  int lastButtonState = LOW;   // the previous reading from the input pin
  char hex = '0';
  // the following variables are unsigned longs because the time, measured in
  // milliseconds, will quickly become a bigger number than can be stored in an int.
  unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
  unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers
  int value = 0;

  while(1) {
    // read the state of the switch into a local variable:
    int reading = digitalRead(buttonPin);
 
     if (reading != buttonState) {
      buttonState = reading;
      value += 1;
      if (value > 9) {
        hex = getHex(value);
      }
        
      else
        hex = ' ';

      if (buttonState == 0 && value == 1)
        value = 0;
      bcd.display(value);
      Serial.print("Count = (Decimal) ");
      if (value < 10)
        Serial.print(value);
      Serial.print(" (Hex) ");
      Serial.println(hex);
      hex = ' ';
    }
    lastButtonState = reading;
    if (buttonState == HIGH)
      blink(reading, buttonState);
    else
      delay(15);
      
    if (value == 15)
      value = 0;
  }
}

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

void blink(int reading, int buttonState) {
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(12, HIGH);
  delay(250);  // Turn on-board led on for 2 seconds
  if (reading != buttonState)
    return;
  delay(250);  // Turn on-board led on for 2 seconds
  if (reading != buttonState)
    return;
  delay(250);  // Turn on-board led on for 2 seconds
  if (reading != buttonState)
    return;
  delay(250);  // Turn on-board led on for 2 seconds
  if (reading != buttonState)
    return;
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(12, LOW);
    delay(500);  // Turn on-board led on for 2 seconds
  if (reading != buttonState)
    return;
  delay(500);  // Turn on-board led on for 2 seconds
  if (reading != buttonState)
    return;
}

void turnOff() {
  digitalWrite(LED_BUILTIN, LOW); // turn led13 off
  digitalWrite(12, LOW);  // turn led4 off
}
