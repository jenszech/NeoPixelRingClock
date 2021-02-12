/*
  Project: WS2812 Ring LED clock with NTP server using ESP32
  Board: Wemos D1 Mini 
  
  Connections:
  ESP32 | OLED Strip
    RAW - VCC
    GND - GND
      4 - DIN
  
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

// Include NeoPixel Lib
#include <Adafruit_NeoPixel.h>

// Include WebServer Lib
#include <ESP8266WebServer.h>
#include <FS.h> // muss vor <detail\RequestHandlersImpl.h> stehen (s. Hinweis im Anschluss)
#include <EspHtmlTemplateProcessor.h>


WiFiUDP ntpUDP;
ESP8266WebServer webserver(80);
EspHtmlTemplateProcessor templateProcessor(&webserver);
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
  setupWebServer();
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

  loopWebServer();
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
  String timeStr ="";
  time_t t=now();
  
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
  if (digits < 10) return "0"+String(digits);
  return String(digits);
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

int getLdrValue() {
  return analogRead(_LDR_PIN);
}

void serialLdrLog() {
  int sensorValue = analogRead(_LDR_PIN); // read analog input pin 0
  
  Serial.print("LDR: ");
  Serial.print(sensorValue, DEC);
  Serial.print(" => isDark: ");
  Serial.println(isDark());
}


//-----------------------------------------------------------------------------
// WebServer Methods
//-----------------------------------------------------------------------------
// Create a new web server
void setupWebServer()
{
  // Initialize file system.
  if (!SPIFFS.begin()) { 
    Serial.println("SPIFFS nicht initialisiert!");
    while (1){ 
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
void notfoundPage(){ 
  Serial.println("GET not found");
  webserver.send(404, "text/plain", "404: Not found"); 
}

// Listen for HTTP requests
void loopWebServer(void){ 
  webserver.handleClient(); 
}

void handleRootIndex() {
  templateProcessor.processAndSend("/index.html", indexKeyProcessor);
}
void handleRootConfig() {
  templateProcessor.processAndSend("/config.html", indexKeyProcessor);
}


String indexKeyProcessor(const String& var){
  Serial.println(var);
  if (var == "CLOCK"){
    return getTimeStr();
  }
  else if (var == "SSID"){
    return WiFi.SSID();
  }
  else if (var == "WLAN"){
    return "online";
  }
  else if (var == "NTPTIME"){
    return timeClient.getFormattedTime();
  }
  else if (var == "NTPOFFSET"){
    return String(_ntpTimeOffset);
  }
  else if (var == "NTPUPDATE"){
    return String(_ntpUpdateIntervall);
  }
  else if (var == "LIBTIME"){
    return getTimeStr();
  }
  else if (var == "LDRVALUE"){
    return String(getLdrValue());
  }
  else if (var == "LDRISDARK"){
    return String(isDark());
  }
  else if (var == "LDRTRESHHOLD"){
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
