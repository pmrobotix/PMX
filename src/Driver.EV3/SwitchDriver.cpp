//drivers...EV3

#include "ev3dev.h"
#include "SwitchDriver.hpp"

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
}

SwitchDriver::~SwitchDriver()
{
}

int SwitchDriver::pressed()
{
	return touch_.value();
}
