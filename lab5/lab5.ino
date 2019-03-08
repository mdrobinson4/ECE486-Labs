/*Author: Mark Robinson
   CWID: 11635959
   Lab: Lab 5
   Date: March 7, 2019
*/

#define potPin 0;

#include <avr/wdt.h>

/*Name: watchdogSetup
   Inputs: void
   Outputs: void
   Functions: Sets the watchdog timer to 4 seconds
*/
void watchdogSetup(void) {
  cli();
  wdt_reset();

  // Enter Watchdog Configuration mode:
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  // Set Watchdog settings:
  WDTCSR = (1 << WDIE) | (1 << WDE) |
           (1 << WDP3) | (0 << WDP2) | (0 << WDP1) |
           (0 << WDP0);
  sei();
}

/*Name: adc_int
   Inputs: void
   Outputs: void
   Functions: Initializes the analog to digital converter
*/
void adc_init() {
  // AREF = AVcc
  ADMUX = (1 << REFS0);

  // ADC Enable and prescaler of 128
  // 16000000/128 = 125000
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

/*Name: adc_read
   Inputs: uint8_t
   Outputs: uint16_t
   Functions: Returns the ADC value using polling
*/
uint16_t adc_read(uint8_t pin) {
    ADMUX = (ADMUX)|pin; // clears the bottom 3 bits before ORing
    while(ADCSRA & (1<<ADSC))
    wdt_reset();
    //value = (ADC);
}

/*Name: startMessage
   Inputs: char method
   Outputs: void
   Functions: Displays the ADC method
*/
void startMessage(char method) {
  if (method == 'a')
    Serial.println("Starting a set of conversions using analogRead()");
  else if (method == 'b')
    Serial.println("Starting a set of conversions using polling and port manipulation");
}


void setup() {
  Serial.begin(9600);
  Serial.println("Board Was Reset");
  reset();
  watchdogSetup();  // Configures watchdog timer
  adc_init();
}

void loop() {
  String inString = "";
  float polling[] = {0, 0}; // [ pollingCount, pollingTime ]
  float analog[] = {0, 0};  // [ analogReadCount, analogReadTime]

  while (1) {
    while (Serial.available() > 0) {
      wdt_reset();
      char inChar = Serial.read();
      if (inChar != '\n') {
        inString += (char)inChar;
      }
      // if you get a newline, print the string, then the string's value:
      else if (inChar == '\n') {
        if (inString.length() == 1 && (inString[0] == 'a' || inString[0] == 'b')) {
          convert(inString[0], polling, analog);
        }
        else {
          Serial.println("Error: invalid user input - the only valid user input is 'c' or 'b')");
          Serial.println();
        }
        inString = "";
      }
    }
  }
}

/*Name: reset
   Inputs: void
   Outputs: void
   Functions: Displays the welcome message
*/
void reset() {
  Serial.println("Select a type of conversion to perform ('a' for lab #4, 'b' for lab #5)");
  Serial.println();
  wdt_reset();
  return;
}

/*Name: convert
   Inputs: void
   Outputs: void
   Functions: Reads the analog value at the potentiometer,
   converts it to a digital value, and displays the values
   in hex and the time it took to convert
*/
void convert(char method, float polling[], float analog[]) {
  startMessage(method);
  float tempTime[30];
  int value = 0;
  char hex[50] = {0};
  for (int i = 0; i < 30; i++) {
    wdt_reset();
    if (method == 'a') {
      tempTime[i] = micros();
      value = analogRead(0);
      tempTime[i] = micros() - tempTime[i];
    }
    else if (method == 'b') {
      int ch = 0;
      ch &= 0b00000111;  // AND operation with 7
      ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
     
      // start single convertion
      // write ’1′ to ADSC
      ADCSRA |= (1<<ADSC);
     
      // wait for conversion to complete
      // ADSC becomes ’0′ again
      // till then, run loop continuously
      tempTime[i] = micros();
      while(ADCSRA & (1<<ADSC));
      tempTime[i] = micros() - tempTime[i];
      value = ADC;
    }
    Serial.print("#");
    Serial.print(i + 1);
    Serial.print(":   digital value = ");
    sprintf(hex, "%03X", value);
    Serial.print(hex);
    Serial.print("    Time = ");
    Serial.print((int)tempTime[i]);
    Serial.println(" usecs");
    Serial.println();
    delay(500);
  }
  float avg = 0.00;
  for (int i = 0; i < 30; i++) {
    wdt_reset();
    avg += tempTime[i];
  }
  avg = (float) avg / 30.00;
  Serial.print("avg conversion time = ");
  Serial.print((float)avg, 2);
  Serial.println(" usecs");
  Serial.println();
  while (Serial.available() > 0) {
    Serial.read();
    wdt_reset();
  }
  wdt_reset();
  reset();
  if (method == 'a') {
    analog[0] += 1.00;
    analog[1] += avg;
  }
  else if (method == 'b') {
    polling[0] += 1.00;
    polling[1] += avg;
  }

  Serial.println("TOTALS");
  Serial.print("analogRead() Average Conversion Time = ");
  if (analog[0] == 0)
    Serial.println("NA (0 total conversions)");
  else {
    Serial.print((float)analog[1] / (float)analog[0]);
    Serial.print(" usecs (");
    Serial.print((int)analog[0]);
    Serial.println(" total conversions)");
  }

  Serial.print("Polling Average Conversion Time = ");
  if (polling[0] == 0)
    Serial.println("NA (0 total conversions)");
  else {
    Serial.print((float)polling[1] / (float)polling[0]);
    Serial.print(" usecs (");
    Serial.print((int)polling[0]);
    Serial.println(" total conversions)");
  }
  wdt_reset();
}
