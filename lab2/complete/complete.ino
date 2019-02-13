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
  const int buttonPin = 2;    // the number of the pushbutton pin
  
  int ledState = LOW;         // the current state of the output pin
  int buttonState = digitalRead(buttonPin);             // the current reading from the input pin
  int lastButtonState = (buttonState == HIGH) ? LOW : HIGH;   // the previous reading from the input pin
  int off = buttonState;
  char hex = ' ';
  int value = 0;
  
  Serial.println(buttonState);
  bcd.display(0);
  Serial.print("Count = (Decimal) ");
  Serial.print(value);
  Serial.print(" (Hex) ");
  Serial.println(hex);

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
      bcd.display(value);
      Serial.print("Count = (Decimal) ");
      if (value < 10)
        Serial.print(value);
      Serial.print(" (Hex) ");
      Serial.println(hex);
      hex = ' ';
    }
    lastButtonState = reading;
    if (buttonState != off)
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
