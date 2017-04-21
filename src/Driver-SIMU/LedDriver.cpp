//drivers...SIMU

#include "LedDriver.hpp"

#include <sstream>
#include <string>

#include "../Log/Logger.hpp"

using namespace std;

ALedDriver * ALedDriver::create(std::string, int nb)
{
	return new LedDriver(nb);
}

LedDriver::LedDriver(int nb)
{
	hexa = 0;
	nb_ = nb;
	gpio = new int[nb_];

	for (int i = 0; i < nb_; i++)
	{
		gpio[i] = 0;
	}
}

LedDriver::~LedDriver()
{
}

void LedDriver::setBit(int index, LedColor color)
{
	gpio[index] = color;

	/*
	 Changing the nth bit to x

	 Setting the nth bit to either 1 or 0 can be achieved with the following:

	 number ^= (-x ^ number) & (1 << n);
	 Bit n will be set if x is 1, and cleared if x is 0.
	 */
	hexa ^= (-1 ^ hexa) & (1 << index);

	ostringstream ost;
	ost << "LED ";
	for (int i = 0; i < nb_; i++)
	{
		if (i == index)
			ost << "\033[1m" << "\033[4;31m" << gpio[i] << "\033[0m";
		else
			ost << "\033[0m" << gpio[i];
	}
	ost << " (POS=" << index << ")";
	logger().debug() << ost.str() << logs::end;

	//logger().info() << "LED " << std::bitset<8>(hexa) << " (POS=" << index << ")" << logs::end;
}

void LedDriver::setBytes(uint hex, LedColor color)
{
	hexa = hex;

	for (int i = 0; i < nb_; i++)
	{
		if (((hex >> i) & 0x01) == 1)
		{
			gpio[i] = color;
		}
		else
		{
			gpio[i] = 0;
		}
	}

	ostringstream ost;
	ost << "LED ";
	for (int i = 0; i < nb_; i++)
	{
		ost << gpio[i];
	}
	logger().info() << ost.str() << logs::end;
}
