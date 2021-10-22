/*
  LdrLib.h - Library for reading ldr values
  Created by Jens Zech, 2021-05-16.
*/

#include "LdrLib.h"
#include "Arduino.h"

#include <numeric>

static const int AVERAGE_VALUE_REPETITION_COUNT = 10;
static const int HYSTERESIS_DIFFERENCE = 5;

LdrLib::LdrLib(uint8_t pin, uint16_t threshold) 
    : m_pin(pin), m_threshold(threshold), m_dark(false) {
}

bool LdrLib::isDark() {
    int sensorValue = getAnalogValue();  // read analog input pin 0

    if(sensorValue < m_threshold) {
        m_dark = true;
    }
    if(sensorValue > (m_threshold + HYSTERESIS_DIFFERENCE)) {
        m_dark = false;
    }

    return m_dark;
}

int LdrLib::getLdrValue() {
    return getAnalogValue();
}

void LdrLib::setTreshold(uint16_t threshold) {
    m_threshold = threshold;
}

void LdrLib::printSerialLog() {
    int sensorValue = getAnalogValue();  // read analog input pin 0

    Serial.print("LDR: ");
    Serial.print(sensorValue, DEC);
    Serial.print(" => isDark: ");
    Serial.println(isDark());
}

int LdrLib::getAnalogValue() {
    uint16_t value = 0;
    for(int i=0; i < AVERAGE_VALUE_REPETITION_COUNT; ++i) {
        delayMicroseconds(1);
        value += analogRead(m_pin);
    }
    return value / 10;
}
