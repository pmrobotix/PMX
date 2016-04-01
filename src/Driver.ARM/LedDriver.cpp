/*!
 * drivers...ARM
 */

#include "LedDriver.hpp"

#include <unistd.h>
#include <string>

#include "../Log/Exception.hpp"
#include "../Log/Logger.hpp"
#include "HostGpioPort.hpp"

ALedDriver * ALedDriver::create(std::string, int nb)
{
	return new LedDriver(nb);
}

LedDriver::LedDriver(int nb)
{

	nb_ = 8; //Force number of leds.

	try
	{
		for (int i = 0; i < 8; i++)
		{
			paths[i] = new HostGpioPort();
		}

		paths[0]->openIoctl('A', 4);
		paths[1]->openIoctl('A', 5);
		paths[2]->openIoctl('A', 6);
		paths[3]->openIoctl('A', 9);
		paths[4]->openIoctl('A', 8);
		paths[5]->openIoctl('A', 7);
		paths[6]->openIoctl('A', 10);
		paths[7]->openIoctl('A', 11);
		for (int i = 0; i < 8; i++)
		{
			paths[i]->setDirIoctl(1);

			paths[i]->setValueIoctl(0);

		}

	} catch (logs::Exception * e)
	{
		logger().error() << "Exception : " << e->what() << logs::end;
	}
	/*
	 sleep(1);
	 for (int i = 0; i < 8; i++)
	 {

	 paths[i]->setValueIoctl(1);
	 }
	 sleep(1);
	 for (int i = 0; i < 8; i++)
	 {

	 paths[i]->setValueIoctl(0);
	 }*/

	//logger().error() << "Begin" << logs::end;
	/*
	 try
	 {
	 paths_as[0] = new HostGpioPort();
	 logger().error() << "openas" << logs::end;
	 paths_as[0]->openAs(4);
	 //paths_as[0]->setDirectionAs(1);
	 //paths_as[0]->setValueAs(1);

	 } catch (logs::Exception * e)
	 {
	 logger().error() << "Exception : " << e->what() << logs::end;
	 }
	 */
}

LedDriver::~LedDriver()
{
	for (int i = 0; i < 8; i++)
	{
		paths[i]->closeIoctl();
	}
}

void LedDriver::setBit(int index, LedColor color)
{
	int c = 0;
	if (color != LED_OFF)
		c = 1;
	paths[index]->setValueIoctl(c);
}

void LedDriver::setBytes(uint hex, LedColor color)
{
	int c = 0;
	if (color != LED_OFF)
		c = 1;
	for (int i = 0; i < nb_; i++)
	{
		if (((hex >> i) & 0x01) == 1)
		{
			paths[i]->setValueIoctl(c);
		}
		else
		{
			paths[i]->setValueIoctl(0);
		}
	}
}
