/*
  LdrLib.h - Library for reading ldr values
  Created by Jens Zech, 2021-05-16.
*/

#ifndef LdrLib_h
#define LdrLib_h

#include "Arduino.h"

class LdrLib {
   public:
    LdrLib(uint8_t pin, uint16_t threshold);
    bool isDark();
    int getLdrValue();
    void printSerialLog();
    void setTreshold(uint16_t threshold);

   private:
    uint8_t _pin;
    uint16_t _threshold;
};

#endif
