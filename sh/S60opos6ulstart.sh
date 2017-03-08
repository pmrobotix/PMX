
#scp git/PMX/sh/S60opos6ulstart.sh   root@192.168.0.223:/root/.
#scp git/PMX/sh/S60opos6ulstart.sh   root@192.168.0.222:/root/.

#WIFI
modprobe -r brcmfmac
sleep 1
modprobe brcmfmac
sleep 3
modprobe -r brcmfmac
sleep 1
modprobe brcmfmac
sleep 3

iwconfig
ifconfig wlan0 up
iwlist wlan0 s 
wpa_supplicant -Dwext -i wlan0 -c /etc/wpa_supplicant.conf -B
dhclient wlan0


#I2C
i2cdetect -y -a 0
i2cdetect -y -a 1

#thread info linuxthreads/NPTL
#getconf GNU_LIBPTHREAD_VERSION

ifconfig

