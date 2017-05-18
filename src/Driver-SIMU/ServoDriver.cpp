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

void ServoDriver::hold(int servo) // 1 à 8
{
	switch (servo)
	{


	default:
		break;
	}
}

void ServoDriver::setPosition(int servo, int percent)
{
	switch (servo)
	{

	default:
		break;
	}
}

void ServoDriver::release(int servo)
{
	switch (servo)
	{


	default:
		break;
	}
}

void ServoDriver::setRate(int servo, int millisec)
{
	switch (servo)
	{

	default:
		break;
	}
}
