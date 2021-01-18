<!--- Cyber Physical Systems
Author: <Max Holzmann>  Date: <2020 11 29> 
Changes by:
<NAME> - <JJJJ MMM DD> - <comment> 

--->
**Cyber Physical Systems**   <img style="float:right" src="../0000_Global_Files/HM_SchriftzugLogo_RGB.png" width="200">  

Wintersemester 2020/21
***

# Lektion 07: MongoDB einrichten und in Rallye Anwendung integrieren
von: Max Holzmann
***

In der Rallye Anwendung wird als Datenbank für die Zeitstempel die MongoDB verwendet.

## Ziele
- Einrichten der Datenbank MongoDB auf dem Raspberry Pi
- Befüllen der Datenbank über Node Red
- Anzeigen von Datenbankeinträgen in einer einfachen Python QT5 GUI

Hinweis: Auf dem Raspberry Pi sind nur MongoDB Server bis zur Version 2.4 verfügbar. Grund hierfür ist, dass ab höheren Versionen MongoDB eine 64 bit Plattform benötigt.

## Voraussetzungen und erforderliches Equipment
- Die Tutorial Lektionen ... \
01: Grundlegende Einstellungen und Installationen  [link](../0010_General_Setup/README.md) \
04: Node Red installieren  [link](../0040_Node_Red/README.md) \
... müssen erledigt sein
- Hardware
    - 1x Raspberry Pi 3 B


## Lösungsschritte Installation
1. Installation von MongoDB über den Paketmanager apt
    ```console
    sudo apt update
    sudo apt upgrade
    sudo apt install mongodb
    ```

2. MongoDB als Service starten
    ```console
    sudo systemctl enable mongodb
    sudo systemctl start mongodb
    ```

3. Test und kommandieren der Datenbank durch ...
    ```console
    mongo
    ```
Die MogoDB Konsole sollte starten und die Version ausgeben. Im Falle des Tutorials version 2.4.14.

4. Weitere Tests können sein
- check if the service is activ
    ```console
    sudo systemctl status mongodb
    ```
- check the mongoDB connection status by
    ```console
    mongo --eval 'db.runCommand({ connectionStatus: 1 })'
    ```
## Lösungsschritte Datenbank einrichten
1. MongoDB Konsole öffnen
    ```console
    mongo
    ```
2. Datenbank erstellen (TIPP: eine bereits erstellte Datenbank auswählen, erfolgt über den selben Befehl)
    ```console
    use rallyDB
    ```
3. Collection mit auto Indexierung für Durchfahrt erstellen
    ```console
    db.createCollection("driveThroughs", { autoIndexID :true }
    ```
4. Test Dokument in Collection einfügen, mit dem Befehl
    ```console
    db.driveThroughs.insert({ nr: "1", car: "car_x", time: "12:15:35,12 2021.01.07", diff: "1234" })
    ```
5. Testen, ob Dokument in Collection eingefügt wurde 
    ```console
    db.driveThroughs.find()
    ```

## Datenbank in Node Red einbinden
1. Installieren einer Bibliothek, falls das noch nicht in der Lektion 04: Node red erfolgt ist.

2. Einfügen eines "mongodb2 in" Knoten in den flow.

    Einstellungen des mongodb2 nodes:
    - Service: External Service
    - Server:
    - URI: mongodb://localhost:27017/rallyDB
    - Name: rallyDB
    - Collection: driveThroughs
    - Operation: Dynamic(msg.opperation)
    - Name: rallyDB

    Nun sollte es möglich sein msg Objekte an den Node zu senden. Das Objekt muss das Attribut *operation* besitzen. Zum Beispiel 'insert' als DB Kommando, um ein Dokument in die Collection einzufügen. Eingefügt wird die Payload der msg. Wenn das *_id* Attribut der msg auf *null* gesetzt wird, dann wird automatisch eine neue ID für den Datenbankeintrag beim Einfügen generiert.

In der Lektion 04: Node Red ist der Code für Node Red enthalten sowie eine detaillierte Beschreibung der Node ketten

## Darstellen der Datenbankeinträge in einer QT5 Python Anwendung

Eine einfache Python3 Anwendung stellen die letzten 10 Einträge aus dem Raspberry Pi Display dar. Hierzu wird die Python Bibliothek QT5 und pymongo(version 2.9.5) verwendet. Es ist wichtig keine neuere pymongo version zu verwenden, da sonst die MongoDB 2.4 nicht mehr unterstützt wird.

1. Installation der Binliotheken
    ```console
    pip3 install pymongo == pymongo==2.9.5
    sudo apt-get install python3-pyqt5
    ```

2. Das Programm liegt im Ordner  [00_Code](00_code/simpleDatabaseViewer.py.).

3. Hier ein Bild der GUI

    <img src="01_Pictures/PythonGui.jpg" width="500">  


## Quellen

[1] [pimylifeup.com](https://pimylifeup.com/mongodb-raspberry-pi/)

[2] [www.ionos.de](https://www.ionos.de/digitalguide/websites/web-entwicklung/mongodb-tutorial-installation-und-einstieg/)