
/usr/bin/loadgpio.sh

cat /proc/driver/gpio/portAmode | sed "s/[0-1]\([0-1]\{4\}\)$/1\1/" > /proc/driver/gpio/portAmode
cat /proc/driver/gpio/portAdir | sed "s/[0-1]\([0-1]\{4\}\)$/1\1/" > /proc/driver/gpio/portAdir
echo -ne "\x00" > /dev/gpio/PA4
echo -ne "\x01" > /dev/gpio/PA4

#xeno-load ?
