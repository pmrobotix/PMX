#include "LedDriver.hpp"

#include "ev3dev.h"

ALedDriver * ALedDriver::create(std::string, int nb)
{
	return new LedDriver(nb);
}

LedDriver::LedDriver(int nb)
{

	nb_ = nb; //Force number of leds.
}

LedDriver::~LedDriver()
{
}

void LedDriver::setBit(int index, LedColor color)
{
	if (index == 0) //Right
	{
		switch (color)
		{
		case LED_OFF: //Off
			ev3dev::led::green_right.off();
			ev3dev::led::red_right.off();
			break;
		case LED_GREEN: //green
			ev3dev::led::green_right.on();
			ev3dev::led::red_right.off();
			break;
		case LED_RED: //red
			ev3dev::led::green_right.off();
			ev3dev::led::red_right.on();
			break;
		case LED_ORANGE: //orange
			ev3dev::led::green_right.on();
			ev3dev::led::red_right.on();
			break;
		default:
			ev3dev::led::green_right.off();
			ev3dev::led::red_right.off();
			break;
		}
	}

	if (index == 1) //Left
	{
		switch (color)
		{
		case LED_OFF: //Off
			ev3dev::led::green_left.off();
			ev3dev::led::red_left.off();
			break;
		case LED_GREEN: //green
			ev3dev::led::green_left.on();
			ev3dev::led::red_left.off();
			break;
		case LED_RED: //red
			ev3dev::led::green_left.off();
			ev3dev::led::red_left.on();
			break;
		case LED_ORANGE: //orange
			ev3dev::led::green_left.on();
			ev3dev::led::red_left.on();
			break;
		default:
			ev3dev::led::green_left.off();
			ev3dev::led::red_left.off();
			break;
		}
	}
}

void LedDriver::setBytes(uint hex, LedColor color)
{
	for (int i = 0; i < nb_; i++)
	{
		if (((hex >> i) & 0x01) == 1)
		{
			setBit(i, color);
		}
		else
		{
			setBit(i, LED_OFF);
		}
	}
}
