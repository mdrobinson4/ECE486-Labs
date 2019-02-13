/*
  fourDigitDisplay.h - Library for displaying the numbers 0 - 15
  Using a seven-segment 4-digit display
  Author - Mark Robinson
*/
#include "Arduino.h"

#ifndef fourDigitDisplay_h
#define fourDigitDisplay_h


class fourDigitDisplay
{
  public:
    fourDigitDisplay();
    void display(int number);
  private:
    int _digit1;
    int _digit2;
    int _digit3;
    int _digit4;
    int _segA;
    int _segB;
    int _segC;
    int _segD;
    int _segE;
    int _segF;
    int _segG;
};

#endif
