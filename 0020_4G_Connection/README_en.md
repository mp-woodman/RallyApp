<!--- Cyber Physical Systems
Author: <Max Holzmann>  Date: <2020 11 29> 
Changes by:
<NAME> - <JJJJ MMM DD> - <comment> 

--->
**Cyber Physical Systems**   <img style="float:right" src="../0000_Global_Files/HM_SchriftzugLogo_RGB.png" width="200">  

Wintersemester 2020/21
***

# Lesson 02: 4G connection with the Raspbberry
by: Max Holzmann
***

## Objectives

- setup the 4G connection as interface the Raspberry Pi
- run the connection process in the autostart by a bash script
- configure NTP service of the py to yout favourite NTP Server 

<img src="01_Pictures\4G_HAT.jpg" height="200">
<img src="01_Pictures\4G_HAT_with_ant.jpg" height="200">

## Prerequisites and required Equipment
- the tutorial lesson 01: General Setup must be done [link](../0010_General_Setup/README.md)
- Hardware
    - 1x Raspberry Pi 3 B
    - 1x Waveshare 4G HAT SIM7600E-H [link producer](https://www.waveshare.com/wiki/SIM7600E-H_4G_HAT) [link distributor](https://www.berrybase.de/neu/4g/3g/2g/gsm/gprs/gnss-hat-f-252-r-raspberry-pi-eu-version)
    - 1x SIM Card (with 4G option, and with out pin lock). I use a card from Vodafone Germany
- Software and the tools uses in this tutorial
    - VNC Viwer as remote desktop
    - Putty as SSH Terminal


## Solutions Steps 

### 1. Hardware Preparation 
1. insert the SIM Card (without pinlock) into the 4G HAT
2. HAT UART Jumper to A
3. HAT VCC Jumper set to 5V
4. connect GPS antenna
5. connect Main antenna
6. connect the HAT with the Pi via GPIO
7. connect the HAT via USB (HAT USB to Uart -> USB Port Pi) additionaly
8. power your System
9. --> the Py will boot and the PWR LED and the NET LED of the HAT should shine red

### 2. There are more ways to use the 4G HAT

I choose the way to connect the HAT as interface to the py. This take some time druing the instalation but than you have a connection to the internet without additional effort. 

If you are interested in handling the AT comandy by your own you scholuld unse the way described in the maual of the HAT.

### 3. To connect the 4G hat as interface I used a lib for Qualcomm  devices called libqmi-utils.
1. enable the Serial Port and explizit disable Serial Console in the raspi configuration
    ```console
    sudo raspi-config
    #enable the serial Port
    #disable the Serial console
    sudo reboot
    ```
2. enable UART by adding at the end of file /boot/config.txt the line
    ```console
    #open file
    sudo nano /boot/config.txt
    #add line at end of file
    enable_uart=1
    ```
3. update pi and install libqmi-utils and udhcpc
    ```console
    sudo apt-get update && sudo apt-get install libqmi-utils udhcpc
    ```
4. start the SIM7600E Module by qmicli software
    ```console
    sudo qmicli -d /dev/cdc-wdm0 --dms-set-operating-mode='online'
    ```
5. give unser Pi the rights for the interface
    ```console
    sudo chown pi /dev/cdc-wdm0
    ```
6. check if connect to network was sucessfull by
    ```console
    qmicli -d /dev/cdc-wdm0 --dms-get-operating-mode
    qmicli -d /dev/cdc-wdm0 --nas-get-signal-strength
    qmicli -d /dev/cdc-wdm0 --nas-get-home-network
    ```
7. change the 802-3 protocoll to the raw-ip protocoll for wwan0
    ```console
    sudo qmicli -d /dev/cdc-wdm0 -w
    sudo ip link set wwan0 down
    echo 'Y' | sudo tee /sys/class/net/wwan0/qmi/raw_ip
    sudo ip link set wwan0 up
    ```
8. connect to network. be carefull user your pasword oth the pi user
    ```console
    qmicli -p -d /dev/cdc-wdm0 --device-open-net='net-raw-ip|net-no-qos-header' --wds-start-network="apn='web.vodafone.de',username='pi',password='Rally2020$',ip-type=4" --client-no-release-cid
    ```
9. set default route and get ip
    ```console
    sudo udhcpc -i wwan0
    ip a s wwan0
    ```
10. now you should be connected to the 4G net. check ist out by dicsonecting all other networc connections and open a website with the browser

11. That you dont have to run all this commands manualy after every boot. These commands combined in a bash script.
i placed the script on the pi in the following directory. In this repo, you find the file [here](00_Code/startInternetConnect.sh)
    ```console
    /home/pi/rally/startInternetConnect.sh
    ```

12. To run the Bashscript in autostart add the following line at the end of the file /etc/rc.local 
    ```console
    #open file
    sudo nano /etc/rc.local
    #line to add at the end
    sudo sh /home/pi/rally/startInternetConnect.sh &
    ```

### 4. Configure NTP service of the py to yout favourite NTP Server
It is important to sync the time of the measuring system with the local time. Otherwise the whole measuring process will be coroupt.

1. To sync with a local ntp Server the Timedatectl service has to be configured.
    ```console
    sudo nano /etc/systemd/timesyncd.conf
    ```
2. these lines has to bechanges to local timeservers. I used the ntp server from vodavone cause the 4G module is connectet to the vodafone net. as fallback i use german ntp server.
    ```console
    [Time]
    NTP=ntp0.one.vodafone-ip.de          
    FallbackNTP=ptbtime1.ptb.de 0.de.pool.ntp.org
    ```

## Further Inputs
nothing

## Useful Resources for Own Searches

4G HAT as Interface

[tutorial 1](https://developers.telnyx.com/docs/v2/wireless/tutorials/sim7600)

[usefull forum entry](https://www.raspberrypi.org/forums/viewtopic.php?t=224355) (Re: 4G Hat Quote Wed Apr 03, 2019 11:35 am)


## Sources

[1] [Berry Base Shop](https://www.berrybase.de/neu/4g/3g/2g/gsm/gprs/gnss-hat-f-252-r-raspberry-pi-eu-version)

[2] [developers.telnyx.com](https://developers.telnyx.com/docs/v2/wireless/tutorials/sim7600)

[3] [raspberrytips.com](https://raspberrytips.com/time-sync-raspberry-pi/ )
