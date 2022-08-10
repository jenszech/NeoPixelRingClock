// WiFi specific defines
#define _wifiTimeOutSeconds 15
#define _mySsid     "<YOUR WLAN SSID>"
#define _myPassword "<YOUR WLAN Password>"

#define _myMDNSHostName "<YOUR MDNS Hostname>" //Webinterface is reachable via "<YOUR MDNS Hostname>.local"

// NeoPixel defines
#define _PIXEL_PIN 4
#define _PIXEL_NUM 60 // Popular NeoPixel ring size
#define _PIXEL_UPDATE_INTERVAL 500 // Time (in milliseconds) to pause between pixels

// How often to resync the time (under normal and error conditions)
#define _NTP_RSYNC 15*60                // RSYNC intervall in seconds
#define _NTP_UPDATE_INTERVAL 12*60*60  // ntp update intervall in seconds
#define _NTP_OFFSET 2*60*60             // time offset in seconds

// LDR Setting
#define _LDR_PIN A0
#define _LDR_TRESHOLD 20
