#par wifi PMX
#scp git/PMX/sh/S90opos6ulstart.sh   root@192.168.2.107:/root/.
#par wifi HOME
#scp git/PMX/sh/S90opos6ulstart.sh   root@192.168.0.223:/root/.
#par eth0 HOME
#scp git/PMX/sh/S90opos6ulstart.sh   root@192.168.0.222:/root/.
#par eth0 PMX
#scp git/PMX/sh/S90opos6ulstart.sh   root@192.168.2.105:/root/.




echo set eth0 network
ifconfig eth0 up
udhcpc -n




echo activate WIFI module
modprobe -r brcmfmac
sleep 1
modprobe brcmfmac
sleep 5
modprobe -r brcmfmac
sleep 1
modprobe brcmfmac
sleep 5

echo set WIFI conf
iwconfig
ifconfig wlan0 up
iwlist wlan0 s 
wpa_supplicant -Dwext -i wlan0 -c /etc/wpa_supplicant.conf -B
#echo "nameserver 127.0.0.1" > /etc/resolv.conf
dhclient wlan0

echo ifconfig
ifconfig

echo check I2c address
#I2C UART4
i2cdetect -y -a 0
#I2C UART5
i2cdetect -y -a 1

#thread info linuxthreads/NPTL
#getconf GNU_LIBPTHREAD_VERSION

#pour eviter une connexion à internet.. les DNS sont ajoutés après l'activation du wifi...
echo "nameserver 127.0.0.1" > /etc/resolv.conf


#start PMX
#/root/Bot_ArmadeusOPOS6UL_ARM m
