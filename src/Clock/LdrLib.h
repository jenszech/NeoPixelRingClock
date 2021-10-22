/*
  LdrLib.h - Library for reading ldr values
  Created by Jens Zech, 2021-05-16.
*/

#ifndef LdrLib_h
#define LdrLib_h

#include "Arduino.h"
#include <deque>

class LdrLib {
   public:
    LdrLib(uint8_t pin, uint16_t threshold);
    bool isDark();
    int getLdrValue();
    void printSerialLog();
    void setTreshold(uint16_t threshold);

   private:
    int getAnalogValue();
    uint8_t m_pin;
    uint16_t m_threshold;
    bool m_dark;
};

#endif
