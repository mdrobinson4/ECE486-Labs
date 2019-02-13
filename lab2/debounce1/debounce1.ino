#include <fourDigitDisplay.h>

fourDigitDisplay bcd;

void setup() {
  Serial.begin(9600);
  Serial.println("On The 4-Dgit 7-Segment Display.");
  Serial.println("The Decimal Value Appears On The First Digit.");
  Serial.println("The Hexidecimal Value appears On The 4th Digit.");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(2, INPUT);
  pinMode(12, OUTPUT);

  // set initial LED state
  //digitalWrite(12, HIGH); 
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
    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:
    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState) {
      // reset the debouncing timer
      lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
      // whatever the reading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state:
  
      // if the button state has changed:
      if (reading != buttonState) {
        buttonState = reading;
        // only toggle the LED if the new button state is HIGH
        //ledState = !ledState;
        Serial.println(buttonState);
        value += 1;
        if (value > 15)
          value = 1;
        if (value > 9)
          hex = getHex(value);
        else
          hex = ' ';
        bcd.display(value);
        Serial.print("Count = (Decimal)");
        Serial.print(value);
        Serial.print(" (Hex)");
        Serial.println(hex);
        hex = ' ';
      }
    }
    // set the LED:
    //digitalWrite(ledPin, ledState);
    //if (ledState == 1)
      //blink();
    //else
      //turnOff();
    // save the reading. Next time through the loop, it'll be the lastButtonState:
    lastButtonState = reading;
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

void blink() {
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(12, HIGH);
  delay(1000);  // Turn on-board led on for 2 seconds
  
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(12, LOW);
  delay(2000); //Turn off-board led on for 1 second
}

void turnOff() {
  digitalWrite(LED_BUILTIN, LOW); // turn led13 off
  digitalWrite(12, LOW);  // turn led4 off
}
