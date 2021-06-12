#!/bin/bash


#TODO sur une carte SD
# install SD
# increase freq CPU
# installation nouveau kernel + wifi5Ghz + i2c 100kHz
# f2fs
# update
# desactivation des services non utiles
# config threads
# config network resolv.conf ?


#Example: deb
#user@host ~ $ scp linux-image-*.deb robot@ev3dev:~
#user@host ~ $ ssh robot@ev3dev
#otheruser@ev3dev:~$ sudo dpkg --install ~/linux-image-*.deb
#otheruser@ev3dev:~$ sudo reboot
#sudo dpkg --install linux-image-4.14.117-ev3dev-2.3.5-ev3_1_armel.deb 
#sudo dpkg --install rtl8812au-modules-4.14.117-ev3dev-2.3.5-ev3_1+1_armel.deb
#sudo dpkg --install rtl8188eu-modules-4.14.117-ev3dev-2.3.5-ev3_8+1_armel.deb



#ARM9 - ARM arch V5 
#So, at the usual 300MHz, this number will be ~150. At 375MHz it will be ~187 and at 456MHz it will be ~228.
lscpu | grep Bogo

ulimit -r


sudo systemctl stop brickman
sudo ./Driver-UnitTest_EV3 >/dev/tty0
sudo systemctl start brickman

#sudo lsblk -o NAME,FSTYPE,SIZE,MOUNTPOINT,LABEL
#mkdir /home/robot/pmx
#sudo chown robot:robot pmx
#create partition mmcblk0p3 with gparted
#edit /etc/fstab
#/dev/mmcblk0p3   /home/robot/pmx   f2fs    rw,acl,active_logs=6,background_gc=on,user_xattr           0 1
#sudo chown robot:robot pmx
#sudo mount -t f2fs /dev/mmcblk0p3 /home/robot/pmx

#cd pmx


 

sudo systemctl stop bluetooth
sudo systemctl stop fake-hwclock.service
sudo systemctl stop systemd-fsck-root.service
sudo systemctl stop systemd-journal-flush.service
sudo systemctl stop nfs-config.service

sudo systemctl disable bluetooth
sudo systemctl disable fake-hwclock.service
sudo systemctl disable systemd-fsck-root.service
sudo systemctl disable systemd-journal-flush.service
sudo systemctl disable nfs-config.service

#sudo systemctl stop systemd-journald.service
#sudo systemctl disable systemd-journald.service
#sudo systemctl stop console-setup.service
#sudo systemctl disable console-setup.service


#keyboard-setup.service, fake-hwclock.service, systemd-journald.service, systemd-fsck-root.service, systemd-journal-flush.service, 
#networking.service, console-setup.service, nfs-config.service.


# pour lancer avec le LCD depuis ssh
sudo systemctl stop brickman
#sudo ./Bot_LegoMindstorm_EV3 m > /dev/tty0

