//drivers...EV3

#include "SwitchDriver.hpp"

#include "../Log/Logger.hpp"


using namespace std;
using namespace ev3dev;

ASwitchDriver * ASwitchDriver::create()
{
	static SwitchDriver *instance = new SwitchDriver();
	return instance;
}

SwitchDriver::SwitchDriver()
{
	touch_ = touch_sensor(INPUT_1);

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

int SwitchDriver::pressed()
{
	return touch_.value();
}

int SwitchDriver::pressed(unsigned char pin)
{
	return 0;
}
