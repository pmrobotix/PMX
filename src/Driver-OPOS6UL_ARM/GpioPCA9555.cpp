/*!
 * \file
 * \brief Implémentation de la classe GpioBoard.
 */

#include "GpioPCA9555.hpp"

#include <unistd.h>

#include "../Log/Logger.hpp"

GpioPCA9555::GpioPCA9555() :
        i2cGB_(1), connected_(true), port0Value_(0), port1Value_(0)
{
    begin();
}

void GpioPCA9555::begin()
{
    //open i2c and setslave
    i2cGB_.setSlaveAddr(GPIOBOARD_PCA9555);
    setup(); //setup and enable IN/OUT
}

void GpioPCA9555::setup()
{

    long r = write_i2c(CONFIG_P0, 0x00); //defines all pins on Port0 are outputs
    if (r < 0) {
        connected_ = false;
        logger().error() << "setup() : GpioBoard NOT CONNECTED !" << logs::end;
        return;
    } else {
        write_i2c(OUT_P0, 0x00); //clears all relays
        write_i2c(CONFIG_P1, 0xFF); //defines all pins on Port1 are inputs
        write_i2c(IN_P1, 0x00); //clears all relays
        usleep(PAUSE);
    }

}

void GpioPCA9555::setValueP0(int port, int pin, int value)
{
    if (!connected_) {
        logger().error() << "setValueP0() : GpioBoard NOT CONNECTED !" << logs::end;
        return;
    }

    int out = 0;

    if (value == 1)
        out = port0Value_ | (0x01 << pin);
    else if (value == 0)
        out = port0Value_ & (0xFE << pin);

    write_i2c(port, out);
}

void GpioPCA9555::setOnP0(int pin) // 0 à 7
{
    setValueP0(OUT_P0, pin, 1);
}

void GpioPCA9555::setOffP0(int pin) // 0 à 7
{
    setValueP0(OUT_P0, pin, 0);
}

int GpioPCA9555::getValueP1(int pin)
{
    if (!connected_) {
        logger().error() << "getValueP1() : return 0; GpioBoard NOT CONNECTED !" << logs::end;
        return 0;
    }
    unsigned char in = read_i2c(IN_P1);
    logger().debug() << "getValueP1 in = " << reinterpret_cast<void*>(in) << logs::end;
    int intmp = (in >> pin) & 0x01;
    logger().debug() << "getValueP1 in" << pin << "=" << reinterpret_cast<void*>(intmp) << logs::end;
    return intmp;
}

long GpioPCA9555::write_i2c(unsigned char command, unsigned char value)
{
    return i2cGB_.writeRegByte(command, value);
}

long GpioPCA9555::read_i2c(unsigned char command)
{
    return i2cGB_.readRegByte(command);
}

