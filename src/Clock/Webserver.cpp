

#include "Webserver.h"
#include "config.h"

ESP8266WebServer Webserver::server(80);
EspHtmlTemplateProcessor Webserver::templateProcessor(&Webserver::server);

//-----------------------------------------------------------------------------
// WebServer Methods
//-----------------------------------------------------------------------------
// Create a new web server
Webserver::Webserver() {
}

void Webserver::initialize() {
    // Initialize file system.
    if (!SPIFFS.begin()) {
        Serial.println("SPIFFS nicht initialisiert!");
        while (1) {
            yield();
        }
    }
    Serial.println("SPIFFS ok");

    // Start Web Server
    server.on("/", Webserver::handleRootIndex);
    server.on("/config.html", Webserver::handleRootConfig);
    //webserver.serveStatic("/", SPIFFS, "/index.html");
    server.serveStatic("/config.html", SPIFFS, "/config.html");
    server.onNotFound(Webserver::notfoundPage);
    server.begin();
}

// Handle 404
void Webserver::notfoundPage() {
    Serial.println("GET not found");
    server.send(404, "text/plain", "404: Not found");
}

// Listen for HTTP requests
void Webserver::loopWebServer(void) {
    server.handleClient();
}

void Webserver::handleRootIndex() {
    templateProcessor.processAndSend("/index.html", Webserver::indexKeyProcessor);
}
void Webserver::handleRootConfig() {
    templateProcessor.processAndSend("/config.html", Webserver::indexKeyProcessor);
}

String Webserver::indexKeyProcessor(const String& var) {
    Serial.println(var);
    if (var == "CLOCK") {
        return "";//ntpTime.getTimeStr();
    } else if (var == "SSID") {
        return WiFi.SSID();
    } else if (var == "WLAN") {
        return "online";
    } else if (var == "NTPTIME") {
        return "";//ntpTime.getNtpRawTimeStr();
    } else if (var == "NTPOFFSET") {
        return String(_NTP_OFFSET);
    } else if (var == "NTPUPDATE") {
        return String(_NTP_UPDATE_INTERVAL);
    } else if (var == "LIBTIME") {
        return "";//ntpTime.getTimeStr();
    } else if (var == "LDRVALUE") {
        return "";//String(ldrSensor.getLdrValue());
    } else if (var == "LDRISDARK") {
        return "";//String(ldrSensor.isDark());
    } else if (var == "LDRTRESHHOLD") {
        return String(_LDR_TRESHOLD);
    }
}