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
        ((uint32_t)20 << 16) | ((uint32_t)20 << 8) | 20,  //color for clock segments
        ((uint32_t)50 << 16) | ((uint32_t)0 << 8) | 0,    //color for second marker
        ((uint32_t)0 << 16) | ((uint32_t)50 << 8) | 0,    //color for minute marker
        ((uint32_t)0 << 16) | ((uint32_t)25 << 8) | 0,    //color for hour marker
        ((uint32_t)0 << 16) | ((uint32_t)5 << 8) | 0      //color for hour marker Left/Right
    },
    {
        ((uint32_t)5 << 16) | ((uint32_t)5 << 8) | 5,  //color for clock segments
        ((uint32_t)0 << 16) | ((uint32_t)0 << 8) | 0,  //color for second marker
        ((uint32_t)0 << 16) | ((uint32_t)5 << 8) | 0,  //color for minute marker
        ((uint32_t)0 << 16) | ((uint32_t)5 << 8) | 0,  //color for hour marker
        ((uint32_t)0 << 16) | ((uint32_t)1 << 8) | 0   //color for hour marker Left/Right
    }};


NeoPixelLib::NeoPixelLib(uint16_t numPixel, uint8_t pin)
    : m_pixels(numPixel, pin, NEO_GRB + NEO_KHZ800) {
}

void NeoPixelLib::setupNeoPixel() {
    m_pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
}

void NeoPixelLib::setBrightness(uint8_t level) {
    m_pixels.setBrightness(level);
}

void NeoPixelLib::loopPixelUpdate(bool isDarkMode) { 
    m_isDark = isDarkMode;
    time_t t = now();
    m_pixels.clear();  // Set all pixel colors to 'off'

    // Setup static clock elements
    showClockSegments();

    // Now print all the elements of the time
    showHourPixel(t);
    showMinutePixel(t);
    showSecondPixel(t);
    m_pixels.show();  // Send the updated pixel colors to the hardware.
}

//-- PRIVATE FUNCTIONS ------------------------------------------
void NeoPixelLib::showClockSegments() {
    if (!m_isDark) {
        for (int b = 0; b < 12; b++) {
            m_pixels.setPixelColor(b * 5, COLORS[0][I_CLOCK]);
        }
    }
    for (int b = 0; b < 4; b++) {
        m_pixels.setPixelColor(b * 15, COLORS[m_isDark][I_CLOCK]);
    }
}

void NeoPixelLib::showHourPixel(time_t time) {
    int hourL = 0, hourR = 0;
    int hourNow = hourFormat12(time);
    int minuteNow = minute(time);

    hourNow = (hourNow * 5) + (hourNow / 12) + (minuteNow / 12);
    hourL = hourNow - 1;
    hourR = hourNow + 1;
    
    if (hourR == 60) {
        hourR = 0;
    }
    if (hourL == -1) {
        hourL = 59;
    }

    m_pixels.setPixelColor(hourL, COLORS[m_isDark][I_HLR]);
    m_pixels.setPixelColor(hourNow, COLORS[m_isDark][I_H]);
    m_pixels.setPixelColor(hourR, COLORS[m_isDark][I_HLR]);
}

void NeoPixelLib::showMinutePixel(time_t time) {
    m_pixels.setPixelColor(minute(time), COLORS[m_isDark][I_M]);
}

void NeoPixelLib::showSecondPixel(time_t time) {
    if (!m_isDark) {
        m_pixels.setPixelColor(second(time), COLORS[m_isDark][I_S]);
    }
}