//drivers...EV3

#include "ContactDriver.hpp"
#include "ev3dev.h"

using namespace std;
using namespace ev3dev;

AContactDriver * AContactDriver::create()
{
	static ContactDriver *instance = new ContactDriver();
	return instance;
}

ContactDriver::ContactDriver()
{
	touch_ = touch_sensor(INPUT_1);
}

ContactDriver::~ContactDriver()
{
}

int ContactDriver::pressed()
{
	return touch_.value();

}
