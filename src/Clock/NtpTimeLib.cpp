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
    Timezone* tzConverterPtr = nullptr;

    //Definition of time change rules and its offsets from UTC
    const TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     // Central European Summer Time
    const TimeChangeRule CET  = {"CET ", Last, Sun, Oct, 3, 60};      // Central European Standard Time
}

NtpTimeLib::NtpTimeLib(uint16_t resyncSeconds, uint16_t updateInterval, WiFiUDP& ntpUDP)
    : m_timeClient(ntpUDP), m_tzConverter(CEST, CET), m_resyncSeconds(resyncSeconds), m_updateIntervall(updateInterval){

    timeClientPtr = &this->m_timeClient;
    tzConverterPtr = &this->m_tzConverter;
}

void NtpTimeLib::setupNTP() {
    m_timeClient.setUpdateInterval(m_updateIntervall);
    m_timeClient.update();
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
    return m_timeClient.getFormattedTime();
}

void NtpTimeLib::printSerialLog() {
    Serial.print("NTP Time: ");
    Serial.println(m_timeClient.getFormattedTime());
    Serial.print("TimeLib: ");
    Serial.println(getTimeStr());
}

// resync Method - get Time from NTPClient
time_t NtpTimeLib::getNtpTime() {
    Serial.println("TimeLib resync");
    return tzConverterPtr->toLocal(timeClientPtr->getEpochTime());
}

//-- PRIVATE FUNCTIONS ------------------------------------------
String NtpTimeLib::printDigits(int digits) {
    // utility for digital clock display: prints leading 0
    if (digits < 10) return "0" + String(digits);
    return String(digits);
}