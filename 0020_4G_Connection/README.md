<!--- Cyber Physical Systems
Author: <Max Holzmann>  Date: <2020 11 29> 
Changes by:
<NAME> - <JJJJ MMM DD> - <comment> 

--->
**Cyber Physical Systems**   <img style="float:right" src="../0000_Global_Files/HM_SchriftzugLogo_RGB.png" width="200">  

Wintersemester 2020/21
***

# Lektion 02: Raspberry mit dem 4G Netz verbinden
von: Max Holzmann
***

Der Raspberry Pi bildet die Basis des Rally Masters. Der Rally Master soll mit dem Internet über Mobilfunk verbunden werden, um sich mit einem Zeitserver verbinden zu können. Das ist erforderlich um am Aufstellort immer die korrekte Uhrzeit als Zeitbasis für das System sicherstellen zu können. 

## Ziele

- Einrichten der 4G Verbindung über das 4G HAT von Waveshare als Interface am Pi
- Automatisches Verbinden mit dem Mobilfunknetz beim Start
- NTP Service an gewünschten Server anpassen 

<img src="01_Pictures\4G_HAT.jpg" height="200">
<img src="01_Pictures\4G_HAT_with_ant.jpg" height="200">

## Voraussetzungen und erforderliches Equipment
- Lektion 01 : Grundlegende Einstellungen muss erledigt sein [link](../0010_General_Setup/README.md)
- Hardware
    - 1x Raspberry Pi 3 B
    - 1x Waveshare 4G HAT SIM7600E-H [link producer](https://www.waveshare.com/wiki/SIM7600E-H_4G_HAT) [link distributor](https://www.berrybase.de/neu/4g/3g/2g/gsm/gprs/gnss-hat-f-252-r-raspberry-pi-eu-version)
    - 1x SIM Card (mit 4G und ohne pin). Im Tutorial wurde eine Karte von Vodafone Deutschland verwendet
- Verwendete Software und verwendete tools in diesem Tutorial
    - VNC Viwer als remote desktop
    - Putty als SSH Terminal


## Lösungsschritte 

### 1. Hardware Vorbereitung 
1. Sim Karte ohne Pinsperre in 4G HAT stecken
2. HAT UART Jumper auf A einstellen
3. HAT VCC Jumper auf 5V einstellen
4. GPS Antenne anschließen
5. Main Antenne anschließen
6. HAT mit dem Pi über GPIO verbinden
7. HAT über USB zusätzlich anschließen (HAT USB to Uart -> USB Port Pi)
8. System Starten
9. --> Der Pi sollte booten und die PWR LED und die NET LED des HAT sollte rot leuchten

### 2. Es gibt mehrere Möglichkeiten das 4G HAT zu verwenden

Im Tutorial wird das 4G HAT als Interface am Pi eingerichtet. Die Einrichtung als Interface benötigt Zeit, jedoch kann anschließend die Verbindung zum Mobilfunk ohne weiteren Aufwand erfolgen.

Wenn man jedoch daran interessiert ist, das HAT über AT Kommandos zu venwenden, dann kann ich die Beschreibung aus der Bedienungsanleitung des HAT empfehlen.

### 3. Verbinden des HAT als Interface mit der Bibliothek libqmi-utils von  Qualcomm  devices
1. Aktivieren des Seriellen Ports und deaktivieren der seriellen Konsole in der Raspberry Konfiguration
    ```console
    sudo raspi-config
    #enable the serial Port
    #disable the Serial console
    sudo reboot
    ```
3. Updaten der Pakete und installieren der libqmi-utils und udhcpc Bibliothek
    ```console
    sudo apt-get update && sudo apt-get install libqmi-utils udhcpc
    ```
4. Starten des SIM7600E Moduls mit der qmicli Software
    ```console
    sudo qmicli -d /dev/cdc-wdm0 --dms-set-operating-mode='online'
    ```
5. Benutzerrechte für Interface anpassen
    ```console
    sudo chown pi /dev/cdc-wdm0
    ```
6. Kontrollieren ob Netzwerkverbindung erfolgreich war
    ```console
    qmicli -d /dev/cdc-wdm0 --dms-get-operating-mode
    qmicli -d /dev/cdc-wdm0 --nas-get-signal-strength
    qmicli -d /dev/cdc-wdm0 --nas-get-home-network
    ```
7. Ändern des 802-3 Protokolls in das raw-ip Protokoll für das Interface wwan0
    ```console
    sudo qmicli -d /dev/cdc-wdm0 -w
    sudo ip link set wwan0 down
    echo 'Y' | sudo tee /sys/class/net/wwan0/qmi/raw_ip
    sudo ip link set wwan0 up
    ```
8. Verbinden zum Netzwerk mit dem Pi Benutzernamen und dem Benutzerpasswort
    ```console
    qmicli -p -d /dev/cdc-wdm0 --device-open-net='net-raw-ip|net-no-qos-header' --wds-start-network="apn='web.vodafone.de',username='pi',password='Rally2020$',ip-type=4" --client-no-release-cid
    ```
9. Setzten des Standard Routes und holen der IP Adresse aus dem 4G Netz
    ```console
    sudo udhcpc -i wwan0
    ip a s wwan0
    ```
10. Nun sollte der Pi mit dem 4G Netz verbunden sein. Ein einfacher Test ist, alle anderen Netzwerkverbindungen zu trennen und eine Website im Browser zu öffnen.

11. Um nicht beim jedem Neustart manuell eine Verbindung herzustellen, werden folgende Codezeilen in einem Bash-Skript zusammengefasst.
Das vollständige Bash-Skript kann dem Repository entnommen werden ([Link](00_Code/startInternetConnect.sh)). Auf dem Pi liegt es im Ordner /home/pi/rally/startInternetConnect und kann manuell durch diesen Befehl gestartet werden
    ```console
    /home/pi/rally/startInternetConnect.sh
    ```

12. Um das Skript beim Start auszuführen, müssen in der Datei /etc/rc.local diese Zeilen eingefügt werden. 
    ```console
    #open file
    sudo nano /etc/rc.local
    #line to add at the end
    sudo sh /home/pi/rally/startInternetConnect.sh &
    ```

### 4. Konfigurieren des NTP Services mit dem gewünschten NTP Server
Es ist wichtig, die Zeit des Systems mit der lokalen Zeit zu synchronisieren. Ansonsten werden die Messungen ungenau und die Anwendung ist zum Rallye-Training nicht einsetzbar.

1. Zur Synchronisation muss der Timedatectl-service  konfiguriert werden.
    ```console
    sudo nano /etc/systemd/timesyncd.conf
    ```
2. Die folgenden Zeilen dieser Konfigurationsdatei müssen auf die lokalen NTP Server geändert werden. Herauszufinden sind die NTP Server durch eine kurze Internetrecherche. Ich habe in diesem Tutorial die Zeitserver von Vodafone Deutschland verwendet, da der Pi mit dem 4G Netz von Vodafone verbunden ist (geringere Verzögerug). Als Rückfallserver wurden Deutsche NTP Server eingetragen.
    ```console
    [Time]
    NTP=ntp0.one.vodafone-ip.de          
    FallbackNTP=ptbtime1.ptb.de 0.de.pool.ntp.org
    ```

## Nützliche Links zur eigenen Recherche

4G HAT als Interface Interface

[weiteres Tutorial](https://developers.telnyx.com/docs/v2/wireless/tutorials/sim7600)

[nützlicher Beitrag im Pi forum](https://www.raspberrypi.org/forums/viewtopic.php?t=224355) (Re: 4G Hat Quote Wed Apr 03, 2019 11:35 am)


## Quellen

[1] [Berry Base Shop](https://www.berrybase.de/neu/4g/3g/2g/gsm/gprs/gnss-hat-f-252-r-raspberry-pi-eu-version)

[2] [developers.telnyx.com](https://developers.telnyx.com/docs/v2/wireless/tutorials/sim7600)

[3] [raspberrytips.com](https://raspberrytips.com/time-sync-raspberry-pi/ )
