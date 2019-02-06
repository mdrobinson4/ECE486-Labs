
char currVal = 's';
int toggle = 0;
int ledPin = 4;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
    Serial.println("Enter 'g' to make LEDs blink || Enter 's' to turn the LEDs off");
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

char getInput() {
  char res = Serial.read(); // Read input from the serial monitor
  if (res == 's' || res == 'g') // update the currVal variable if either 's' or 'g' were entered
    currVal = res;  // update the currVal variable
}

// the loop function runs over and over again forever
void loop() {
  getInput(); // get the input from the serial monitor
  // Check to see if the currVal is set to 'g'
  if (currVal == 'g') {
    // if toggle is set to 0 turn led13 off and turn led4 on
    if (toggle == 0) {
      toggle = 1; // Update the toggle variable to specify that this case has occurred
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(ledPin, HIGH);
      delay(1000);
    }
    // if toggle is set to 1 turn led13 on and turn led4 off
    else if (toggle == 1) {
      toggle = 0; // Update the toggle variable to specify that this case has occurred
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(ledPin, LOW);
      delay(2000);
    }
  }
  // Check to see if the user inputted s into the serial monitor
  else if (currVal == 's') {
      digitalWrite(LED_BUILTIN, LOW); // turn led13 off
      digitalWrite(ledPin, LOW);  // turn led4 off
      toggle = 0; // Reset the toggle variable so that if 'g' is inputted the first case will occur first
  }
}
