#!/bin/bash
sudo systemctl stop brickman
sudo ./Driver-UnitTest_EV3 >/dev/tty0
sudo systemctl start brickman

#sudo lsblk -o NAME,FSTYPE,SIZE,MOUNTPOINT,LABEL
#mkdir /home/robot/pmx
#sudo chown robot:robot pmx
#sudo mount -t f2fs /dev/mmcblk0p3 pmx
#cd pmx