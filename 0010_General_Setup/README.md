<!--- Cyber Physical Systems
Author: <Max Holzmann>  Date: <2020 11 29> 
Changes by:
<NAME> - <JJJJ MMM DD> - <comment> 

--->
**Cyber Physical Systems**   <img style="float:right" src="../0000_Global_Files/HM_SchriftzugLogo_RGB.png" width="200">  

Wintersemester 2020/21
***

# Lektion 01: Grundlegende Einstellungen und Installationen
von: Max Holzmann
***

## Ziele
- EntwicklungsPC einrichten um ESP Programme zu kompilen und zu flashen
	- Arduino IDE
	- Visualstudio Code mit Espressiv extention
- Raspberry Pi aufsetzen und basis OS Konfiguration


## Konfigurieren der Arduino IDE

1. Installieren der aktuellsten Arduino IDE
2. Arduino IDE für ESP einrichten. Mithilfe des Heise Tuorials. ([link, "Profi-Installation über GIT" empfohlen](]https://m.heise.de/make/artikel/ESP-Boards-mit-der-Arduino-IDE-programmieren-4130861.html?seite=all))


## Konfigurieren der Visual Studio Code Programierumgebung mit Espressiv IDE Erweiterung
Hinweis: Die Schritt für Schritt Anleitung ist für die VS code Version 1.15 und die Espressiv extention Verison vor 2021 beschrieben. In neueren Releases wurde die Installation verändert und grafisch überarbeitet.

Hinweis: Zu Beginn des Projekts wurde in VS Code programmiert, da diese Entwicklungsumgebung deutlich mehr Funktionen bietet. Jedoch hat sich während dem Projektverlauf gezeigt, dass das Bibliotheken-handling und das Kompilieren immer wieder fehleranfällig ist. Aus diesem Grund wurde zurück auf die Arduino IDE gewechselt. Die Arduino IDE erleichtert die Arbeit mit Bibliotheken, dafür ist jedoch der Editor nicht auf dem aktuellen Stand der Technik. 

### Voraussetzungen und erforderliches Equipment
- Git ((2.29.2) 64-bit)
- python (3.9.1) (manchmal gibt es bei der Installation Probleme mit bestehenden Python Umgebungen, hier kann eine neue separate Pyhon Umgebung weiterhelfen)
- Build Tools for C++ ([Build Tools for Visual Studio 2019 - Microsoft](https://visualstudio.microsoft.com/de/thank-you-downloading-visual-studio/?sku=BuildTools&rel=16))
- visual studio code (November 2020 (version 1.52))

### Lösungsschritte
1. Installation der Espressiv IDE Extention im VS Code Anwendungskatalog
2. In die Espressiv Extention wechseln und den Installations guide folgen
	- Tipps zur Installation:
	- Installation in user space
	- Verison V4.0.x (Die Arduino Bibliotheken werden aktuell nur bis V4.0 unterstützt)
	1. Alle Tools installieren
	2. Tool Konfiguration
		1. Pfad zur gewünschten Pythonumgebung  für espressiv eingeben
		2. Auf check klicken, es werden wahrscheinlich einige Fehler auftreten. Die Fehlerausgabe ist sehr ausführlich und hilfreich bei der Fehlersuche!			- Zur Fehlerkorrektur die Konsole CMD öffnen und die aus dem FehlerLog empfohlenen Befehle ausführen, bei mir waren das die folgenden:
			- YOUR_PATH\esp-idf\install.bat zur Installation fehlender Pakete
			- YOUR_PATH\esp-idf\export.bat to um fehlende Pfade zur Umgebungsvariable hinzuzufügen
			- C:\\Users\\holzi\\.espressif\\python_env\\idf4.0_py3.9_env\\Scripts\\python.exe -m pip install -r c:\\Users\\holzi\\.vscode\\extensions\\espressif.esp-idf-extension-0.5.1\\esp_debug_adapter\\requirements.txt 
			- Manchmal müssen Pyhton-pakete manuell über pip installiert werden. Ich musste folgende manuell installieren: 
				- click>=5.0
				- pyserial>=3.0
				- future>=0.15.2
				- cryptography>=2.1.4
				- pyparsing>=2.0.3,<2.4.0
				- pyelftools>=0.22
					
	3. Der Installations Guide teilt dem Nutzer mit, wenn alle Installationen erfolgreich waren
	4. Nun sollte man den folgenden Text sehen: "ESP-IDF Tools have been configured for this extension of Visual Studio Code."
7. Es sollte nun möglich sein, die mitgelieferten Beispiele zu übersetzen und zu flaschen.
8. Hinweis: Wenn während der Installation VS code neu gestartet werden muss, kann man in der ESP Installation mit ein paar Klicks wieder weitermachen.


## Aufsetzen und konfigurieren des Raspberry Pi 3 B

### Voraussetzungen und erforderliches Equipment
Hardware:
- Raspberry Pi 3 Model B Rev 1.2
- Raspberry Pi Display V1.1 (optional)

### Lösungsschritte
1. Raspberry Pi an Display anschließen ([link to Tutorial](https://www.youtube.com/watch?v=nVnW8J9Fq2k&feature=emb_title))
2. Raspberry OS mithilfe des Raspberry Imagers auf die SD-Karte übertragen
	- [hilfreicher link](https://www.raspberrypi.org/software/) 
	- release 2020-12-02
	- (andere Imager wie der WinDiskImager sidn auch möglich)
3. Tastatur und Maus anschließen (optional)
4. Stromversorgung via USB
5. Raspberry Pi Konfiguration
	```console
	sudo raspi-config
	```
	- Benutzer pi
	- Passwort Rally2020$ *your own PW*
	- enable SSH
	- enable VNC