<!--- Cyber Physical Systems
Author: <Max Holzmann>  Date: <2021 01 06> 
Changes by:
<NAME> - <JJJJ MMM DD> - <comment> 

--->

**Cyber Physical Systems**   <img style="float:right" src="../0000_Global_Files/HM_SchriftzugLogo_RGB.png" width="200">  

Wintersemester 2020/21
***

# Lektion 10: The Total Rally ESP32 Code

von: Max Holzmann
***

Diese Lektion beinhaltet das gesamte RallyESP Programm und eine kurze Anleitung, wie man es auf den ESP übertragen und testen kann.

## Ziele

- Alle Programmteile der vorherigen Lektionen in einem Programm zusammengefasst
- Übersetzen des Programms in der Arduino IDE, übertragen und testen

## Voraussetzungen und erforderliches Equipment
- Die Tutorial Lektion 01: Grundlegende Einstellungen und Installationen muss erledigt sein [link](../0010_General_Setup/README.md)
- Hardware
    - 1x ESP DevKitC
    - 1x Waveshare 2.9inch e-Paper Module
    - 1x NFC Module with PN532 Chip
- Verwendete Software und verwendete Tools in diesem Tutorial
    - Arduino IDE

##  Lösungsschritte
1. Öffnen des Programms in der Arduino IDE \
ggf. Bibliotheken installieren, falls noch nicht geschehen
    - PubSubClient
    - Adafruit GFX Library + erforderliche Bibliotheken
    - Adafruit-PN532 (mit Änderungen, die in Lektion 05: NFC, Schritt 2 ([link zur Lektion](../0050_NFC/README.md)) beschrieben sind)
2. Hochladen des Programms
    - Beachten, das ESP32 als Hardware ausgewählt ist und der richtige COM port gewählt wurde
3. Test, ob die einzelnen Teile der Hard- uns Software funktionieren
## Hardware Details
### Anschlussplan
Pin ESP | Ziel
---|---
5V | NFC Board VCC
GND| NFC Board GND
GPIO 12      | NFC Board MISO 
GPIO 13      | NFC Board MOSI 
GPIO 14      | NFC Board SCK
GPIO 15      | NFC Board SS
3,3V | E-Paper VCC
GND | E-Paper GND
GPIO 4| E-Paper Busy 
GPIO 16| E-Paper RST
GPIO 17| E-Paper DC
GPIO 5| E-Paper CS
GPIO 18| E-Paper CLK
GPIO 23| E-Paper DIN
GPIO 27| Buzzer +
GND| Buzzer -
## Nützliche Links zur eigenen Recherche

- ESP mit Spannung versorgen
- richtige Verkabelung des E-Paper über SPI
- richtige Verkabelung des NFC Boards über SPI
- DIP-Schalter auf SPI am NFC Board

## Nützliche Links zur eigenen Recherche


<https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkitc.html>

## Hilfreiches Bild beim Anschließen

ESP32 Pinout
![DevKitC](https://components101.com/sites/default/files/component_pin/ESP32-Pinout.png)
