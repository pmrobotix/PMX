#include "LedDriver.hpp"

#include <unistd.h>
#include <string>

#include "ev3dev.h"

using namespace std;
using namespace ev3dev;

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
			led::set_color(led::right, led::green);
			break;
		case LED_RED: //red
			led::set_color(led::right, led::red);
			break;
		case LED_ORANGE: //orange
			led::set_color(led::right, led::orange);
			break;
		case LED_AMBER: //amber
			led::set_color(led::right, led::amber);
			break;
		case LED_YELLOW: //yellow
			led::set_color(led::right, led::yellow);
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
			led::set_color(led::left, led::green);
			break;
		case LED_RED: //red
			led::set_color(led::left, led::red);
			break;
		case LED_ORANGE: //orange
			led::set_color(led::left, led::orange);
			break;
		case LED_AMBER: //amber
			led::set_color(led::left, led::amber);
			break;
		case LED_YELLOW: //yellow
			led::set_color(led::left, led::yellow);
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
