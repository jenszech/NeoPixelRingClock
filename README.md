# NeoPixelRingClock

Dieses Projekt verwendet die Adafruit NeoPixel-Bibliothek, um einen 60-LED-NeoPixel-Ring als Uhr zu steuern.

## Erforderliche Hardware
* 4 x Neo Pixel 1/4 Ringe (je 15 LEDs)
* 1 x ESP8266-Modul. Ich benutze den WeMos D1 mini
* 1 x LDR GL5528
* 1 x 1k Ohm Widerstand
* 1 * 1000uF 10V Kondensator

## Features
* Anzeige der Uhrzeit durch blaue Marker
* Anzeige von Stundenmarkierungen
* Helligkeitssensor wechselt zwischen Day & Night Modus
* WebServer auf dem ESP zeigt die aktuelle Configuration & Urhzeit

## Geplante Features
* Einstellen einer Alarmzeit
* Die Alarmzeit wird durch eine rote Markierung zusätzlich zur Uhrzeit anzeigt
* Rest / HTML API zum steuern / einstellen der Uhr per Smartphone
* Dokumentation per Fritzing Hardware Beispiel
 
 ![Hardware Beispiel](/doku/clock.jpg)
