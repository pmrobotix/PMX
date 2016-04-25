//drivers...SIMU

#include "ServoDriver.hpp"

#include <string>

#include "../Log/Logger.hpp"

using namespace std;


AServoDriver * AServoDriver::create()
{
	static ServoDriver *instance = new ServoDriver();
	return instance;
}

ServoDriver::ServoDriver() :
		connected_(0)
{
	logger().debug() << "ServoDriver()" << logs::end;


}

void ServoDriver::hold(int servoId) // 1 à 8
{
	switch (servoId)
	{
	case 1:

		break;
	case 8:

		break;
	default:
		break;
	}
}

void ServoDriver::setPosition(int servoId, int pos)
{
	switch (servoId)
	{
	case 1:

		break;
	case 8:

		break;
	default:
		break;
	}
}

void ServoDriver::release(int servoId)
{
	switch (servoId)
	{
	case 1:

		break;
	case 8:

		break;
	default:
		break;
	}
}
