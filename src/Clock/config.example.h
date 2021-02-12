// WiFi specific defines
#define _wifiTimeOutSeconds 15
#define _ssid     "<YOUR WLAN SSID>"
#define _password "<YOUT WLAN Password>"

// NeoPixel defines
#define PIN 4
#define NUMPIXELS 60                 // Popular NeoPixel ring size
#define _PIXEL_UPDATE_INTERVAL 500   // Time (in milliseconds) to pause between pixels

// Time defines
#define _resyncSeconds 15*60        // How often to resync the time from NTP to TimeLib
#define _ntpUpdateIntervall 12*60*60  // NTP Update Intervall
#define _ntpTimeOffset 2*60*60      // NTP Timezone offset
