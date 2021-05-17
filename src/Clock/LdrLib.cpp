/*
  LdrLib.h - Library for reading ldr values
  Created by Jens Zech, 2021-05-16.
*/

#include "LdrLib.h"

#include "Arduino.h"

LdrLib::LdrLib(uint8_t pin, uint16_t threshold) {
    _pin = pin;
    _threshold = threshold;
}

bool LdrLib::isDark() {
    int sensorValue = analogRead(_pin);  // read analog input pin 0
    return (sensorValue < _threshold);
}

int LdrLib::getLdrValue() {
    return analogRead(_pin);
}

void LdrLib::setTreshold(uint16_t threshold) {
    _threshold = threshold;
}

void LdrLib::printSerialLog() {
    int sensorValue = analogRead(_pin);  // read analog input pin 0

    Serial.print("LDR: ");
    Serial.print(sensorValue, DEC);
    Serial.print(" => isDark: ");
    Serial.println(isDark());
}