/*
  NeoPixelLibary.h - Library for display time via WS2813 RGB LEDs.
  Created by Jens Zech, 2021-05-16.
*/
#ifndef NeoPixelLib_h
#define NeoPixelLib_h

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class NeoPixelLib
{
  public:
    NeoPixelLib(uint16_t numPixel, uint8_t pin);
    void setupNeoPixel();
    void loopPixelUpdate(bool isDarkMode);
  private:
    Adafruit_NeoPixel pixels;
    bool _isDark;
    void showClockSegments();
    void showHourPixel(int hourNow);
    void showMinutePixel(int minuteNow);
    void showSecondPixel(int secondNow);
};

#endif

