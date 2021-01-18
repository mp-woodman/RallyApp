#!/bin/bash

echo "start to connect..."
sudo qmicli -d /dev/cdc-wdm0 --dms-set-operating-mode='online'
sudo ip link set wwan0 down
echo 'Y' | sudo tee /sys/class/net/wwan0/qmi/raw_ip
sudo ip link set wwan0 up
sudo qmicli -p -d /dev/cdc-wdm0 --device-open-net='net-raw-ip|net-no-qos-header' --wds-start-network="apn='web.vodafone.de',username='pi',password='Rally2020$',ip-type=4" --client-no-release-cid
sudo udhcpc -i wwan0
ip a s wwan0
echo "connect finished"
