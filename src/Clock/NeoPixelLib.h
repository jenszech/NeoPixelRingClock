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
    void loopPixelUpdate(int isDarkMode);
  private:
    Adafruit_NeoPixel pixels;
    void clockSegments(int isDarkMode);
};

#endif

