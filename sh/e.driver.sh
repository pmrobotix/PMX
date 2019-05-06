#!/bin/bash

#ARM9 - ARM arch V5 
#So, at the usual 300MHz, this number will be ~150. At 375MHz it will be ~187 and at 456MHz it will be ~228.
lscpu | grep Bogo


sudo systemctl stop brickman
sudo ./Driver-UnitTest_EV3 >/dev/tty0
sudo systemctl start brickman

#sudo lsblk -o NAME,FSTYPE,SIZE,MOUNTPOINT,LABEL
#mkdir /home/robot/pmx
#sudo chown robot:robot pmx
#sudo mount -t f2fs /dev/mmcblk0p3 pmx
#cd pmx