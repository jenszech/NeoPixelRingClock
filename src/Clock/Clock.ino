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
#include <NTPClient.h>
#include <TimeLib.h>
#include <WiFiUdp.h>

// Include WebServer Lib
#include <ESP8266WebServer.h>
#include <FS.h>  // muss vor <detail\RequestHandlersImpl.h> stehen (s. Hinweis im Anschluss)
#include <EspHtmlTemplateProcessor.h>

#include "NeoPixelLib.h"

WiFiUDP ntpUDP;
ESP8266WebServer webserver(80);
EspHtmlTemplateProcessor templateProcessor(&webserver);
NTPClient timeClient(ntpUDP);

// forward declarations
void serialTimeLog();
void setupNTP();
void setupNeoPixel();
void displayBasicClock();
time_t updateTimeByNTP();

//-----------------------------------------------------------------------------
// MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN
//-----------------------------------------------------------------------------

NeoPixelLib neoPixel(NUM_PIXEL, PIN_PIXEL);

void setup() {
    Serial.begin(115200);
    connectToWifi();
    setupOTA();
    setupNTP();
    neoPixel.setupNeoPixel();
    setupWebServer();
}

long currentMillis = 0;
long previousMillis = 0;
int isDarkMode = false;

void loop() {
    currentMillis = millis();
    isDarkMode = isDark();

    //Update NeoPixel Display
    neoPixel.loopPixelUpdate(isDarkMode);

    //Debug Output every 60sec.
    if (currentMillis - previousMillis > 60000) {
        previousMillis = millis();
        serialTimeLog();
        serialLdrLog();
    }

    loopWebServer();
    loopOTA();
    delay(_PIXEL_UPDATE_INTERVAL);  // Pause before next pass through loop
}

//-----------------------------------------------------------------------------
// NTP Methods
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

void setupNTP() {
    Serial.print("\nNTP startet with Offset: ");
    Serial.println(_ntpTimeOffset);
    timeClient.begin();
    timeClient.setTimeOffset(_ntpTimeOffset);
    timeClient.setUpdateInterval(_ntpUpdateIntervall);
    timeClient.update();
    while (!timeClient.update()) {
        timeClient.forceUpdate();
    }
    setSyncProvider(updateTimeByNTP);
    setSyncInterval(_resyncSeconds);  // just for demo purposes!
    Serial.print("NTP Time: ");
    Serial.println(timeClient.getFormattedTime());
    Serial.print("TimeLib: ");
    serialTimeLog();
    Serial.println(getTimeStr());
}

//-----------------------------------------------------------------------------
// TimeLib Functions
//-----------------------------------------------------------------------------

// resync Method - get Time from NTPClient
time_t updateTimeByNTP() {
    Serial.println("TimeLib resync");
    return timeClient.getEpochTime();
}

String getTimeStr() {
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

void serialTimeLog() {
    Serial.println(getTimeStr());
}

String printDigits(int digits) {
    // utility for digital clock display: prints leading 0
    if (digits < 10) return "0" + String(digits);
    return String(digits);
}

//-----------------------------------------------------------------------------
// LDR Methods
//-----------------------------------------------------------------------------

bool isDark() {
    int sensorValue = analogRead(_LDR_PIN);  // read analog input pin 0
    return (sensorValue < _LDR_TRESHHOLD);
}

int getLdrValue() {
    return analogRead(_LDR_PIN);
}

void serialLdrLog() {
    int sensorValue = analogRead(_LDR_PIN);  // read analog input pin 0

    Serial.print("LDR: ");
    Serial.print(sensorValue, DEC);
    Serial.print(" => isDark: ");
    Serial.println(isDark());
}

//-----------------------------------------------------------------------------
// WebServer Methods
//-----------------------------------------------------------------------------
// Create a new web server
void setupWebServer() {
    // Initialize file system.
    if (!SPIFFS.begin()) {
        Serial.println("SPIFFS nicht initialisiert!");
        while (1) {
            yield();
        }
    }
    Serial.println("SPIFFS ok");

    // Start Web Server
    webserver.on("/", handleRootIndex);
    webserver.on("/config.html", handleRootConfig);
    //webserver.serveStatic("/", SPIFFS, "/index.html");
    webserver.serveStatic("/config.html", SPIFFS, "/config.html");
    webserver.onNotFound(notfoundPage);
    webserver.begin();
}

// Handle 404
void notfoundPage() {
    Serial.println("GET not found");
    webserver.send(404, "text/plain", "404: Not found");
}

// Listen for HTTP requests
void loopWebServer(void) {
    webserver.handleClient();
}

void handleRootIndex() {
    templateProcessor.processAndSend("/index.html", indexKeyProcessor);
}
void handleRootConfig() {
    templateProcessor.processAndSend("/config.html", indexKeyProcessor);
}

String indexKeyProcessor(const String& var) {
    Serial.println(var);
    if (var == "CLOCK") {
        return getTimeStr();
    } else if (var == "SSID") {
        return WiFi.SSID();
    } else if (var == "WLAN") {
        return "online";
    } else if (var == "NTPTIME") {
        return timeClient.getFormattedTime();
    } else if (var == "NTPOFFSET") {
        return String(_ntpTimeOffset);
    } else if (var == "NTPUPDATE") {
        return String(_ntpUpdateIntervall);
    } else if (var == "LIBTIME") {
        return getTimeStr();
    } else if (var == "LDRVALUE") {
        return String(getLdrValue());
    } else if (var == "LDRISDARK") {
        return String(isDark());
    } else if (var == "LDRTRESHHOLD") {
        return String(_LDR_TRESHHOLD);
    }
}

//-----------------------------------------------------------------------------
// OTA Methods
//-----------------------------------------------------------------------------
void setupOTA() {
    Serial.printf("Sketch size: %u\n", ESP.getSketchSize());
    Serial.printf("Free size: %u\n", ESP.getFreeSketchSpace());
    ArduinoOTA.setHostname("myPixelClock");
    ArduinoOTA.begin();
}

void loopOTA() {
    ArduinoOTA.handle();
}
