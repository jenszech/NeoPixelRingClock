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

// Include WebServer Lib
#include <ESP8266WebServer.h>
#include <FS.h>  // muss vor <detail\RequestHandlersImpl.h> stehen (s. Hinweis im Anschluss)
#include <EspHtmlTemplateProcessor.h>

#include "NeoPixelLib.h"
#include "LdrLib.h"
#include "NtpTimeLib.h"

WiFiUDP ntpUDP;
ESP8266WebServer webserver(80);
EspHtmlTemplateProcessor templateProcessor(&webserver);

//-----------------------------------------------------------------------------
// MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN
//-----------------------------------------------------------------------------

NeoPixelLib neoPixel(_PIXEL_NUM, _PIXEL_PIN);
LdrLib ldrSensor(_LDR_PIN,  _LDR_TRESHOLD);
NtpTimeLib ntpTime(_NTP_OFFSET, _NTP_RSYNC, _NTP_UPDATE_INTERVAL, ntpUDP);

void setup() {
    Serial.begin(115200);
    connectToWifi();
    setupOTA();
    ntpTime.setupNTP();
    neoPixel.setupNeoPixel();
    setupWebServer();
}

long currentMillis = 0;
long lastLogMillis = 0;

void loop() {
    currentMillis = millis();
    //Update NeoPixel Display
    if(!ldrSensor.isDark()) {
        auto conv = map(ldrSensor.getLdrValue(), 0, 1024, 50, 255);
        neoPixel.setBrightness(conv);
    }
    neoPixel.loopPixelUpdate(ldrSensor.isDark());

    //Debug Output every 60sec.
    if (currentMillis - lastLogMillis > 60000) {
        lastLogMillis = millis();
        ntpTime.printSerialLog();
        ldrSensor.printSerialLog();
    }

    loopWebServer();
    loopOTA();
    delay(_PIXEL_UPDATE_INTERVAL);  // Pause before next pass through loop
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
        return ntpTime.getTimeStr();
    } else if (var == "SSID") {
        return WiFi.SSID();
    } else if (var == "WLAN") {
        return "online";
    } else if (var == "NTPTIME") {
        return ntpTime.getNtpRawTimeStr();
    } else if (var == "NTPOFFSET") {
        return String(_NTP_OFFSET);
    } else if (var == "NTPUPDATE") {
        return String(_NTP_UPDATE_INTERVAL);
    } else if (var == "LIBTIME") {
        return ntpTime.getTimeStr();
    } else if (var == "LDRVALUE") {
        return String(ldrSensor.getLdrValue());
    } else if (var == "LDRISDARK") {
        return String(ldrSensor.isDark());
    } else if (var == "LDRTRESHHOLD") {
        return String(_LDR_TRESHOLD);
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
