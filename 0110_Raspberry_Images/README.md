<!--- Cyber Physical Systems
Author: <Max Holzmann>  Date: <2021 01 06> 
Changes by:
<NAME> - <JJJJ MMM DD> - <comment> 

--->

**Cyber Physical Systems**   <img style="float:right" src="../0000_Global_Files/HM_SchriftzugLogo_RGB.png" width="200">  

Wintersemester 2020/21
***

# Lektion 11: The Total Raspberry Pi 3 Image

von: Max Holzmann
***
Diese Lektion beinhaltet das gesamte Raspberry Pi 3 B Image und eine kurze Anleitung, wie man es auf den Raspberry Pi 3 B übertragen und testen kann.

## Ziele

- Alle Programmteile und Konfigurationen der vorherigen Lektionen in einem Image zusammenfassen
- Schreiben des Images auf eine SD Karte und Testen am Pi

## Voraussetzungen und erforderliches Equipment
- Die Tutorial Lektion 01: Grundlegende Einstellungen und Installationen muss erledigt sein [link](../0010_General_Setup/README.md)
- Hardware
    - 1x Raspberry Pi 3B
    - 1x ESP DevKitC
    - 1x NFC Module mit PN532 Chip
    - 1x 4G Raspberry Pi HAT mit SIM7600E
- Verwendete Software und verwendete Tools in diesem Tutorial
    - WinDiskImager
    - VNC Viewer
    - Putty

## Lösungsschritte

1. Entzippen des Images
2. Schreiben des Images auf eine SD Karte mit einem Imager, z.B. WinDiskImager 
3. Hardware anschließen
    1. (optional) Anschließen des Raspberry Pi Displays [link to Tutorial](https://www.youtube.com/watch?v=nVnW8J9Fq2k&feature=emb_title)
    2. Anschließen des 4G HATs über GIPO und USB [detailliertere Informationen in Lektion 02](../0020_4G_Connection/README.md)
    3. ESP32 als NFC Tag Leser und Schreiber anschließen [detailliertere Informationen in Lektion 05](../0050_NFC/README.md)
    4. Anschließen der Lichtschranke an den GIPO PIN 7 mit galvanischer Trennung, z.B. Optokoppler

4. Testen, ob der Pi startet und ob alle Programmteile sinngemäß funktionieren. ggf sind Netzwerkeinstellungen anzupassen.

## Hardware Details
- Raspberry Pi 3 Model B Rev 1.2
- Raspberry Pi Display V1.1 (optional)
- SIM7600E-H 4G HAT

### verwendete GPIO Pins:
Raspberry PI| Ziel
---| ---
5V | 5V Display 
GND | GND Display
USB | USB on HAT
P15 | RXD 4G HAT (nicht verwendet)
P14 | TXD 4G HAT (nicht verwendet)
P6  | PWR 4G HAT
P4  | Fugmodus 4G HAT
P7 | Lichtschranke

## Tipps und mögliche Stolperfallen

Vor dem Start, sollten folgende Punkte kontrolliert werden
- Spannungsversorgung des Raspberry pis über USB mit einem Netzteil mit mindestens 2,5A
- 4G Waveshare HAT über GIPO und USB angeschlosssen
- ESP32 über USB mit NFC Board angeschlossen
- Anschließen der Lichtschranke über galvanische Trennung an GPIO

## Nützliche Links und Bilder 

<https://raspberrypi.org>

<img src="01_Pictures/GPIO-Pinout-Diagram-2.png" width="400">

## Quellen
[1] <https://raspberrypi.org>