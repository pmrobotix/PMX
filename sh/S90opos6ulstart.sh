#!/bin/sh

#par wifi PMX 2.6Ghz
#scp git/PMX/sh/S90opos6ulstart.sh   root@192.168.2.107:/root/.
#par wifi HOME 2.6Ghz
#scp git/PMX/sh/S90opos6ulstart.sh   root@192.168.0.223:/root/.
#par eth0 HOME 2.6Ghz
#scp git/PMX/sh/S90opos6ulstart.sh   root@192.168.0.222:/root/.
#par eth0 PMX 2.6Ghz
#scp git/PMX/sh/S90opos6ulstart.sh   root@192.168.2.105:/root/.

#ln -s /root/S90opos6ulstart.sh /etc/init.d/.


echo set eth0 network
ifconfig eth0 up
udhcpc -n

#ancienne version
#modprobe 8812au



#echo activate internal WIFI module
#modprobe -r brcmfmac
#sleep 1
#modprobe brcmfmac
#sleep 5
#modprobe -r brcmfmac
#sleep 1
#modprobe brcmfmac
#sleep 5

echo activate rtl8812au WIFI module
modprobe rtl8812au

echo set WIFI conf
iwconfig
ifconfig wlan0 up
#lister les reseaux disponible
#iwlist wlan0 s 
wpa_supplicant -Dwext -i wlan0 -c /etc/wpa_supplicant.conf -B

#pour eviter une connexion à internet.. les DNS sont ajoutés après l'activation du wifi...
#echo "nameserver 127.0.0.1" > /etc/resolv.conf

#configuration du dhcp
#dhclient wlan0
#ifconfig wlan0 192.168.3.104 netmask 255.255.255.0 up

#IP for PMX_AP_5G (box 6137)
ifconfig wlan0 192.168.4.103 netmask 255.255.255.0 up


echo ifconfig
ifconfig

echo check I2c address
#I2C UART4
i2cdetect -y -a 0
#I2C UART5
i2cdetect -y -a 1
i2cdetect -y -a 2
i2cdetect -y -a 3

#liste des i2c
#bus 0
#MBED I2C_SLAVE_ADDRESS 0x05
#bus 1
#GPIOBOARD_PCA9555 24
#MCP23017_ADDRESS 20
#AX12ADC_ADDR 08
#GROVE_COLOR_DEFAULT_ADDRESS 39
#ADDRESS_gp2y0e02b 40


# modprobe spidev
# ls /dev/spidev*

#thread info linuxthreads/NPTL //TODO A verifier sur opos6ul !!!
#getconf GNU_LIBPTHREAD_VERSION

#change info processor
echo performance > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq
cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq


#pour ajouter de la place sur la partition 2
# resize2fs /dev/mmcblk0p2

#pour la partition 3 de data

#mount  /dev/mmcblk0p3 /media/data/

#F2FS
# mettre un # dans /etc/fstab sur la ligne /dev/mmcblk0p3 
#mkdir /root/pmx
#formater en f2fs
#umount /dev/mmcblk0p3
#mkfs.f2fs /dev/mmcblk0p3
#mount -t f2fs /dev/mmcblk0p3 /root/pmx

#nano /etc/fstab
#/dev/mmcblk0p3   /root/pmx   f2fs    rw,acl,active_logs=6,background_gc=on,user_xattr           0 1

#cd /root/pmx
#start PMX
#/root/pmx/Bot_ArmadeusOPOS6UL_ARM m
#/root/pmx/Bot_ArmadeusOPOS6UL_ARM m > log.txt &

