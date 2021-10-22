/*
  NtpTimeLib.h - Library to get time from NTP Server
  Created by Jens Zech, 2021-05-16.
*/
#include "NtpTimeLib.h"

#include <NTPClient.h>
#include <TimeLib.h>
#include <WiFiUdp.h>

#include "Arduino.h"

namespace {
    NTPClient* timeClientPtr = nullptr;
}

NtpTimeLib::NtpTimeLib(uint16_t timeOffset, uint16_t resyncSeconds, uint16_t updateInterval, WiFiUDP& ntpUDP)
    : timeClient(ntpUDP, timeOffset), m_resyncSeconds(resyncSeconds), m_updateIntervall(updateInterval){

    timeClientPtr = &this->timeClient;
}

void NtpTimeLib::setupNTP() {
    timeClient.setUpdateInterval(m_updateIntervall);
    timeClient.update();
    setSyncProvider(&NtpTimeLib::getNtpTime);
    setSyncInterval(m_resyncSeconds);  // just for demo purposes!
    printSerialLog();
}

String NtpTimeLib::getTimeStr() {
    String timeStr = "";
    time_t t = now();

    timeStr += printDigits(hour(t));
    timeStr += ":";
    timeStr += printDigits(minute(t));
    timeStr += ":";
    timeStr += printDigits(second(t));
    timeStr += " ";
    timeStr += printDigits(day(t));
    timeStr += ".";
    timeStr += printDigits(month(t));
    timeStr += ".";
    timeStr += printDigits(year(t));

    return timeStr;
}

String NtpTimeLib::getNtpRawTimeStr() {
    return timeClient.getFormattedTime();
}

void NtpTimeLib::printSerialLog() {
    Serial.print("NTP Time: ");
    Serial.println(timeClient.getFormattedTime());
    Serial.print("TimeLib: ");
    Serial.println(getTimeStr());
}

// resync Method - get Time from NTPClient
time_t NtpTimeLib::getNtpTime() {
    Serial.println("TimeLib resync");
    return timeClientPtr->getEpochTime();
}

//-- PRIVATE FUNCTIONS ------------------------------------------
String NtpTimeLib::printDigits(int digits) {
    // utility for digital clock display: prints leading 0
    if (digits < 10) return "0" + String(digits);
    return String(digits);
}