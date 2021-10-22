/*
  NeoPixelLibary.h - Library for display time via WS2813 RGB LEDs.
  Created by Jens Zech, 2021-05-16.
*/
#ifndef NeoPixelLib_h
#define NeoPixelLib_h

#include <Adafruit_NeoPixel.h>

#include "Arduino.h"

class NeoPixelLib {
   public:
    NeoPixelLib(uint16_t numPixel, uint8_t pin);
    void setupNeoPixel();
    void loopPixelUpdate(bool isDarkMode);
    void setBrightness(uint8_t level);

   private:
    Adafruit_NeoPixel m_pixels;
    bool m_isDark;
    void showClockSegments();
    void showHourPixel(time_t time);
    void showMinutePixel(time_t time);
    void showSecondPixel(time_t time);
};

#endif
