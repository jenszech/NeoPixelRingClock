# Roadmap

## Geplante Features
* Uhr Basic Funktion
    * Überblenden optimieren
    * BUG: Sekunden & Minutenzeigen überlappung
    * BUG: All Seconds On
    * Anzeige Alarmzeit
        Die Alarmzeit wird durch eine rote Markierung zusätzlich zur Uhrzeit anzeigt
* Zeitzonen
    * Zeitzonen handling / Lib replacment
    * Zeitzonenerkennung (Automatische Sommer / Winterzeit)
    * Automatisches setzen der Zeitzone (GeoLocation)
* Konfigurations Management
    * Farben Konfigurierbar machen
    * Alarm Sound konfigurieren
    * WLAN bei ersten Boot initialisieren
    * Hell / Dunkelmodus Konfigurierbar (SleepMode, NightMode)
* Hardware Erweiterung
    * Drehgeber Integration
    * Display Imtegration
    * Buzzer Integration (Alarm, Taster)
    * MP3 Modul & Lautsprecher
    * Integration Helligkeitssensor
* Webserver Integration
    * WebServer für Config
    * WebServer zur Alarmverwaltung
    * MobilFirst Design der Webseite
* Sound
    * Tastertöne beim Einstellen
    * Individuelle AlarmSounds
* Helligkeitssensor
    * Konfigurierbarer SleepMode
    * Konfigurierbarer NightMode
    * Alarm per Lichtsensor / Bewegung deaktivieren
* Drehgeber
    * Einstellen einer Alarmzeit
    * Aktivieren / Deaktivieren eines Alarms
    * Starten einer StopUhr
* Zusatzfunktionen
    * Andere Uhrzeit Modi: 24h / 
    * Timer / Countdown Clock
    * StopUhr
* OTA wieder aktivieren
* Dokumentation
    * Bauanleitung
    * Platinenlayout
    * ProjektSetup & Deploy
    * Nutzungsanleitung
    * Bestellliste

## Changelog

### 1.0.x

* Auslagern des LdrSensors in eine eigene Klasse

### 1.0.2

* Umstellung auf VSCode
* Auslagern der PixelSteuerung in eine eigene Klasse

### 1.0.1

* Hardware Layout als Fritzing Entwurf hinzugefügt
* Lizenz Files & CodeOfConduct hinzugefügt

### 1.0.0

* Erste lauffähige Version der Uhr
* Anbindung an den NTP Zeitserver
* OverTheAir Update implementiert
* HTTP Interface mit Static HTML 

### 0.0.1

* Initiales Projekt Setup

  
