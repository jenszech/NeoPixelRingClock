
/*
  NeoPixelLibary.cpp - Library for display time via WS2813 RGB LEDs.
  Created by Jens Zech, 2021-05-16.
*/

#include "Arduino.h"
#include "NeoPixelLib.h"
#include <Adafruit_NeoPixel.h>
#include <TimeLib.h>


NeoPixelLib::NeoPixelLib(uint16_t numPixel, uint8_t pin) 
  : pixels(numPixel, pin, NEO_GRB + NEO_KHZ800) {
}

void NeoPixelLib::setupNeoPixel() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void NeoPixelLib::loopPixelUpdate(int isDarkMode) {
  uint32_t cSecond = pixels.Color(0,  10, 0);
  uint32_t cMinute = pixels.Color(0,  50, 0);
  uint32_t cHour   = pixels.Color(0,  25, 0);
  uint32_t cHourLR = pixels.Color(0,  5 , 0);
  int hourNow, hourL, hourR;

  if (isDarkMode) {
    cMinute = pixels.Color(0,  5, 0);
    cHour   = pixels.Color(0,  5, 0);
    cHourLR = pixels.Color(0,  1 , 0);
  }
 
  time_t t=now(); 
  pixels.clear(); // Set all pixel colors to 'off'
  clockSegments(isDarkMode);

  // Now print all the elements of the time 
  hourNow = hour(t);
  while (hourNow > 11) {
      hourNow = hourNow - 12;
  }
  hourNow = (hourNow * 5) + (hourNow / 12);
  hourL=hourNow-1;
  hourR=hourNow+1;
  if (hourR == 60) hourR = 0;
  if (hourL == -1) hourL = 59;

  pixels.setPixelColor(hourL, cHourLR);
  pixels.setPixelColor(hourNow, cHour);
  pixels.setPixelColor(hourR, cHourLR);
  pixels.setPixelColor(minute(t), cMinute);
  if (!isDarkMode) pixels.setPixelColor(second(t), cSecond);
  pixels.show();   // Send the updated pixel colors to the hardware.
}

void NeoPixelLib::clockSegments(int isDarkMode) {
  uint32_t cWhiteDim = pixels.Color(5, 5, 5);
  uint32_t cWhite = pixels.Color(20, 20, 20);

  if (isDarkMode) {
    for(int b=0; b<4; b++) { 
      pixels.setPixelColor(b*15, cWhiteDim);
    }
  } else {
    for(int b=0; b<12; b++) { 
      pixels.setPixelColor(b*5, cWhiteDim);
    }
    for(int b=0; b<4; b++) { 
      pixels.setPixelColor(b*15, cWhite);
    }
    
  }
}