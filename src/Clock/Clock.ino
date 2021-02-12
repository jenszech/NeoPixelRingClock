/*
  Project: WS2812 Ring LED clock with NTP server using ESP32
  Board: ESP32 Dev Module (Node32 Lite)
  
  Connections:
  ESP32 | OLED Strip
    RAW - VCC
    GND - GND
     25 - DIN
  
  External libraries:
  - NeoPixelBus by Micheal C. Miller V2.5.7 (Manager)
  - NTPClient by Fabrice Weinberg V3.3.0 
    https://github.com/arduino-libraries/NTPClient/blob/master/NTPClient.h   
  - TimeLib by Paul Stoffregen
    https://github.com/PaulStoffregen/Time
 */

#include <arduino.h>

// My secret stuff (eg WiFi password)
#include "config.h"

// How we connect to your local wifi
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

// UDP library which is how we communicate with Time Server
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ArduinoOTA.h>

// See Arduino Playground for details of this useful time synchronisation library
#include <TimeLib.h>

#include <Adafruit_NeoPixel.h>


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// forward declarations
void serialTimeLog();
void setupNTP();
void setupNeoPixel();
void displayBasicClock();
time_t updateTimeByNTP();

//-----------------------------------------------------------------------------
// MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN   MAIN
//-----------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  connectToWifi();
  setupOTA();
  setupNTP();
  setupNeoPixel();
}

long currentMillis = 0;
long previousMillis = 0;
int isDarkMode = false;

void loop() {
  currentMillis = millis();
  isDarkMode = isDark();
  
  //Update NeoPixel Display
  loopPixelUpdate();

  //Debug Output every 60sec.
  if (currentMillis - previousMillis > 60000) {
    previousMillis = millis();
    serialTimeLog();
    serialLdrLog();
  }

  loopOTA();
  delay(_PIXEL_UPDATE_INTERVAL); // Pause before next pass through loop
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
  setSyncInterval(_resyncSeconds); // just for demo purposes!
  Serial.print("NTP Time: ");
  Serial.println(timeClient.getFormattedTime());  
  Serial.print("TimeLib: ");
  serialTimeLog();
}

//-----------------------------------------------------------------------------
// TimeLib Functions
//-----------------------------------------------------------------------------

// resync Method - get Time from NTPClient
time_t updateTimeByNTP() {
  Serial.println("TimeLib resync");
  return timeClient.getEpochTime();
}

void serialTimeLog() {
  // We'll grab the time so it doesn't change whilst we're printing it
  time_t t=now();

  //Now print all the elements of the time secure that it won't change under our feet
  printDigits(hour(t));
  Serial.print(":");
  printDigits(minute(t));
  Serial.print(":");
  printDigits(second(t));
  Serial.print("    ");
  printDigits(day(t));
  Serial.print("/");
  printDigits(month(t));
  Serial.print("/");
  printDigits(year(t));
  Serial.println();
}

void printDigits(int digits) {
  // utility for digital clock display: prints leading 0
  if (digits < 10) Serial.print('0');
  Serial.print(digits);
}

//-----------------------------------------------------------------------------
// NeoPixel Methods
//-----------------------------------------------------------------------------

void setupNeoPixel() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loopPixelUpdate() {
  uint32_t cSecond = pixels.Color(0,  10, 0);
  uint32_t cMinute = pixels.Color(0,  50, 0);
  uint32_t cHour   = pixels.Color(0,  25, 0);
  uint32_t cHourLR = pixels.Color(0,  5 , 0);
  int hourNow, hourL, hourR;
  if (isDarkMode) {
    cMinute = pixels.Color(0,  5, 0);
    cHour   = pixels.Color(0,  5, 0);
    cHourLR = pixels.Color(0,  1 , 0);
  }
 
  time_t t=now(); 
  pixels.clear(); // Set all pixel colors to 'off'
  clockSegments();

  // Now print all the elements of the time 
  hourNow = hour(t);
  while (hourNow > 11) {
      hourNow = hourNow - 12;
  }
  hourNow = (hourNow * 5) + (hourNow / 12);
  hourL=hourNow-1;
  hourR=hourNow+1;
  if (hourR == 60) hourR = 0;
  if (hourL == -1) hourL = 59;

  pixels.setPixelColor(hourL, cHourLR);
  pixels.setPixelColor(hourNow, cHour);
  pixels.setPixelColor(hourR, cHourLR);
  pixels.setPixelColor(minute(t), cMinute);
  if (!isDarkMode) pixels.setPixelColor(second(t), cSecond);
  pixels.show();   // Send the updated pixel colors to the hardware.
}

void clockSegments() {
  uint32_t cWhiteDim = pixels.Color(5, 5, 5);
  uint32_t cWhite = pixels.Color(20, 20, 20);

  if (isDarkMode) {
    for(int b=0; b<4; b++) { 
      pixels.setPixelColor(b*15, cWhiteDim);
    }
  } else {
    for(int b=0; b<12; b++) { 
      pixels.setPixelColor(b*5, cWhiteDim);
    }
    for(int b=0; b<4; b++) { 
      pixels.setPixelColor(b*15, cWhite);
    }
    
  }
}

//-----------------------------------------------------------------------------
// LDR Methods
//-----------------------------------------------------------------------------

bool isDark() {
  int sensorValue = analogRead(_LDR_PIN); // read analog input pin 0
  return (sensorValue < _LDR_TRESHHOLD);
}

void serialLdrLog() {
  int sensorValue = analogRead(_LDR_PIN); // read analog input pin 0
  
  Serial.print("LDR: ");
  Serial.print(sensorValue, DEC);
  Serial.print(" => isDark: ");
  Serial.println(isDark());
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
