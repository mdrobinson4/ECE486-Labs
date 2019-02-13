/*
  fourDigitDisplay.cpp - Library for displaying the numbers 0 - 15
  Using a seven-segment 4-digit display
  Author - Mark Robinson
*/

#include "Arduino.h"
#include "fourDigitDisplay.h"

fourDigitDisplay::fourDigitDisplay() {
  _digit1 = 11; //PWM Display pin 1
  _digit2 = 10; //PWM Display pin 2
  _digit3 = 9; //PWM Display pin 6
  _digit4 = 6; //PWM Display pin 8

  //Pin mapping from Arduino to the ATmega DIP28 if you need it
  //http://www.arduino.cc/en/Hacking/PinMapping
  _segA = A1; //Display pin 14
  _segB = 3; //Display pin 16
  _segC = 4; //Display pin 13
  _segD = 5; //Display pin 3
  _segE = A0; //Display pin 5
  _segF = 7; //Display pin 11
  _segG = 8; //Display pin 15

  pinMode(_segA, OUTPUT);
  pinMode(_segB, OUTPUT);
  pinMode(_segC, OUTPUT);
  pinMode(_segD, OUTPUT);
  pinMode(_segE, OUTPUT);
  pinMode(_segF, OUTPUT);
  pinMode(_segG, OUTPUT);

  pinMode(_digit1, OUTPUT);
  pinMode(_digit2, OUTPUT);
  pinMode(_digit3, OUTPUT);
  pinMode(_digit4, OUTPUT);

  pinMode(13, OUTPUT);
  digitalWrite(_digit1, 1);
  return;
}

void fourDigitDisplay::display(int number) {
  if (number < 10) {
    digitalWrite(_digit4, 0);
    digitalWrite(_digit1, 1);
  }
  else if (number >= 10) {
    digitalWrite(_digit1, 0);
    digitalWrite(_digit4, 1);
  }
  switch (number){
  case 0:
    digitalWrite(_segA, LOW);
    digitalWrite(_segB, LOW);
    digitalWrite(_segC, LOW);
    digitalWrite(_segD, LOW);
    digitalWrite(_segE, LOW);
    digitalWrite(_segF, LOW);
    digitalWrite(_segG, HIGH);
    break;

  case 1:
    digitalWrite(_segA, HIGH);
    digitalWrite(_segB, LOW);
    digitalWrite(_segC, LOW);
    digitalWrite(_segD, HIGH);
    digitalWrite(_segE, HIGH);
    digitalWrite(_segF, HIGH);
    digitalWrite(_segG, HIGH);
    break;

  case 2:
    digitalWrite(_segA, LOW);
    digitalWrite(_segB, LOW);
    digitalWrite(_segC, HIGH);
    digitalWrite(_segD, LOW);
    digitalWrite(_segE, LOW);
    digitalWrite(_segF, HIGH);
    digitalWrite(_segG, LOW);
    break;

  case 3:
    digitalWrite(_segA, LOW);
    digitalWrite(_segB, LOW);
    digitalWrite(_segC, LOW);
    digitalWrite(_segD, LOW);
    digitalWrite(_segE, HIGH);
    digitalWrite(_segF, HIGH);
    digitalWrite(_segG, LOW);
    break;

  case 4:
    digitalWrite(_segA, HIGH);
    digitalWrite(_segB, LOW);
    digitalWrite(_segC, LOW);
    digitalWrite(_segD, HIGH);
    digitalWrite(_segE, HIGH);
    digitalWrite(_segF, LOW);
    digitalWrite(_segG, LOW);
    break;

  case 5:
    digitalWrite(_segA, LOW);
    digitalWrite(_segB, HIGH);
    digitalWrite(_segC, LOW);
    digitalWrite(_segD, LOW);
    digitalWrite(_segE, HIGH);
    digitalWrite(_segF, LOW);
    digitalWrite(_segG, LOW);
    break;

  case 6:
    digitalWrite(_segA, LOW);
    digitalWrite(_segB, HIGH);
    digitalWrite(_segC, LOW);
    digitalWrite(_segD, LOW);
    digitalWrite(_segE, LOW);
    digitalWrite(_segF, LOW);
    digitalWrite(_segG, LOW);
    break;

  case 7:
    digitalWrite(_segA, LOW);
    digitalWrite(_segB, LOW);
    digitalWrite(_segC, LOW);
    digitalWrite(_segD, HIGH);
    digitalWrite(_segE, HIGH);
    digitalWrite(_segF, HIGH);
    digitalWrite(_segG, HIGH);
    break;

  case 8:
    digitalWrite(_segA, LOW);
    digitalWrite(_segB, LOW);
    digitalWrite(_segC, LOW);
    digitalWrite(_segD, LOW);
    digitalWrite(_segE, LOW);
    digitalWrite(_segF, LOW);
    digitalWrite(_segG, LOW);
    break;

  case 9:
    digitalWrite(_segA, LOW);
    digitalWrite(_segB, LOW);
    digitalWrite(_segC, LOW);
    digitalWrite(_segD, LOW);
    digitalWrite(_segE, HIGH);
    digitalWrite(_segF, LOW);
    digitalWrite(_segG, LOW);
    break;

  case 10:
    digitalWrite(_segA, LOW);
    digitalWrite(_segB, LOW);
    digitalWrite(_segC, LOW);
    digitalWrite(_segD, HIGH);
    digitalWrite(_segE, LOW);
    digitalWrite(_segF, LOW);
    digitalWrite(_segG, LOW);
    break;

  case 11:
    digitalWrite(_segA, LOW);
    digitalWrite(_segB, LOW);
    digitalWrite(_segC, LOW);
    digitalWrite(_segD, LOW);
    digitalWrite(_segE, LOW);
    digitalWrite(_segF, LOW);
    digitalWrite(_segG, LOW);
    break;

  case 12:
    digitalWrite(_segA, LOW);
    digitalWrite(_segB, HIGH);
    digitalWrite(_segC, HIGH);
    digitalWrite(_segD, LOW);
    digitalWrite(_segE, LOW);
    digitalWrite(_segF, LOW);
    digitalWrite(_segG, HIGH);
    break;

  case 13:
    digitalWrite(_segA, LOW);
    digitalWrite(_segB, LOW);
    digitalWrite(_segC, LOW);
    digitalWrite(_segD, LOW);
    digitalWrite(_segE, LOW);
    digitalWrite(_segF, LOW);
    digitalWrite(_segG, HIGH);
    break;

  case 14:
    digitalWrite(_segA, LOW);
    digitalWrite(_segB, HIGH);
    digitalWrite(_segC, HIGH);
    digitalWrite(_segD, LOW);
    digitalWrite(_segE, LOW);
    digitalWrite(_segF, LOW);
    digitalWrite(_segG, LOW);
    break;

  case 15:
    digitalWrite(_segA, LOW);
    digitalWrite(_segB, HIGH);
    digitalWrite(_segC, HIGH);
    digitalWrite(_segD, HIGH);
    digitalWrite(_segE, LOW);
    digitalWrite(_segF, LOW);
    digitalWrite(_segG, LOW);
    break;
  }
  return;
}
