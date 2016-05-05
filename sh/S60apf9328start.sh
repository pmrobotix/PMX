
/usr/bin/loadgpio.sh

cat /proc/driver/gpio/portAmode | sed "s/[0-1]\([0-1]\{4\}\)$/1\1/" > /proc/driver/gpio/portAmode
cat /proc/driver/gpio/portAdir | sed "s/[0-1]\([0-1]\{4\}\)$/1\1/" > /proc/driver/gpio/portAdir
echo -ne "\x00" > /dev/gpio/PA4
echo -ne "\x01" > /dev/gpio/PA4

#xeno-load ?

#I2C change frequency 96 000 000 / IFDR 
# { 960,  0x17 } = 100 000 kHz
# { 288,  0x10 } = 333 333 kHz (defaut)
# { 240,  0x0F } = 375 000 kHz
# { 256,  0x33 } = 400 000 kHz
imxregs IFDR 0x33
imxregs IFDR


modprobe max1027
export ADC_SPI="spi1.0"


modprobe spidev

modprobe servo

i2cdetect -y -a 0


cd /pmx

./Bot_ArmadeusAPF9328_ARM m
