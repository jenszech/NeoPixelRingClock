/*
  Project: WS2812 Ring LED clock with NTP server using ESP32
  Board: Wemos D1 Mini 
  Author: jzech@gmx.de
 */

#include <arduino.h>

// My secret stuff (eg WiFi password)
#include "config.h"

// How we connect to your local wifi
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

// UDP library which is how we communicate with Time Server
#include <ArduinoOTA.h>
#include <TimeLib.h>
#include <WiFiUdp.h>

#include "LdrLib.h"
#include "NtpTimeLib.h"
#include "Webserver.h"

#include "Clock.h"

WiFiUDP ntpUDP;

//-----------------------------------------------------------------------------
// MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN
//-----------------------------------------------------------------------------

Webserver web;
Clock neoClock(_PIXEL_NUM, _PIXEL_PIN);
LdrLib ldrSensor(_LDR_PIN,  _LDR_TRESHOLD);
NtpTimeLib ntpTime(_NTP_RSYNC, _NTP_UPDATE_INTERVAL, ntpUDP);

void setup() {
    Serial.begin(115200);
    connectToWifi();
    setupOTA();
    ntpTime.setupNTP();
    neoClock.initialize();
    web.initialize();
}

void loop() {
    neoClock.updateLoop();
    web.loopWebServer();
    loopOTA();
}


//-----------------------------------------------------------------------------
// WIFI Methods
//-----------------------------------------------------------------------------

// Establish a WiFi connection with your router
void connectToWifi() {
    Serial.print("\nConnecting to: ");
    Serial.println(_mySsid);
    WiFi.begin(_mySsid, _myPassword);

    // Try to connect 4 times a second for X seconds before timing out
    int timeout = _wifiTimeOutSeconds * 4;
    while (WiFi.status() != WL_CONNECTED && (timeout-- > 0)) {
        delay(250);
        Serial.print(".");
    }

    // Successful connection?
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nFailed to connect, exiting");
        // Set some LED failure here, for example
        delay(1000);
        return;
    }

    Serial.print("\nWiFi connected with (local) IP address of: ");
    Serial.println(WiFi.localIP());
}

//-----------------------------------------------------------------------------
// OTA Methods
//-----------------------------------------------------------------------------
void setupOTA() {
    Serial.printf("Sketch size: %u\n", ESP.getSketchSize());
    Serial.printf("Free size: %u\n", ESP.getFreeSketchSpace());
    ArduinoOTA.setHostname(_myMDNSHostName);
    ArduinoOTA.begin();
}

void loopOTA() {
    ArduinoOTA.handle();
}
