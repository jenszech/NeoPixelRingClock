/*
  NeoPixelLibary.cpp - Library for display time via WS2813 RGB LEDs.
  Created by Jens Zech, 2021-05-16.
*/

#include "NeoPixelLib.h"

#include <Adafruit_NeoPixel.h>
#include <TimeLib.h>

#include "Arduino.h"

const int I_CLOCK = 0;
const int I_S = 1;
const int I_M = 2;
const int I_H = 3;
const int I_HLR = 4;

const uint32_t COLORS[2][5] = {
    {
        ((uint32_t)20 << 16) | ((uint32_t)20 << 8) | 20,  //color for clock segemnts
        ((uint32_t)0 << 16) | ((uint32_t)10 << 8) | 0,    //color for second marker
        ((uint32_t)0 << 16) | ((uint32_t)50 << 8) | 0,    //color for minute marker
        ((uint32_t)0 << 16) | ((uint32_t)25 << 8) | 0,    //color for hour marker
        ((uint32_t)0 << 16) | ((uint32_t)5 << 8) | 0      //color for hour marker Left/Right
    },
    {
        ((uint32_t)5 << 16) | ((uint32_t)5 << 8) | 5,  //color for clock segemnts
        ((uint32_t)0 << 16) | ((uint32_t)0 << 8) | 0,  //color for second marker
        ((uint32_t)0 << 16) | ((uint32_t)5 << 8) | 0,  //color for minute marker
        ((uint32_t)0 << 16) | ((uint32_t)5 << 8) | 0,  //color for hour marker
        ((uint32_t)0 << 16) | ((uint32_t)1 << 8) | 0   //color for hour marker Left/Right
    }};

NeoPixelLib::NeoPixelLib(uint16_t numPixel, uint8_t pin)
    : pixels(numPixel, pin, NEO_GRB + NEO_KHZ800) {
}

void NeoPixelLib::setupNeoPixel() {
    pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
}

void NeoPixelLib::loopPixelUpdate(bool isDarkMode) {
    _isDark = isDarkMode;
    time_t t = now();
    pixels.clear();  // Set all pixel colors to 'off'

    // Setup static clock elements
    showClockSegments();

    // Now print all the elements of the time
    showHourPixel(hour(t));
    showMinutePixel(minute(t));
    showSecondPixel(second(t));
    pixels.show();  // Send the updated pixel colors to the hardware.
}

//-- PRIVATE FUNCTIONS ------------------------------------------
void NeoPixelLib::showClockSegments() {
    if (!_isDark) {
        for (int b = 0; b < 12; b++) {
            pixels.setPixelColor(b * 5, COLORS[0][I_CLOCK]);
        }
    }
    for (int b = 0; b < 4; b++) {
        pixels.setPixelColor(b * 15, COLORS[_isDark][I_CLOCK]);
    }
}

void NeoPixelLib::showHourPixel(int hourNow) {
    int hourL, hourR;
    while (hourNow > 11) {
        hourNow = hourNow - 12;
    }
    hourNow = (hourNow * 5) + (hourNow / 12);
    hourL = hourNow - 1;
    hourR = hourNow + 1;
    if (hourR == 60)
        hourR = 0;
    if (hourL == -1)
        hourL = 59;

    pixels.setPixelColor(hourL, COLORS[_isDark][I_HLR]);
    pixels.setPixelColor(hourNow, COLORS[_isDark][I_H]);
    pixels.setPixelColor(hourR, COLORS[_isDark][I_HLR]);
}

void NeoPixelLib::showMinutePixel(int minuteNow) {
    pixels.setPixelColor(minuteNow, COLORS[_isDark][I_M]);
}

void NeoPixelLib::showSecondPixel(int secondNow) {
    if (!_isDark) {
        pixels.setPixelColor(secondNow, COLORS[_isDark][I_S]);
    }
}