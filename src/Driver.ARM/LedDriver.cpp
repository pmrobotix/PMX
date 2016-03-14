/*!
 * drivers...ARM
 */

#include "LedDriver.hpp"

ALedDriver * ALedDriver::create(std::string, int nb)
{
	return new LedDriver(nb);
}

LedDriver::LedDriver(int nb)
{

	nb = 8; //force 8 leds.
/*
	gpio[0] = new AsGpio(4); //A4
	gpio[1] = new AsGpio(5); //A5
	gpio[2] = new AsGpio(6); //A6
	gpio[3] = new AsGpio(9); //A9
	gpio[4] = new AsGpio(8); //A8
	gpio[5] = new AsGpio(7); //A7
	gpio[6] = new AsGpio(10); //A10
	gpio[7] = new AsGpio(11); //A11

	for (int i = 0; i < nb; i++)
	{
		gpio[i]->setPinDirection(OUT);
		gpio[i]->setPinValue(0);
	}*/
}

LedDriver::~LedDriver()
{
	//delete (gpio);
}

void LedDriver::setBit(int index, LedColor color)
{
	//gpio[position]->setPinValue(aValue);
}

void LedDriver::setBytes(uint hex, LedColor color)
{

}
