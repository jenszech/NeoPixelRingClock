// WiFi specific defines
#define _wifiTimeOutSeconds 15
#define _mySsid     "<YOUR WLAN SSID>"
#define _myPassword "<YOUT WLAN Password>"

// NeoPixel defines
#define _PIXEL_PIN 4
#define _PIXEL_NUM 60 // Popular NeoPixel ring size
#define _PIXEL_UPDATE_INTERVAL 500 // Time (in milliseconds) to pause between pixels

// How often to resync the time (under normal and error conditions)
#define _resyncSeconds 15*60
#define _ntpUpdateIntervall 12*60*60
#define _ntpTimeOffset 1*60*60

// LDR Setting
#define _LDR_PIN A0
#define _LDR_TRESHOLD 20
