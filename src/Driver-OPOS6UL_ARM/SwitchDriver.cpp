//drivers...ARM

#include "SwitchDriver.hpp"

#include "../Log/Logger.hpp"
#include "GpioPCA9555.hpp"

using namespace std;

ASwitchDriver * ASwitchDriver::create(std::string)
{
	return new SwitchDriver();
}

SwitchDriver::SwitchDriver()
{

	bool c = GpioPCA9555::instance().isConnected();
	if (c)
	{
		//logger().debug() << "GpioPCA9555 CONNECTED !" << logs::end;
		GpioPCA9555::instance().setOnP0(0);
	}
	else
		logger().error() << "GpioPCA9555 NOT CONNECTED !" << logs::end;
}

SwitchDriver::~SwitchDriver()
{
}

int SwitchDriver::pressed(unsigned char pin)
{
	return GpioPCA9555::instance().getValueP1(pin);
}

int SwitchDriver::tirettePressed()
{
	return pressed(7);
}

int SwitchDriver::backLeftPressed()
{
	return pressed(0);
}
int SwitchDriver::backRightPressed()
{
	return pressed(1);
}
