//drivers...EV3

#include "SwitchDriver.hpp"

#include <string>

#include "../Log/Logger.hpp"


using namespace std;
using namespace ev3dev;

ASwitchDriver * ASwitchDriver::create(std::string botName)
{
	static SwitchDriver *instance = new SwitchDriver();
	return instance;
}

SwitchDriver::SwitchDriver()
{
	touch_ = touch_sensor(INPUT_AUTO);

	if (touch_.connected())
	{
		logger().info() << touch_.type_name()
				<< " connected (device "
				<< touch_.driver_name()
				<< ", port "
				<< touch_.address()
				<< ", mode "
				<< touch_.mode()
				<< ")"
				<< logs::end;
	}else
	{
		logger().error() << "INPUT_1 (Tirette) not Connected !!" << logs::end;
	}
}

SwitchDriver::~SwitchDriver()
{
}


int SwitchDriver::tirettePressed()
{
	return touch_.value();
}

int SwitchDriver::backLeftPressed()
{
	return 0;
}
int SwitchDriver::backRightPressed()
{
	return 0;
}

int SwitchDriver::pressed(unsigned char pin)
{
	return 0;
}
