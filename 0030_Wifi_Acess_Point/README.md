<!--- Cyber Physical Systems
Author: <Max Holzmann>  Date: <2020 11 29> 
Changes by:
<NAME> - <JJJJ MMM DD> - <comment> 

--->
**Cyber Physical Systems**   <img style="float:right" src="../0000_Global_Files/HM_SchriftzugLogo_RGB.png" width="200">  

Wintersemester 2020/21
***

# Lektion 03: Konfigurieren des WLAN Chips des Raspberry pi als Acesspoint
von: Max Holzmann
***
Der Acesspoint wird dafür verwendet, um rund um den Rallymaster ein WLAN Netz aufzuspannen. Mit diesem Netz verbinden sich die RallyESPs zum Datenaustausch.

## Ziele

- Einrichten des WLAN Chips des Raspberry Pi als Acesspoint 
- Einrichten des Accespoints ohne weiterreichen der Internetverbindung. (Für diese Anwendung nicht notwendig und spart Datenvolumen)

## Voraussetzungen und erforderliches Equipment
- Die Tutorial Lektion 01: Grundlegende Einstellungen und Installationen muss erledigt sein [link](../0010_General_Setup/README.md)
- Hardware
    - 1x Raspberry Pi 3 B

## Lösungsschritte
1. Installation hostapd als acesspoint software und dnsmasq als DNS Server.
    ```console
    sudo apt-get install hostapd
    sudo apt-get install dnsmasq
    ```
2. Zur Konfiguration beide Services stoppen.
    ```console
    sudo systemctl stop hostapd
    sudo systemctl stop dnsmasq
    ```
3. Einstellen einer statischen IP für das wlan0 Interface  in der Datei /etc/dhcpcd.conf 
    ```console
    sudo nano /etc/dhcpcd.conf
    #add the following lines at the end. Choose your own ip for the wifi AP
    interface wlan0
    static ip_address=192.168.5.1
    static routers=
    static domain_name_servers=
    static domain_search=
    ```
4. Konfigurieren des dhcp Servers
    1. Backup  der Konfigurationsdatei erstellen und neue Konfigurationsdatei erstellen
        ```console
        sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig
        ```
    2. In der neuen /etc/dnsmasq.conf den DNS IP Bereich zwischen 192.168.0.11 und 192.168.0.30 setzen
        ```console
        sudo nano /etc/dnsmasq.conf
        #lines to add
        interface=wlan0
        listen-address=192.168.5.1
        dhcp-range=192.168.5.100,192.168.5.200,24h
        ```

5. Konfigurieren der Acesspoint Software hostapd
    1. Änderungen in den Konfigurationsdateien
        ```console
        sudo nano /etc/hostapd/hostapd.conf
        #add the lines for the ap settings. choose your own ssid and wpa_passphrase
        interface=wlan0
        driver=nl80211
        ssid=rallynet
        hw_mode=g
        channel=6
        ieee80211n=1
        wmm_enabled=1
        ht_capab=[HT40][SHORT-GI-20][DSSS_CCK-40]
        macaddr_acl=0
        auth_algs=1
        ignore_broadcast_ssid=0
        wpa=2
        wpa_key_mgmt=WPA-PSK
        wpa_passphrase=raspberry
        rsn_pairwise=CCMP
    2. hostapd mitteilen, wo sich die Konfigurationsdatei befindet
        ```console
        sudo nano /etc/default/hostapd
        ```
        uncoment the line #DEAMON_CONF="" and change to
        ```console
        DAEMON_CONF="/etc/hostapd/hostapd.conf"
        ```
    3. Neustarten des Systems
        ```console
        sudo reboot
        ```
    4. Starten der konfigurierten Services
        ```console
        sudo systemctl start hostapd
        sudo systemctl start dnsmasq
        ````
    5. Manueller Start (nicht empfohlen, nur zum debugen)
        ```code
        sudo hostapd -dd /etc/hostapd/hostapd.conf
        ```
6. Nun sollte es möglich sein, das aufgespannte WLAN Netz mit anderen WLAN-Geräten zu erreichen.
7. Ein einfacher Test ist es, sich mit dem Smartphone zu verbinden. Man sollte das WLAN "rallynet" sehen und nach Eingabe des Passworts eine IP Adresse erhalten.

    <img src="01_Pictures\wifi_list.jpg" width="200">
    <img src="01_Pictures\network_info.jpg" width="200">


## Quellen

[1] [thepi.io](https://thepi.io/how-to-use-your-raspberry-pi-as-a-wireless-access-point/)

[2] [learn.sparkfun.com](https://learn.sparkfun.com/tutorials/setting-up-a-raspberry-pi-3-as-an-access-point/all)
