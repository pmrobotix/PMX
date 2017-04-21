//drivers...SIMU

#include "ServoDriver.hpp"

#include "../Log/Logger.hpp"

using namespace std;

AServoDriver * AServoDriver::create()
{
	static ServoDriver *instance = new ServoDriver();
	return instance;
}

ServoDriver::ServoDriver()
		: connected_(0)
{
	logger().debug() << "ServoDriver()" << logs::end;

}

void ServoDriver::hold(ServoLabel servo) // 1 à 8
{
	switch (servo)
	{


	default:
		break;
	}
}

void ServoDriver::setPosition(ServoLabel servo, double percent)
{
	switch (servo)
	{

	default:
		break;
	}
}

void ServoDriver::release(ServoLabel servo)
{
	switch (servo)
	{


	default:
		break;
	}
}

void ServoDriver::setRate(ServoLabel servo, int millisec)
{
	switch (servo)
	{

	default:
		break;
	}
}
