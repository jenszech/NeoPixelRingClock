/*
  NtpTimeLib.h - Library to get time from NTP Server
  Created by Jens Zech, 2021-05-16.
*/

#ifndef NtpTimeLib_h
#define NtpTimeLib_h

#include <NTPClient.h>
#include <TimeLib.h>
#include <WiFiUdp.h>

#include "Arduino.h"

class NtpTimeLib {
   public:
    NtpTimeLib(uint16_t timeOffset, uint16_t resyncSeconds, uint16_t updateIntervall, WiFiUDP ntpUDP);
    void setupNTP();
    String getTimeStr();
    String getNtpRawTimeStr();
    void printSerialLog();
    time_t getNtpTime();
    
   private:
    NTPClient timeClient;
    uint16_t _timeOffset;
    uint16_t _resyncSeconds;
    uint16_t _updateIntervall;
    String printDigits(int digits);
};

#endif
